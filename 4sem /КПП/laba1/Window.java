package laba1;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;

public class Window extends JFrame {
	int number = 0;
	private JButton button1 = new JButton("Генерировать больше");
	private JButton button2 = new JButton("Генерировать меньше");
	
	private JTextField input = new JTextField(null);
	
	JLabel more = new JLabel();
	JLabel less = new JLabel();
	
	int rMore = 0, rLess = 0;
	
	public class Button1ActionListener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			revalidate();
			repaint();
				
			try {
				more.setText(null);
					
				number = Integer.parseInt(input.getText());
					
				do {
					rMore = (int)(Math.random()*(100001))-50000;
				}while(rMore <= number);
				
				more.setText(String.valueOf(rMore));
				getContentPane().add(more);
				more.setBounds(230, 60, 60, 30);
			}
			catch(NumberFormatException exception) {
				more.setText("-");
					
				input.setText("null");
				number = 0;
					
				more.setText(Integer.toString(rMore));
				getContentPane().add(more);
				more.setBounds(230, 60, 60, 30);
			}
		}
	}
	public class Button2ActionListener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {	
			revalidate();
			repaint();
			
			try {
				less.setText(null);
				
				number = Integer.parseInt(input.getText());

				do {
					rLess = (int)(Math.random()*(100001))-50000;
				}while(rLess >= number);
				
				less.setText(Integer.toString(rLess));
				getContentPane().add(less);
				less.setBounds(230, 105, 60, 30);
			}
			catch(NumberFormatException exception) {
				less.setText("-");
				
				input.setText("null");
				number = 0;

				getContentPane().add(less);
				less.setBounds(230, 105, 60, 30);
			}
		}
	};

	Window(String Name){
	super(Name);
	
	setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	setSize(400,200);
	setLocation(465, 810);
	this.getContentPane().setLayout(null);
	
	button1.addActionListener(new Button1ActionListener());
	button2.addActionListener(new Button2ActionListener());
	
	getContentPane().add(input);
	input.setBounds(10, 15, 200, 30);
	getContentPane().add(button1);
	button1.setBounds(10, 60, 200, 30);
	getContentPane().add(button2);
	button2.setBounds(10, 105, 200, 30);
	}
}
