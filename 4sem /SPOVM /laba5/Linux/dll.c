#include <aio.h>
#include <fcntl.h>
#include <string.h>
 
int async_read(int file, char* buffer, size_t size, off_t offset)
{
    struct aiocb cb;
    memset(&cb, 0, sizeof(struct aiocb));
    cb.aio_fildes = file; //дескриптор файла
    cb.aio_offset = offset; // смещение файла
    cb.aio_nbytes = size; //длина передачи
    cb.aio_lio_opcode = LIO_READ; //выполнить операцию чтения
    cb.aio_buf = buffer; //буфер
    struct sigevent se; //структура для уведомления от асинхронных программ
    se.sigev_notify = SIGEV_NONE; //уведомления не происходит
    cb.aio_sigevent = se; //номер сигнала и значение
 
    aio_read(&cb); //ставит в очередь запрос,описанный в буфере
 
    while (aio_error(&cb) != 0) //получение статуса ошибки для асинхронной операции
        ;
 
    return aio_return(&cb); //получение состояния возврата асинхронной операции ввода-вывода
}
 
int async_write(int file, char* buffer, size_t size, off_t offset)
{
    struct aiocb cb;
    memset(&cb, 0, sizeof(struct aiocb));
    cb.aio_fildes = file;
    cb.aio_offset = offset;
    cb.aio_nbytes = size;
    cb.aio_lio_opcode = LIO_WRITE;
    cb.aio_buf = buffer;
    struct sigevent se;
    se.sigev_notify = SIGEV_NONE;
    cb.aio_sigevent = se;
 
    aio_write(&cb);
 
    while (aio_error(&cb) != 0)
        ;
 
    return aio_return(&cb);
}