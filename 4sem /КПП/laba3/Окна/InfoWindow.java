package Windows;

import java.awt.GridLayout;

import javax.swing.JFrame;
import javax.swing.JLabel;

public class InfoWindow extends JFrame{
	public InfoWindow(String str1, String str2, String str3, String str4, String str5){
		super("Information");
		this.setLocation(400, 200);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setSize(400, 200);
		
		this.getContentPane().setLayout(new GridLayout(5, 1, 20, 20));
		JLabel txt1 = new JLabel(str1);
		getContentPane().add(txt1);
		JLabel txt2 = new JLabel(str2);
		getContentPane().add(txt2);
		JLabel txt3 = new JLabel(str3);
		getContentPane().add(txt3);
		JLabel txt4 = new JLabel(str4);
		getContentPane().add(txt4);
		JLabel txt5 = new JLabel(str5);
		getContentPane().add(txt5);
	}
}
