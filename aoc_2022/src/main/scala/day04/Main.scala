package day04

import scala.io.Source

case class AssignedSection(start: Int, end: Int) {
  def fulContains(assignedSection: AssignedSection): Boolean = {
    start <= assignedSection.start && assignedSection.end <= end
  }

  def overlaps(assignedSection: AssignedSection): Boolean = {
    !(start > assignedSection.end || end < assignedSection.start)
  }
}

object Main extends App {
  val data = {
    val source = Source.fromFile("data/04.txt")
    val ret = source.mkString
    source.close()
    ret
  }

  val assignments = data.linesIterator.map { line =>
    line.split(",")
      .map { assigment =>
        assigment.split("-") match {
          case Array(start, end) => AssignedSection(start.toInt, end.toInt)
        }
      }
  }.toArray

  val fullContains = assignments.count { pair =>
    pair(0).fulContains(pair(1)) || pair(1).fulContains(pair(0))
  }

  println(s"Part 1 solution: $fullContains")

  val overlaps = assignments.count { pair =>
    pair(0).overlaps(pair(1))
  }

  println(s"Part 2 solution: $overlaps")

}
