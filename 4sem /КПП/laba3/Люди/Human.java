package People;

import javax.swing.JFrame;

import Food.Food;

public abstract class Human {
	private Programmer programmer;
	private Cook cook;
	
	String name;
	
	Human(){
		programmer = null;
		cook = null;		
	}
	public Human(Programmer p, Cook c) {
		this.programmer = p;
		this.cook = c;
	}
	
	public Programmer getProgrammer() {
		return programmer;
	}
	public Cook getCook() {
		return cook;
	}
		
	public abstract void eat(Food f, String str);
}
