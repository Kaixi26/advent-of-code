package day21

import scala.annotation.tailrec
import scala.collection.mutable
import scala.io.Source

sealed trait Expr

final case class Bop(l: String, r: String, op: String) extends Expr

final case class Number(v: BigInt) extends Expr

final case class State(monkeys: Map[String, Expr]) {

  @tailrec
  private def evalMemo(exprs: List[String], cache: mutable.Map[String, BigInt]): Unit = {
    exprs.headOption match {
      case Some(key) =>
        monkeys(key) match {
          case Bop(l, r, op) =>
            (cache.get(l), cache.get(r)) match {
              case (Some(l), Some(r)) =>
                val v = op match {
                  case "+" => l + r
                  case "-" => l - r
                  case "*" => l * r
                  case "/" => l / r
                }
                cache.addOne(key, v)
                evalMemo(exprs.tail, cache)
              case _ =>
                evalMemo(l :: r :: exprs, cache)
            }
          case Number(v) =>
            cache.addOne(key, v)
            evalMemo(exprs.tail, cache)
        }
      case None =>
    }
  }

  def eval(str: String): BigInt = {
    val cache: mutable.Map[String, BigInt] = new mutable.HashMap()
    evalMemo(List("root"), cache)
    cache(str)
  }
}

object Main extends App {

  val data = {
    val source = Source.fromFile("data/21.txt")
    val ret = source.mkString
    source.close()
    ret
  }.linesIterator.map(_.split(":")).map { case Array(name, exprStr) =>
    val expr = exprStr.trim.split(" ") match {
      case Array(v) => Number(BigInt(v.toInt))
      case Array(l, op, r) => Bop(l, r, op)
    }
    name -> expr
  }.toMap

  val part1 = State(data).eval("root")

  val roots = data("root") match {
    case Bop(l, r, _) => (l, r)
  }

  /*
    Very cursed but works lol
   */
  @tailrec
  def findStartingPow(epow: Int = 0): Option[Int] = {
    val n = BigInt(10).pow(epow)
    val state = State(data + ("humn" -> Number(n)))
    val diff = state.eval(roots._1) - state.eval(roots._2)
    if (n > BigInt(10000000000000L)) None
    else if (diff < 0) Some(epow - 1)
    else findStartingPow(epow + 1)
  }

  def findFromStartingPow(curr: BigInt, epow: Int): Option[BigInt] = {
    val n = (curr + 1) * BigInt(10).pow(epow)
    val state = State(data + ("humn" -> Number(n)))
    val diff = state.eval(roots._1) - state.eval(roots._2)
    if (n > BigInt(10000000000000L)) None
    else if (diff == 0) Some(n)
    else if (diff < 0) findFromStartingPow((curr - 1) * 10, epow - 1)
    else findFromStartingPow(curr + 1, epow)
  }

  val part2 = findFromStartingPow(1, findStartingPow().get).get

  println(s"Part 1 solution: $part1")
  println(s"Part 2 solution: $part2")
}
