package day13

import scala.annotation.tailrec
import scala.io.Source

sealed trait Packet {
  def compareTo(anotherPacket: Packet): Int
}

object Packet {
  def parse(str: String): (Packet, String) = {
    str.head match {
      case '[' =>
        val parsed_packets = Iterator.iterate((Seq.empty: Seq[Packet], str.tail)) { case (packets, str) =>
          val parsed_packet = parse(str)
          (packets.appended(parsed_packet._1), parsed_packet._2)
        }.find(_._2.head == ']').get
        (Packets(parsed_packets._1), parsed_packets._2.tail)
      case ',' =>
        parse(str.drop(1))
      case _ => str.span(x => x.isDigit) match {
        case (n, rest) =>
          (Value(n.toInt), rest)
      }
    }
  }

  def fromString(str: String): Packet = {
    parse(str)._1
  }

  implicit def ordering[A <: Packet]: Ordering[A] = (x: A, y: A) => {
    x.compareTo(y)
  }
}

final case class Value(v: Int) extends Packet {
  override def compareTo(anotherPacket: Packet): Int = anotherPacket match {
    case Value(another_v) => v.compareTo(another_v)
    case Packets(_) => Packets(Seq(Value(v))).compareTo(anotherPacket)
  }

  override def toString: String = s"$v"
}

final case class Packets(vs: Seq[Packet]) extends Packet {
  @tailrec
  override def compareTo(anotherPacket: Packet): Int = anotherPacket match {
    case Value(_) => this.compareTo(Packets(Seq(anotherPacket)))
    case Packets(anotherVs) =>
      vs.zip(anotherVs).map(pair => pair._1.compareTo(pair._2)).find(_ != 0).getOrElse(vs.length.compareTo(anotherVs.length))
  }

  override def toString: String = vs.mkString("[", ",", "]")
}

object Main extends App {
  val data = {
    val source = Source.fromFile("data/13.txt")
    val ret = source.mkString
    source.close()
    ret
  }

  val packet_pairs = data.split("\n\n").map(_.linesIterator.map(p => Packet.fromString(p)).toSeq).toSeq

  val part1 = packet_pairs.map(pair => pair.head.compareTo(pair(1))).zipWithIndex.filter(_._1 <= 0).map(pair => pair._2 + 1).sum

  val divider_packets = Seq(Packet.fromString("[[2]]"), Packet.fromString("[[6]]"))
  val part2 = (packet_pairs.flatten ++ divider_packets).sorted.zipWithIndex.flatMap {
    case (Packets(Seq(Packets(Seq(Value(2))))), n) => Some(n + 1)
    case (Packets(Seq(Packets(Seq(Value(6))))), n) => Some(n + 1)
    case _ => None
  }.product

  println(s"Part 1 solution: $part1")
  println(s"Part 2 solution: $part2")
}
