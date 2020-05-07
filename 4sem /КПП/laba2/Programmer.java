package laba2;

import javax.swing.JFrame;

public class Programmer extends Human{
	void writeCode() {
		int n = 0;
		char c;
		String code = "Код: ";
		
		for(int i = 0; i < 10; ++i) {
			c = 'a';
			n = (int)(Math.random()*(26));
			c += n;
			code += c;
		}
		
		JFrame infowindow2 = new InfoWindow(code, null,null, null);
	}
}
