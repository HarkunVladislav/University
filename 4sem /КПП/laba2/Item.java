package laba2;

public class Item {
	protected String title;
	protected String quantity;
	protected Food food;
	Item()
	{
		title = null;
		quantity = null;
	}
	public Food getFood() {
		return food;
	}
}
