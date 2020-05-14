package People;

import javax.swing.JFrame;

import Food.Food;
import Windows.InfoWindow;

public class Programmer extends Human{
	public void writeCode() {
		int n = 0;
		char c;
		String code = "Код: ";
		
		for(int i = 0; i < 10; ++i) {
			c = 'a';
			n = (int)(Math.random()*(26));
			c += n;
			code += c;
		}
		
		JFrame infowindow2 = new InfoWindow(code, null,null, null, null);
	}
	
	public void eat(Food f, String str) {
		if(f.taste == null) {
			String message1 = "Повар еще не приготовил еду!";
			JFrame infowindow1 = new InfoWindow(message1, null, null, null, null);
		}
		else {
			String message2 = str + " накормлен " + f.taste + " едой";
			JFrame infowindow2 = new InfoWindow(message2, null, null, null, null);
		}
	}
}
