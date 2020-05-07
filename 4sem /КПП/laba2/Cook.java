package laba2;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

public class Cook extends Human{
	Food makeFood(Food f) {
		f.title = JOptionPane.showInputDialog("Введите название:");
		f.quantity = JOptionPane.showInputDialog("Введите количество:");
		f.taste = JOptionPane.showInputDialog("Введите вкус:");
		String str1 = "Повар приготовил блюдо";
		String str2 = "Название: " + f.title;
		String str3 = "Количество: " + f.quantity;
		String str4 = "Вкус: " + f.taste;
		JFrame infowindow1 = new InfoWindow(str1, str2, str3, str4);
		
		return f;
	}
}
