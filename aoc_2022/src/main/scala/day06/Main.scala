package day06

import scala.io.Source


object Main extends App {
  val data = {
    val source = Source.fromFile("data/06.txt")
    val ret = source.mkString
    source.close()
    ret
  }

  val chrs = data.linesIterator.next().toCharArray
  val part1 = (0 until chrs.length - 4).indexWhere(i => chrs.slice(i, i+4).toSet.size == 4) + 4
  val part2 = (0 until chrs.length - 14).indexWhere(i => chrs.slice(i, i+14).toSet.size == 14) + 14

  println(s"Part 1 solution: $part1")
  println(s"Part 2 solution: $part2")

}
