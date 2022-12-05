package day05

import scala.io.Source


object Main extends App {
  val data = {
    val source = Source.fromFile("data/05.txt")
    val ret = source.mkString
    source.close()
    ret
  }

  val move: (Int, Seq[Char], Seq[Char]) => (Seq[Char], Seq[Char]) = (n: Int, from: Seq[Char], to: Seq[Char]) => {
    if (n > 0) {
      move(n - 1, from.init, to.appended(from.last))
    } else {
      (from, to)
    }
  }

  val moveP2: (Int, Seq[Char], Seq[Char]) => (Seq[Char], Seq[Char]) = (n: Int, from: Seq[Char], to: Seq[Char]) => {
    (from.take(from.length - n), to ++ from.drop(from.length - n))
  }

  val (part1, part2) = data.split("\n\n") match {
    case Array(sts, moves) =>
      val initialStacks = () => {
        sts.split("\n").map { stack =>
          stack.chars().toArray.map(c => c.toChar).toSeq
        }
      }

      val applyMoves = (mover: (Int, Seq[Char], Seq[Char]) => (Seq[Char], Seq[Char])) => {
        val stacks = initialStacks()
        moves.linesIterator.map(_.split(" ")) foreach {
          case Array(_, n, _, from, _, to) =>
            mover(n.toInt, stacks(from.toInt - 1), stacks(to.toInt - 1)) match {
              case (fromStack, toStack) =>
                stacks(from.toInt - 1) = fromStack
                stacks(to.toInt - 1) = toStack
            }
        }
        stacks.map(stack => stack.last).mkString("")
      }

      (applyMoves(move), applyMoves(moveP2))
  }


  println(s"Part 1 solution: $part1")
  println(s"Part 2 solution: $part2")

}
