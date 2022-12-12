package day12

import scala.io.Source

final case class Square(visited: Boolean, key: Char)

object Main extends App {
  val data = {
    val source = Source.fromFile("data/12.txt")
    val ret = source.mkString
    source.close()
    ret
  }

  val squares = data.linesIterator.map { line =>
    line.toCharArray.map { key =>
      val modified_key = if (key == 'E') 'z' + 1 else if (key == 'S') 'a' - 1 else key
      Square(visited = false, modified_key.toChar)
    }
  }.toArray

  val width = squares(0).length
  val height = squares.length

  val origin = (0 until width).flatMap(x => (0 until height).map((x, _))).find(pair => squares(pair._2)(pair._1).key == ('a' - 1)).get
  val target = (0 until width).flatMap(x => (0 until height).map((x, _))).find(pair => squares(pair._2)(pair._1).key == ('z' + 1)).get

  def setVisited(visited: Boolean, pos: (Int, Int)): Unit = {
    squares(pos._2)(pos._1) = Square(visited = visited, squares(pos._2)(pos._1).key)
  }

  setVisited(visited = true, origin)
  val part1 = Iterator.iterate(Seq(List(origin))) { paths =>
    val new_paths = paths.flatMap { path =>
      val curr = path.head
      val neighbours = Seq((curr._1 + 1, curr._2), (curr._1 - 1, curr._2), (curr._1, curr._2 + 1), (curr._1, curr._2 - 1))
        .filter(neighbour => 0 <= neighbour._1 && neighbour._1 < width && 0 <= neighbour._2 && neighbour._2 < height)
        .filter(neighbour => !squares(neighbour._2)(neighbour._1).visited)
        .filter(neighbour => squares(curr._2)(curr._1).key + 1 >= squares(neighbour._2)(neighbour._1).key)
      neighbours.foreach(setVisited(true, _))
      neighbours.map(_::path)
    }
    new_paths
  }.find(_.exists(path => path.head == target)).get.find(path => path.head == target).get.length - 1


  (0 until height).foreach(y => (0 until width).foreach(x => setVisited(visited = false, (x, y))))

  setVisited(visited = true, target)
  val part2 = Iterator.iterate(Seq(List(target))) { paths =>
    val new_paths = paths.flatMap { path =>
      val curr = path.head
      val neighbours = Seq((curr._1 + 1, curr._2), (curr._1 - 1, curr._2), (curr._1, curr._2 + 1), (curr._1, curr._2 - 1))
        .filter(neighbour => 0 <= neighbour._1 && neighbour._1 < width && 0 <= neighbour._2 && neighbour._2 < height)
        .filter(neighbour => !squares(neighbour._2)(neighbour._1).visited)
        .filter(neighbour => squares(neighbour._2)(neighbour._1).key + 1 >= squares(curr._2)(curr._1).key)
      neighbours.foreach(setVisited(true, _))
      neighbours.map(_ :: path)
    }
    new_paths
  }.find(_.exists(path => squares(path.head._2)(path.head._1).key <= 'a')).get.find(path => squares(path.head._2)(path.head._1).key <= 'a').get.length - 1

  println(s"Part 1 solution: $part1")
  println(s"Part 2 solution: $part2")
}
