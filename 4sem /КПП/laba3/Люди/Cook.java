package People;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

import Food.Food;
import Windows.InfoWindow;

public class Cook extends Human{
	public Food makeFood(Food f, Farmer fm) {
		if(fm.grow == false) {
			String str = "Фермер еще не вырастил овощи!";
			JFrame infowindow1 = new InfoWindow(str, null, null, null, null);
		}
		else {
			f.title = JOptionPane.showInputDialog("Введите название:");
			f.quantity = JOptionPane.showInputDialog("Введите количество:");
			f.taste = JOptionPane.showInputDialog("Введите вкус:");
			String str1 = "Повар приготовил блюдо";
			String str2 = "Название: " + f.title;
			String str3 = "Количество: " + f.quantity;
			String str4 = "Вкус: " + f.taste;
			JFrame infowindow2 = new InfoWindow(str1, str2, str3, str4, null);
		}
		
		return f;
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
