package Thread;

public class ThreadWork implements Runnable{
	public void run() {
		for(int i = 0; i < 50; ++i)
		{
			String str = Thread.currentThread().getName() + "_" + Integer.toString(i + 1);
			ThreadOutput.StringOutput(str);
			System.out.printf("\n");
			
			try {
				Thread.sleep(100);
			}
			catch(InterruptedException exception) {
				System.out.println("Thread has been interrupted\n");
			}
		}
	}
}