package day09

import scala.io.Source

sealed trait Direction

case object Up extends Direction

case object Down extends Direction

case object Left extends Direction

case object Right extends Direction

object Direction {
  def fromString(str: String): Option[Direction] = {
    str match {
      case "U" => Some(Up)
      case "D" => Some(Down)
      case "L" => Some(Left)
      case "R" => Some(Right)
      case _ => None
    }
  }
}

final case class Tail(x: Int, y: Int)

final case class Head(x: Int, y: Int)

object Main extends App {
  val data = {
    val source = Source.fromFile("data/09.txt")
    val ret = source.mkString
    source.close()
    ret
  }

  def move(dir: Direction, head: Head): Head = {
    dir match {
      case Up => Head(head.x, head.y + 1)
      case Down => Head(head.x, head.y - 1)
      case Left => Head(head.x - 1, head.y)
      case Right => Head(head.x + 1, head.y)
    }
  }

  def follow(head: Head, tail: Tail): Tail = {
    (head.x - tail.x, head.y - tail.y) match {
      case (x, y) if x.abs < 2 && y.abs < 2 => tail
      case (x, y) if x.abs == 2 || y.abs == 2 => Tail(tail.x + x.sign, tail.y + y.sign)
    }
  }

  val initialHead = Head(0, 0)
  val initialTails = IndexedSeq.fill(9)(Tail(0, 0))
  val parts = data.linesIterator.foldLeft((Set.empty: Set[Tail], Set.empty: Set[Tail]), (initialHead, initialTails)) { case (ht, line) =>
    line.split(" ") match {
      case Array(d, n) =>
        val dir = Direction.fromString(d).get
        (0 until n.toInt).foldLeft(ht) { case (((s1, s2), (h, ts)), _) =>
          val nh = move(dir, h)
          val nts = ts.tail.foldLeft(IndexedSeq(follow(nh, ts.head))) { case (ts, t) =>
            ts.appended(follow(Head(ts.last.x, ts.last.y), t))
          }
          ((s1 + nts(0), s2 + nts(8)), (nh, nts))
        }
    }
  }._1

  println(s"Part 1 solution: ${parts._1.size}")
  println(s"Part 2 solution: ${parts._2.size}")
}
