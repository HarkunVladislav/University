import java.util
import java.util.concurrent.atomic.{AtomicBoolean, AtomicInteger}

import scala.collection.Iterator


class Maps {
  def deleteSameElements(map:Map[Int,Int]): Map[Int,Int] ={
    val keys = new util.ArrayList[Int]()
    val curElement = new AtomicInteger(1)
    val iter = new AtomicInteger(1)
    while (curElement.intValue() < map.size) {
      iter.set(curElement.intValue()+1)
      while (iter.intValue() <= map.size) {
        if(map(curElement.intValue()) == map(iter.intValue()))
          keys.add(iter.intValue())
        iter.incrementAndGet()
      }
      curElement.incrementAndGet()
    }
    del(map,keys,keys.listIterator())
  }
  def del(map:Map[Int,Int], list:util.ArrayList[Int],iterat: util.ListIterator[Int]): Map[Int,Int] = {
    if(!iterat.hasNext) {
      map
    } else {
      del(map - iterat.next(),list,iterat)
    }
  }
}