package day24

import scala.annotation.tailrec
import scala.collection.mutable
import scala.io.Source

sealed trait Blizzard {
  def toVec2: Vec2 = this match {
    case Blizzard.Left => Vec2(-1, 0)
    case Blizzard.Right => Vec2(1, 0)
    case Blizzard.Down => Vec2(0, 1)
    case Blizzard.Up => Vec2(0, -1)
  }

  override def toString: String = this match {
    case Blizzard.Left => "<"
    case Blizzard.Right => ">"
    case Blizzard.Down => "v"
    case Blizzard.Up => "^"
  }
}

object Blizzard {
  object Left extends Blizzard

  object Right extends Blizzard

  object Down extends Blizzard

  object Up extends Blizzard
}

case class Vec2(x: Int, y: Int) {
  def +(vec2: Vec2): Vec2 = Vec2(x + vec2.x, y + vec2.y)

  def %(vec2: Vec2): Vec2 = Vec2(x % vec2.x, y % vec2.y)

  def neighbours: Set[Vec2] = Set(Vec2(x - 1, y), Vec2(x + 1, y), Vec2(x, y - 1), Vec2(x, y + 1))
}


case class Valley(valley: IndexedSeq[IndexedSeq[Seq[Blizzard]]]) {
  val width = valley(0).length
  val height = valley.length

  def outOfBounds(vec2: Vec2) = vec2.x < 0 || vec2.x >= width || vec2.y < 0 || vec2.y >= height

  def update: Valley = {
    val newValley = valley.map(_.toArray).toArray
    for (y <- 0 until height; x <- 0 until width) {
      valley(y)(x).foreach { blizz =>
        val newPos = (Vec2(x, y) + blizz.toVec2 + Vec2(width, height)) % Vec2(width, height)
        newValley(y)(x) = newValley(y)(x).span(_ != blizz) match {
          case (l, r) => l ++ r.tail
        }
        newValley(newPos.y)(newPos.x) = newValley(newPos.y)(newPos.x) :+ blizz
      }
    }
    Valley(newValley.map(_.toIndexedSeq).toIndexedSeq)
  }

  def validPos(pos: Vec2): Boolean = {
    pos == start || pos == end || (!outOfBounds(pos) && valley(pos.y)(pos.x).isEmpty)
  }

  val start = Vec2(0, -1)
  val end = Vec2(width - 1, height)
}

case class State(valley: Valley, possiblePositions: Set[Vec2], mins: Int) {
  def update: State = {
    val newValley = valley.update
    val newPositions = possiblePositions.flatMap { pos =>
      pos.neighbours + pos
    }.filter(newValley.validPos)
    State(valley.update, newPositions, mins + 1)
  }

  def filterInEnd: State = State(valley, possiblePositions.filter(_ == valley.end), mins)

  def filterInStart: State = State(valley, possiblePositions.filter(_ == valley.start), mins)

  override def toString: String = {
    val sb = new mutable.StringBuilder()
    for (y <- -1 to valley.height) {
      for (x <- -1 to valley.width) {
        val char: Char =
          if (possiblePositions.contains(Vec2(x, y))) 'E'
          else if (Vec2(x, y) == valley.start || Vec2(x, y) == valley.end) '.'
          else if (valley.outOfBounds(Vec2(x, y))) '#'
          else valley.valley(y)(x).size match {
            case 0 => '.'
            case 1 => valley.valley(y)(x).head.toString.head
            case n => n.toString.head
          }
        sb.append(char)
      }
      sb.append("\n")
    }
    sb.toString()
  }
}


object Main extends App {

  val data = {
    val source = Source.fromFile("data/24.txt")
    val ret = source.mkString
    source.close()
    ret
  }.linesIterator.map(_.tail.init).filter(!_.contains('#')).map(_.map { c =>
    val blizzards: Seq[Blizzard] = c match {
      case '<' => Seq(Blizzard.Left)
      case '>' => Seq(Blizzard.Right)
      case '^' => Seq(Blizzard.Up)
      case 'v' => Seq(Blizzard.Down)
      case '.' => Seq()
    }
    blizzards
  }.toIndexedSeq).toIndexedSeq

  val initialState = State(Valley(data), Set(Vec2(0, -1)), 0)

  val part1 = Iterator.iterate(initialState)(_.update)
    .find { st => st.possiblePositions.contains(st.valley.end) }.get

  val backState = Iterator.iterate(part1.filterInEnd)(_.update)
    .find { st => st.possiblePositions.contains(st.valley.start) }.get

  val part2 = Iterator.iterate(backState.filterInStart)(_.update)
    .find { st => st.possiblePositions.contains(st.valley.end) }.get

  println(s"Part 1 solution: ${part1.mins}")
  println(s"Part 2 solution: ${part2.mins}")
}
