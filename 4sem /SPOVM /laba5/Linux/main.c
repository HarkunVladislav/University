#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glob.h>
#include <pthread.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
 
#define BUF_SIZE 1024
 
sem_t read_sem;
sem_t write_sem;
 
int (*async_read)(int file, char* buffer, size_t size, off_t offset);
int (*async_write)(int file, char* buffer, size_t size, off_t offset);
void (*print)();
 
char buffer[BUF_SIZE];
 
int errfunc(const char* epath, int errno)
{
    fprintf(stderr, "Glob error: %s, %d\n", epath, errno);
    return 0;
}
 
void* writer_thread_routine(void* args)
{
    off_t offset = 0, bytes_written = 0;
    size_t bytes_numb_to_write = 0;
 
    while (1)
    {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
 
        sem_wait(&read_sem);
 
        if (strlen(buffer) >= BUF_SIZE)
        {
            bytes_numb_to_write = BUF_SIZE;
        }
        else
        {
            bytes_numb_to_write = strlen(buffer);
        }
 
        bytes_written = async_write(*((int*)args), buffer, bytes_numb_to_write, offset);
        offset += bytes_written;
       
        sem_post(&write_sem);
 
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    }
 
    pthread_exit(NULL);
}
 
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("Invalid input!\n");
        return 1;
    }
 
    void* handle = dlopen("/home/vladislav/SPOVM/laba5/dll.so", RTLD_LAZY); //загружает динамическую библиотеку
    if (!handle)
    {
        fprintf(stderr, "Can't lopad dll\n");
        return 1;
    }
 
    *(void**)(&async_read) = dlsym(handle, "async_read"); //возвращает адрес, по которому объект расположен в памяти
    *(void**)(&async_write) = dlsym(handle, "async_write");
 
    if (dlerror()) //значение, описывающее последнюю ошибку
    {
        fprintf(stderr, "Can't load dll function\n");
        dlclose(handle);
        return 1;
    }
 
    char cur_dir[PATH_MAX];
    getcwd(cur_dir, PATH_MAX); //возвращает текущий рабочий каталог
 
    int output_file = open(argv[2], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); //открывает файл
 
    if (chdir(argv[1]) == -1) //изменяет текущий каталог
    {
        fprintf(stderr, "Can't open required directory!\n");
        return 1;
    }
 
    glob_t gl;
    glob("*", GLOB_ERR, errfunc, &gl); //поиск имен всех файлов
 
    sem_init(&write_sem, 0, 1); // инициализирует семафор по адрсу, будет доступен в потока процесса, будет коллективно использоваться в потоках и может располагаться в общей области памяти
    sem_init(&read_sem, 0, 0);                                                                    //,не будет использоваться в потоках 
 
    pthread_t thread;
    pthread_create(&thread, NULL, writer_thread_routine, (void*)&output_file); 
 
    off_t offset = 0;
    off_t bytes_read = 0;
 
    for (int i = 0; i < gl.gl_pathc; i++) //проходимя по файлам
    {
        int source;
 
        if ((source = open(gl.gl_pathv[i], O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) //открываем i-ый файл
        {
            fprintf(stderr, "Can't open file %s\n", gl.gl_pathv[i]);
            break;
        }
 
        do
        {
            sem_wait(&write_sem);
 
            bytes_read = async_read(source, buffer, BUF_SIZE, offset);
            offset += bytes_read;
 
            if (bytes_read < BUF_SIZE)
            {
                buffer[bytes_read] = '\0';
            }
           
            sem_post(&read_sem);
        } while (bytes_read == BUF_SIZE);
 
        offset = 0;
        bytes_read = 0;
 
        close(source);
    }
 
    sem_wait(&write_sem);
    pthread_cancel(thread);
 
    close(output_file);
    chdir(cur_dir);
    globfree(&gl);
    sem_close(&write_sem);
    sem_close(&read_sem);
    dlclose(handle);
 
    return 0;
}