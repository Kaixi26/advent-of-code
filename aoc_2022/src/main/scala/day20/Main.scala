package day20

import java.util.UUID
import scala.io.Source

private object MixEntry {
  def apply(v: BigInt): MixEntry = {
    MixEntry(v, java.util.UUID.randomUUID)
  }
}

final case class MixEntry(v: BigInt, uuid: UUID) {
  override def toString: String = v.toString
}

final case class Mixing(entries: IndexedSeq[MixEntry]) {
  def move(entry: MixEntry): Mixing = {
    val index = entries.indexOf(entry)
    val newIndex = {
      val x = ((index + entry.v) % (entries.length - 1)).toInt
      if (x < 0) entries.length + x - 1 else x
    }

    if (newIndex > index)
      Mixing(entries.patch(index, Nil, 1).patch(newIndex, Seq(entry), 0))
    else
      Mixing(entries.patch(newIndex, Seq(entry), 0).patch(index + 1, Nil, 1))
  }

  def groveSum: BigInt = {
    val zeroIndex = entries.indexWhere(_.v == 0)
    Seq(1000, 2000, 3000)
      .map(dx => (zeroIndex + dx) % entries.length)
      .map(entries(_))
      .map(_.v)
      .sum
  }

  override def toString: String = entries.mkString(", ")
}

object Main extends App {

  val data = {
    val source = Source.fromFile("data/20.txt")
    val ret = source.mkString
    source.close()
    ret
  }.linesIterator.toIndexedSeq

  private val initialMixing = Mixing(data.map(line => MixEntry(line.toInt)))
  private val finalMixingP1 = initialMixing.entries.foldLeft(initialMixing) { (mixing, entry) =>
    mixing.move(entry)
  }

  private val initialMixingP2 = Mixing(data.map(line => MixEntry(BigInt(line.toInt) * 811589153)))
  private val finalMixingP2 = (1 to 10).foldLeft(initialMixingP2) { (mixing, n) =>
    initialMixingP2.entries.foldLeft(mixing) { (mixing, entry) =>
      mixing.move(entry)
    }
  }

  println(s"Part 1 solution: ${finalMixingP1.groveSum}")
  println(s"Part 2 solution: ${finalMixingP2.groveSum}")
}
