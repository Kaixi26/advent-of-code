package day11

import scala.io.Source

final case class Monkey(items: Seq[Int], inspections: Int, op: Int => Int, test: Int => Int) {

  def appended(elem: Int): Monkey = {
    Monkey(items.appended(elem), inspections, op, test)
  }

  def empty(): Monkey = {
    Monkey(Seq.empty, inspections, op, test)
  }

  def inspect(amount: Int): Monkey = {
    Monkey(items, inspections + amount, op, test)
  }

  override def toString: String = s"Monkey($items, $inspections)"
}

object Monkey {
  def fromString(str: String): Monkey = {
    str.split("\n").take(6) match {
      case Array(_, starting_items, operation, testdiv, ifftrue, iffalse) =>
        val items = starting_items.drop(18).split(", ").map(_.toInt)
        /*
        exercise:
        > product [5, 2, 19, 7, 17, 13, 3, 11] = 9699690
        example:
        > product [23, 19, 13, 17] = 96577

        $ factor 936766961130
        936766961130: 2 3 5 7 11 13 13 17 17 19 19 23
        product [2, 3, 5, 7, 11, 13, 17, 19, 23] = 223092870
         */
        val op: Int => Int = operation.drop(23).split(" ") match {
          case Array("+", n) => x =>
            ((BigInt(x) + (if (n == "old") x else n.toInt)) % 223092870).toInt
          case Array("*", n) => x =>
            ((BigInt(x) * (if (n == "old") x else n.toInt)) % 223092870).toInt
        }
        val test: Int => Int = (testdiv.drop(21), ifftrue.drop(29), iffalse.drop(30)) match {
          case (ndiv, ntrue, nfalse) => x => if (x % ndiv.toInt == 0) ntrue.toInt else nfalse.toInt
        }
        Monkey(items, 0, op, test)

    }
  }
}

object Main extends App {
  val data = {
    val source = Source.fromFile("data/11.txt")
    val ret = source.mkString
    source.close()
    ret
  }

  val monkeys = data.split("\n\n").map(Monkey.fromString).toIndexedSeq

  def simulateNRounds(monkeys: IndexedSeq[Monkey], rounds: Int, divisor: Int): IndexedSeq[Monkey] = {
    (0 until rounds).foldLeft(monkeys) { case (mks, _) =>
      val monkeys = mks.toArray
      monkeys.indices.foreach { i =>
        val monkey = monkeys(i)
        monkeys(i) = monkey.empty().inspect(monkey.items.length)
        monkey.items.foreach { item =>
          val worry = monkey.op(item) / divisor
          monkeys(monkey.test(worry)) = monkeys(monkey.test(worry)).appended(worry)
        }
      }
      monkeys.toIndexedSeq
    }
  }


  val part1 = simulateNRounds(monkeys, 20, 3)
    .sortBy(-_.inspections).take(2).map(_.inspections).product

  val part2 = simulateNRounds(monkeys, 10000, 1)
    .sortBy(-_.inspections).take(2).map(monkey => BigInt(monkey.inspections)).product

  println(s"Part 1 solution: $part1")
  println(s"Part 2 solution: $part2")
}
