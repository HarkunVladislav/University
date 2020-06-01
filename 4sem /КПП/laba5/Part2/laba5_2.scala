object laba5_2 extends App {
  //recursive method
  def findDiff(list: List[Int],first:Int,second:Int): List[Int] ={
    //проверка на окончания элементов в листе
    if(list.isEmpty)
      List()
    else
    //если разность  двух предыдущих меньше текущего элемента
    // то вызываем рекурсию (она будет возвращать список на каждой итерации и если условие соблюдено,то элемент добавится к листу
      if(first-second < list.head)
        (list.head)::findDiff(list.tail,second,list.head)
      else findDiff(list.tail,second,list.head) // иначе не добавится
  }
  @scala.annotation.tailrec
  def findDiff(list: List[Int], first:Int, second:Int, acc:List[Int]):List[Int]={
    // если исхожный лист пуст,возварщаем аккамулятор
    if (list.isEmpty)
      acc
    else
      if(first-second < list.head)
      // аккамулятор накапливает объекты,удовлетворящие условиям
      findDiff(list.tail,second,list.head,list.head::acc)
        else findDiff(list.tail,second,list.head,acc)
  }
  val list=List[Int](3,2,5,6,4,8,9,7,2,4,5,6,8,7)
  println(findDiff(list,0,0,List()))
  println(findDiff(list,0,0).reverse)
}