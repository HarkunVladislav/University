
object laba5_1 extends App {
  var string: String = "cHeCk"
  var len = string.length();
// без рекурсии
var newString = for (c <- string) yield c.toUpper

// с рекурсией
def uppercase(string: String) : String = {

def iterate(string: String, i: Int = 0, result: String = "") : String = {
if (i != len) {
var c: Char = string.charAt(i).toUpper
iterate(string, i + 1, result) + c
}
}

iterate(string).reverse
}
}