package day03

import scala.io.Source

object Main {

  def priority(c: Char): Int = {
    if (c.isUpper) {
      c - 'A' + 27
    } else {
      c - 'a' + 1
    }
  }

  def main(args: Array[String]): Unit = {
    val data = try {
      val source = Source.fromFile("data/03.txt")
      val ret = source.mkString
      source.close()
      ret
    }

    var priorities = 0
    data.linesIterator.foreach(line => {
      val commonItems = line.chars().toArray.splitAt(line.length / 2) match {
        case (l, r) => l.toSet.intersect(r.toSet)
      }
      commonItems.foreach(item => priorities += priority(item.toChar))
    })

    println(s"Part 1 solution: $priorities")

    var priorities2 = data.linesIterator
      .map(line => line.chars().toArray.toSet).iterator
      .grouped(3)
      .map(groups => groups.reduce((x, y) => x.intersect(y)))
      .map(common => priority(common.head.toChar)).sum

    println(s"Part 2 solution: $priorities2")

  }
}