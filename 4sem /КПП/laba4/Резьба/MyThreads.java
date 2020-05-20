package Thread;

public class MyThreads extends Thread{
	public static void Function(int quantity) {
		String threadStr = null;
		for(int i = 0; i < quantity; ++i) {
			threadStr = "Thread_" + Integer.toString(i + 1);
			Thread thread = new Thread(new ThreadWork(), threadStr);
			thread.start();
		}
	}
}
