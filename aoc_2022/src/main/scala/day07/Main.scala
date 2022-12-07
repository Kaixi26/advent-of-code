package day07

import scala.io.Source

sealed trait File

object File {
  def fromString(str: String): Option[File] = {
    val words = str.split(" +")
    words match {
      case Array("dir", name) => Some(Dir(name))
      case Array(size, name) => size.toIntOption.map(RegularFile(name, _))
      case _ => None
    }
  }
}

case class Dir(name: String) extends File

case class RegularFile(name: String, size: BigInt) extends File

sealed trait Command

case class Ls(files: Array[File]) extends Command {
  override def toString: String = s"Ls(${files.mkString("Array(", ", ", ")")})"
}

case class Cd(path: String) extends Command

object Command {
  def fromString(str: String): Option[Command] = {
    val lines = str.split("\n")
    lines.head.dropWhile(_ == ' ').split(" ") match {
      case Array("cd", path) => Some(Cd(path))
      case Array("ls") =>
        val files = lines.tail.map(File.fromString).map(_.get)
        Some(Ls(files))
      case _ => None
    }
  }
}


object Main extends App {
  val data = {
    val source = Source.fromFile("data/07.txt")
    val ret = source.mkString
    source.close()
    ret
  }

  val commands = data.split("\\$").tail.map { cmdAndOutput =>
    Command.fromString(cmdAndOutput).get
  }

  var currentPath = Seq("/")
  var files: Map[Seq[String], BigInt] = Map()
  var dirs: Set[Seq[String]] = Set()
  commands.foreach {
    case Cd(path) =>
      path match {
        case ".." => currentPath = currentPath.init
        case "/" => currentPath = Seq("/")
        case path => currentPath = currentPath.appended(path)
      }
      dirs += currentPath
    case Ls(fs) => fs.foreach {
      case RegularFile(name, size) => files ++= Seq((currentPath.appended(name), size))
      case _ =>
    }
  }

  val sums = dirs.map(path => (path, files.filter(_._1.startsWith(path)).values.sum))
  val part1 = sums.filter(_._2 < 100000).map(_._2).sum

  val required_space = sums.toMap.get(Seq("/")).get - 40000000
  val part2 = sums.toList.sortBy(_._2).dropWhile(_._2 < required_space).head._2

  println(s"Part 1 solution: $part1")
  println(s"Part 2 solution: $part2")
}
