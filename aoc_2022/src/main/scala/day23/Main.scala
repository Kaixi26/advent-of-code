package day23

import day23.Elf.considerations

import scala.annotation.tailrec
import scala.collection.immutable.HashSet
import scala.collection.{immutable, mutable}
import scala.io.Source

object Vec2 {
  def squareArea(v1: Vec2, v2: Vec2): Int = {
    (v1.x - v2.x).abs * (v1.y - v2.y).abs
  }
}

case class Vec2(x: Int, y: Int) {
  def +(vec2: Vec2): Vec2 = Vec2(x + vec2.x, y + vec2.y)

  def %(vec2: Vec2): Vec2 = Vec2(x % vec2.x, y % vec2.y)

  def neighbours: Set[Vec2] = {
    for {
      dx <- Set(-1, 0, 1)
      dy <- Set(-1, 0, 1)
      if dx != 0 || dy != 0
    } yield Vec2(x + dx, y + dy)
  }
}

object Elf {
  private val considerationsSeq = IndexedSeq(
    (elf: Elf, elves: Set[Vec2]) => if ((-1 to 1).forall(dx => !elves.contains(elf.pos + Vec2(dx, -1)))) Some(elf.pos + Vec2(0, -1)) else None,
    (elf: Elf, elves: Set[Vec2]) => if ((-1 to 1).forall(dx => !elves.contains(elf.pos + Vec2(dx, 1)))) Some(elf.pos + Vec2(0, 1)) else None,
    (elf: Elf, elves: Set[Vec2]) => if ((-1 to 1).forall(dy => !elves.contains(elf.pos + Vec2(-1, dy)))) Some(elf.pos + Vec2(-1, 0)) else None,
    (elf: Elf, elves: Set[Vec2]) => if ((-1 to 1).forall(dy => !elves.contains(elf.pos + Vec2(1, dy)))) Some(elf.pos + Vec2(1, 0)) else None
  )

  def considerations(n: Int): (Elf, Set[Vec2]) => Option[Vec2] = considerationsSeq(n % considerationsSeq.length)
}

final case class Elf(pos: Vec2, n: Int) {
  def propose(elvesPositions: Set[Vec2]): Option[Vec2] = {
    if (pos.neighbours.forall(n => !elvesPositions.contains(n))) {
      None
    } else {
      Elf.considerations(n)(this, elvesPositions)
        .orElse(Elf.considerations(n + 1)(this, elvesPositions))
        .orElse(Elf.considerations(n + 2)(this, elvesPositions))
        .orElse(Elf.considerations(n + 3)(this, elvesPositions))
    }
  }

  def stay: Elf = Elf(pos, n + 1)

  def move(vec2: Vec2): Elf = Elf(vec2, n + 1)
}

final case class State(elves: Set[Elf], round: Int) {
  lazy val min: Vec2 = Vec2(elves.map(_.pos.x).min, elves.map(_.pos.y).min)
  lazy val max: Vec2 = Vec2(elves.map(_.pos.x).max, elves.map(_.pos.y).max)
  lazy val elvesPositions: Set[Vec2] = elves.map(_.pos)

  def update: State = {
    val proposals = elves.flatMap { elf => elf.propose(elvesPositions).map(elf -> _) }.toMap
    val elfsByProposal = proposals.foldLeft(Map.empty[Vec2, Set[Elf]]) { case (acc, prop) =>
      acc.updatedWith(prop._2) {
        case Some(props) => Some(props + prop._1)
        case None => Some(Set(prop._1))
      }
    }

    State(elves.map { elf =>
      proposals.get(elf) match {
        case Some(p) => if (elfsByProposal(p).size == 1) elf.move(p) else elf.stay
        case None => elf.stay
      }
    }, round + 1)
  }

  override def toString: String = {
    val sb = new mutable.StringBuilder()
    sb.append(s"Round $round:\n")
    for (y <- min.y to max.y) {
      for (x <- min.x to max.x) {
        val char =
          if (elvesPositions.contains(Vec2(x, y))) '#'
          else '.'
        sb.append(char)
      }
      sb.append("\n")
    }
    sb.toString()
  }
}

object Main extends App {

  val data = {
    val source = Source.fromFile("data/23.txt")
    val ret = source.mkString
    source.close()
    ret
  }.linesIterator.map(_.toArray).toArray

  private val initialElves: mutable.Set[Elf] = mutable.HashSet.empty
  for (y <- data.indices; x <- data(0).indices) {
    if (data(y)(x) == '#') {
      initialElves.add(Elf(Vec2(x, y), 0))
    }
  }

  val initialState = State(HashSet(initialElves.toSeq: _*), 0)

  val round10 = Iterator.iterate(initialState)(_.update)
    .take(11)
    .toIndexedSeq(10)

  val part1 = Vec2.squareArea(round10.min, round10.max + Vec2(1, 1)) - round10.elves.size

  val part2 = {
    var prev: Set[Vec2] = Set.empty
    Iterator.iterate(initialState)(_.update)
      .takeWhile { st =>
        if (st.elvesPositions == prev) {
          false
        } else {
          prev = st.elvesPositions
          true
        }
      }
  }.toSeq.size

  println(s"Part 1 solution: $part1")
  println(s"Part 2 solution: $part2")
}
