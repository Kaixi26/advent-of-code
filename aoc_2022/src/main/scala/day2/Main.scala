package day2

import scala.io.Source

sealed trait Choice

case object Rock extends Choice

case object Paper extends Choice

case object Scissors extends Choice

object Choice {
  def fromString(choice: String): Option[Choice] = {
    choice match {
      case "A" | "X" => Some(Rock)
      case "B" | "Y" => Some(Paper)
      case "C" | "Z" => Some(Scissors)
      case _ => None
    }
  }
}

final class Match(left: Choice, right: Choice) {
  def score(): Int = {
    val extra = right match {
      case Rock => 1
      case Paper => 2
      case Scissors => 3
    }
    val points = (left, right) match {
      case (Rock, Paper) | (Paper, Scissors) | (Scissors, Rock) => 6
      case (left, right) => if (left == right) 3 else 0
    }
    points + extra
  }

  override def toString: String = s"${this.getClass.getSimpleName}{left: $left, right: $right}"

}

object Match {
  def fromString(str: String): Option[Match] = {
    val plays = str.split(" +")
    Option.when(plays.length == 2)(
      Choice.fromString(plays(0)).flatMap(l => {
        Choice.fromString(plays(1)).flatMap(r => {
          Some(new Match(l, r))
        })
      })).flatten
  }

  def fromStringPart2(str: String): Option[Match] = {
    val plays = str.split(" +")
    Option.when(plays.length == 2)(
      Choice.fromString(plays(0)).flatMap(l => {
        (l, plays(1)) match {
          case (Rock, "X") | (Paper, "Z") => Some(new Match(l, Scissors))
          case (Paper, "X") | (Scissors, "Z") => Some(new Match(l, Rock))
          case (Scissors, "X") | (Rock, "Z") => Some(new Match(l, Paper))
          case (l, "Y") => Some(new Match(l, l))
          case _ => None
        }
      })).flatten
  }

}

object Main {
  def main(args: Array[String]): Unit = {
    val data = try {
      val source = Source.fromFile("data/02.txt")
      val ret = source.mkString
      source.close()
      ret
    }

    var score = 0
    data.linesIterator.foreach(line => {
      Match.fromString(line).foreach(m => {
        score += m.score()
      })
    })

    println(s"Part 1 solution: $score")

    var score2 = 0
    data.linesIterator.foreach(line => {
      Match.fromStringPart2(line).foreach(m => {
        score2 += m.score()
      })
    })
    println(s"Part 2 solution: $score2")

  }
}
