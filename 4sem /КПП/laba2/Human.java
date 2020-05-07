package laba2;

import javax.swing.JFrame;

public class Human {
	private Programmer programmer;
	private Cook cook;
	private Item item;
	
	String name;
	
	Human(){
		programmer = null;
		cook = null;
		item = null;		
	}
	public Human(Programmer p, Cook c, Item i) {
		this.programmer = p;
		this.cook = c;
		this.item = i;
	}
	
	public Programmer getProgrammer() {
		return programmer;
	}
	public Cook getCook() {
		return cook;
	}
	public Item getItem() {
		return item;
	}
	
	public void eat(Food f, String str) {
		if(f.taste == null) {
			String message1 = "ѕовар еще не приготовил еду!";
			JFrame infowindow1 = new InfoWindow(message1, null, null, null);
		}
		else {
			String message2 = str + " накормлен " + f.taste + " едой";
			JFrame infowindow2 = new InfoWindow(message2, null, null, null);
		}
	}
}
