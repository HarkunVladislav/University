package GUI;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextPane;

import Thread.MyThreads;

public class Gui extends JFrame{
	private JLabel txt1 = new JLabel("Введите число строк, которые будут выводиться");
	private JLabel txt2 = new JLabel("");
	private JTextPane input = new JTextPane();
	
	private JButton button = new JButton("Запустить работу потоков");
	
	public ActionListener action = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			int quantity = 0;
			String str = input.getText();
			if(!str.isEmpty()) {
				MyThreads ob = new MyThreads();
				try {
					ob.Function(Integer.parseInt(str));
					txt2.setText("Строки успешно выведены");
					getContentPane().add(txt2);
					txt2.setBounds(20, 80, 290, 30);
				}catch(NumberFormatException exception) {
					txt2.setText("Вы ввели не число!");
					getContentPane().add(txt2);
					txt2.setBounds(20, 80, 290, 30);
					return;
				}
			}
			else {
				txt2.setText("Вы не ввели число!");
				getContentPane().add(txt2);
				txt2.setBounds(20, 80, 290, 30);
			}
		}
	};
	
	public Gui(String name) {
		super(name);
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.setSize(350, 200);
		this.setLocation(500, 200);
		this.setVisible(true);
		
		getContentPane().setLayout(null);
		getContentPane().add(txt1);
		txt1.setBounds(20, 20, 290, 20);
		getContentPane().add(input);
		input.setBounds(20, 50, 290, 20);
		getContentPane().add(button);
		button.setBounds(20, 120, 290, 30);
		button.addActionListener(action);
	}
}
