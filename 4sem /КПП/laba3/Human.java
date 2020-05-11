package laba2;

import javax.swing.JFrame;

public abstract class Human {
	private Programmer programmer;
	private Cook cook;
	//private Item item;
	
	String name;
	
	Human(){
		programmer = null;
		cook = null;
	//	item = null;		
	}
	public Human(Programmer p, Cook c) {
		this.programmer = p;
		this.cook = c;
		//this.item = i;
	}
	
	public Programmer getProgrammer() {
		return programmer;
	}
	public Cook getCook() {
		return cook;
	}
		
	public abstract void eat(Food f, String str);
}
