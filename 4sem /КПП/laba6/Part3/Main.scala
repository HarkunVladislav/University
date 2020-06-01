

object Main {
    def main(args:Array[String]): Unit = {
    val map = Map(1 -> -2, 2 -> 4, 3 -> -2,4 -> 7,5 -> -2,6 -> -2, 7 -> 4)

    val newMap=new Maps().deleteSameElements(map)
    val iter= newMap.iterator
    while(iter.hasNext) {
      print(iter.next())
      print(' ')
    }
  }
}