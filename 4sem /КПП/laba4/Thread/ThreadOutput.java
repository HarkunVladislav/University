package Thread;

public class ThreadOutput {
	public static synchronized void StringOutput(String str) {
		for(int j = 0; j < str.length(); ++j) {
			System.out.printf("%c", str.charAt(j));
		}
		System.out.printf("\n");
	}
}
