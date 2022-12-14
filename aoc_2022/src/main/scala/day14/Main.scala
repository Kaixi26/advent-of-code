package day14

import scala.annotation.tailrec
import scala.io.Source

sealed trait Tile
case object Air extends Tile {
  override def toString: String = "."
}

case object Rock extends Tile {
  override def toString: String = "#"
}

case object Sand extends Tile {
  override def toString: String = "o"
}


object Main extends App {
  type Board = Map[(Int, Int), Tile]

  val data = {
    val source = Source.fromFile("data/14.txt")
    val ret = source.mkString
    source.close()
    ret
  }


  val board = data.linesIterator.foldLeft(Map.empty: Board) { (board, line) =>
    val paths = line.split(" -> ").map(coord => {
      val coord_xy = coord.split(",")
      (coord_xy(0).toInt, coord_xy(1).toInt)
    })
    (0 until paths.length - 1).foldLeft(board) { (board, i) =>
      val c1 = paths(i)
      val c2 = paths(i + 1)
      if (c1._1 == c2._1) {
        (Math.min(c1._2, c2._2) to Math.max(c1._2, c2._2)).foldLeft(board) { (board, y) =>
          board + (((c1._1, y), Rock))
        }
      } else {
        (Math.min(c1._1, c2._1) to Math.max(c1._1, c2._1)).foldLeft(board) { (board, x) =>
          board + (((x, c1._2), Rock))
        }
      }
    }
    //println(paths.mkString("Array(", ", ", ")"))
  }

  @tailrec
  def drop_sand(board: Board, coords: (Int, Int)): Option[Board] = {
    if (coords._2 < 1000) {
        board.getOrElse((coords._1, coords._2 + 1), Air) match {
          case Sand | Rock =>
            val left = board.getOrElse((coords._1 - 1, coords._2), Air)
            val left_down = board.getOrElse((coords._1 - 1, coords._2 + 1), Air)
            val right = board.getOrElse((coords._1 + 1, coords._2), Air)
            val right_down = board.getOrElse((coords._1 + 1, coords._2 + 1), Air)
            (left, left_down, right, right_down) match {
              case (Air, Air, _, _) | (Rock, Air, _, _) => drop_sand(board, (coords._1 - 1, coords._2 + 1))
              case (_, _, Air, Air) | (_, _, Rock, Air) => drop_sand(board, (coords._1 + 1, coords._2 + 1))
              case (_, _, _, _) => Some(board + ((coords, Sand)))
            }
          case Air => drop_sand(board, (coords._1, coords._2 + 1))
      }
    } else None
  }

  val final_board_part1 = Iterator.iterate(Option(board)) { board =>
    board.flatMap(drop_sand(_, (500, 0)))
  }.takeWhile(_.nonEmpty).toSeq.zipWithIndex.last

  val bottom_y = board.map(_._1._2).max + 2
  val init_board_part2 = (0 until 1000).foldLeft(board) { (board, x) =>
    board + (((x, bottom_y), Rock))
  }

  val final_board_part2 = Iterator.iterate(Option(init_board_part2)) { board =>
    board.flatMap(drop_sand(_, (500, 0)))
  }.takeWhile(board => board.nonEmpty && !board.get.contains((500, 0))).toSeq.zipWithIndex.last

  (0 until bottom_y).foreach { y =>
    (300 until 600).foreach { x =>
      print(final_board_part2._1.get.getOrElse((x, y), Air))
    }
    println("")
  }

  println(s"Part 1 solution: ${final_board_part1._2}")
  println(s"Part 2 solution: ${final_board_part2._2 + 1}")
}
