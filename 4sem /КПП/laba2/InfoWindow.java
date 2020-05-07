package laba2;

import java.awt.GridLayout;

import javax.swing.JFrame;
import javax.swing.JLabel;

public class InfoWindow extends JFrame{
	InfoWindow(String str1, String str2, String str3, String str4){
		super("Information");
		this.setLocation(500, 200);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setSize(300, 150);
		
		this.getContentPane().setLayout(new GridLayout(4, 1, 20, 20));
		JLabel txt1 = new JLabel(str1);
		getContentPane().add(txt1);
		JLabel txt2 = new JLabel(str2);
		getContentPane().add(txt2);
		JLabel txt3 = new JLabel(str3);
		getContentPane().add(txt3);
		JLabel txt4 = new JLabel(str4);
		getContentPane().add(txt4);
	}
}
