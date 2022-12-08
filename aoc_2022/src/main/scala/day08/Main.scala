package day08

import scala.io.Source

object Main extends App {
  val data = {
    val source = Source.fromFile("data/08.txt")
    val ret = source.mkString
    source.close()
    ret
  }

  val height_map = data.split("\n").map(_.toCharArray.map(_.toString.toInt))
  val width = height_map.length
  val height = height_map(0).length

  val visible: (Int, Int) => Boolean = (x: Int, y: Int) => {
    val left = (0 until x).forall(height_map(y)(x) > height_map(y)(_))
    lazy val right = (x + 1 until width).forall(height_map(y)(x) > height_map(y)(_))
    lazy val top = (0 until y).forall(height_map(y)(x) > height_map(_)(x))
    lazy val bot = (y + 1 until height).forall(height_map(y)(x) > height_map(_)(x))
    left || right || top || bot
  }

  val part1 = (0 until height).flatMap { y =>
    (0 until width).map { x =>
      visible(x, y)
    }
  }.count(x => x)

  val scenicScore: (Int, Int) => Int = (x: Int, y: Int) => {
    val left = (0 until x).reverse.span(height_map(y)(x) > height_map(y)(_))
    val right = (x + 1 until width).span(height_map(y)(x) > height_map(y)(_))
    val top = (0 until y).reverse.span(height_map(y)(x) > height_map(_)(x))
    val bot = (y + 1 until height).span(height_map(y)(x) > height_map(_)(x))
    Seq(left, right, top, bot)
      .map(r => r._1.length + (if (r._2.nonEmpty) 1 else 0))
      .product
  }

  val part2 = (0 until height).flatMap { y =>
    (0 until width).map { x =>
      scenicScore(x,y)
    }
  }.max

  println(s"Part 1 solution: $part1")
  println(s"Part 2 solution: $part2")
}
