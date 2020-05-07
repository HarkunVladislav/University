package laba2;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;

public class Window extends JFrame{
	Programmer p = new Programmer();
	Cook c = new Cook();
	Item i = new Item();
	Food f = new Food();
	private Human human = new Human(p, c, i);
	private String str = null;
	
	private JButton button1 = new JButton("����������� ���");
	private JButton button2 = new JButton("�������� ���");
	private JButton button3 = new JButton("��������� ������");
	private JButton button4 = new JButton("��������� ������������");
	
	private ActionListener action1 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			f = human.getCook().makeFood(f);
		}
	};
	private ActionListener action2 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			human.getProgrammer().writeCode();
		}
	};
	private ActionListener action3 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			str = "�����";
			human.eat(f, str);
			f.title = null;
			f.quantity = null;
			f.taste = null;
		}
	};
	private ActionListener action4 = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			str = "�����������";
			human.eat(f, str);
			f.title = null;
			f.quantity = null;
			f.taste = null;
		}
	};
	
	Window(String name){
		super(name);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(400, 250);
		setLocation(400, 400);
		setVisible(true);
		this.getContentPane().setLayout(new GridLayout(4, 1, 20, 20));
		
		getContentPane().add(button1);
		getContentPane().add(button2);
		getContentPane().add(button3);
		getContentPane().add(button4);
		
		button1.addActionListener(action1);
		button2.addActionListener(action2);
		button3.addActionListener(action3);
		button4.addActionListener(action4);
	}
}