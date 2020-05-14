package Windows;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;

import Food.Food;
import Food.Vegetable;
import People.Cook;
import People.Farmer;
import People.Programmer;

public class Window extends JFrame{
	private Programmer p = new Programmer();
	private Cook c = new Cook();
	private Food f = new Food();
	private Farmer fm = new Farmer();
	private Vegetable v = new Vegetable();
	
	private String str = null;
	
	private JButton button1 = new JButton("Приготовить еду");
	private JButton button2 = new JButton("Написать код");
	private JButton button3 = new JButton("Накормить повара");
	private JButton button4 = new JButton("Накормить программиста");
	private JButton button5 = new JButton("Накормить фермера");
	private JButton button6 = new JButton("Посадить овощ");
	private JButton button7 = new JButton("Полить овощ");
	private JButton button8 = new JButton("Вырастить овощ");
	
	private ActionListener action1 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			f = c.makeFood(f, fm);
			v.title = null;
			v.quantity = null;
			v.form = null;
			v.color = null;
			fm.setPlant(false);
			fm.setPour(false);
			fm.setGrow(false);
		}
	};
	private ActionListener action2 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			p.writeCode();
		}
	};
	private ActionListener action3 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			str = "Повар";
			c.eat(f, str);
			f.title = null;
			f.quantity = null;
			f.taste = null;
		}
	};
	private ActionListener action4 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			str = "Программист";
			p.eat(f, str);
			f.title = null;
			f.quantity = null;
			f.taste = null;
		}
	};
	private ActionListener action5 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			str = "Фермер";
			p.eat(f, str);
			f.title = null;
			f.quantity = null;
			f.taste = null;
		}
	};
	private ActionListener action6 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			v = fm.plantVegetable(v);
			fm.setPlant(true);
		}
	};
	private ActionListener action7 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			fm.pourVegetable();
			fm.setPour(true);
		}
	};
	private ActionListener action8 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			fm.growVegetable();
			fm.setGrow(true);
		}
	};
	
	public Window(String name){
		super(name);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(400, 200);
		setLocation(400, 400);
		setVisible(true);
		this.getContentPane().setLayout(new GridLayout(4, 2, 5, 5));
		
		getContentPane().add(button1);
		getContentPane().add(button2);
		getContentPane().add(button3);
		getContentPane().add(button4);
		getContentPane().add(button5);
		getContentPane().add(button6);
		getContentPane().add(button7);
		getContentPane().add(button8);
		
		button1.addActionListener(action1);
		button2.addActionListener(action2);
		button3.addActionListener(action3);
		button4.addActionListener(action4);
		button5.addActionListener(action5);
		button6.addActionListener(action6);
		button7.addActionListener(action7);
		button8.addActionListener(action8);

	}
}