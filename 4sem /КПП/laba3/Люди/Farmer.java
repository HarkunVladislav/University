package People;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import Food.Vegetable;
import Windows.InfoWindow;

public class Farmer {
	private boolean plant;
	private boolean pour;
	protected boolean grow;
	
	public Farmer(){
		this.plant = false;
		this.pour = false;
		this.grow = false;
	}
	
	public Vegetable plantVegetable(Vegetable v) {
		v.title = JOptionPane.showInputDialog("Введите название:");
		v.quantity = JOptionPane.showInputDialog("Введите количество:");
		v.form = JOptionPane.showInputDialog("Введите вид:");
		v.color = JOptionPane.showInputDialog("Введите цвет:");
		
		String str1 = "Фермер посадил овощ";
		String str2 = "Название: " + v.title;
		String str3 = "Количество: " + v.quantity;
		String str4 = "Вид: " + v.form;
		String str5 = "Цвет: " + v.color;
		JFrame infowindow1 = new InfoWindow(str1, str2, str3, str4, str5);
		
		return v;
	}
	
	public void pourVegetable() {
		if(this.plant == false)
		{
			String str1 = "Овощ еще не посажен!";
			JFrame infowindow1 = new InfoWindow(str1, null, null, null, null);
		}
		else {
			String str2 = "Овощ полит";
			JFrame infowindow2 = new InfoWindow(str2, null, null, null ,null);
		}
	}
	
	public void growVegetable() {
		if(this.plant == false)
		{
			String str1 = "Овощ еще не посажен!";
			JFrame infowindow1 = new InfoWindow(str1, null, null, null, null);
			return;
		}
		if(this.pour == false)
		{
			String str1 = "Овощ еще не полит!";
			JFrame infowindow1 = new InfoWindow(str1, null, null, null, null);
		}
		else {
			String str2 = "Овощ вырощен";
			JFrame infowindow2 = new InfoWindow(str2, null, null, null ,null);
		}
	}
	
	public void setPlant(boolean b) {
		this.plant = b;
	}
	public void setPour(boolean b) {
		this.pour = b;
	}
	public void setGrow(boolean b) {
		this.grow = b;
	}
}
