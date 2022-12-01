package day01

import scala.io.Source
import scala.util.Try
import scala.util.control.Breaks.{break, breakable}

object Main {
  def main(args: Array[String]): Unit = {
    val data = try {
      val source = Source.fromFile("data/01.txt")
      val ret = source.mkString
      source.close()
      ret
    }

    val elf_amount = data.linesIterator.count(l => l.isEmpty)
    val elf_calories = Array.fill(elf_amount + 1)(0)
    var curr_elf = 0
    for (line <- data.linesIterator) {
      breakable {
        if (line.isEmpty) {
          curr_elf += 1
          break
        }
        elf_calories(curr_elf) += Try(line.toInt).getOrElse(0)
      }
    }
    println(s"Part 1 solution: ${elf_calories.max}")

    val elf_calories_sorted = elf_calories.sortBy(x => -x)
    assert(elf_calories_sorted.length > 2)
    println(s"Part 1 solution: ${elf_calories_sorted(0) + elf_calories_sorted(1) + elf_calories_sorted(2)}")
  }
}
