
object laba6_2 extends App{
    def getSum(list:List[Int]): Int = {
    list.fold(0)(_ + _)
  }
    
        
  val list:List[Int] = List(5,10,4,-3,1,-3)
  
  println(getSum(list))
}