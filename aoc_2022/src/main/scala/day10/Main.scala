package day10

import scala.io.Source

final case class CPU(x: Int, cycles: Int) {
  def add(v: Int): CPU = {
    CPU(x + v, cycles + 2)
  }

  def noop(): CPU = {
    CPU(x, cycles + 1)
  }
}

object Main extends App {
  val data = {
    val source = Source.fromFile("data/10.txt")
    val ret = source.mkString
    source.close()
    ret
  }

  val cpu = CPU(1, 0)
  val instants = data.linesIterator.map(_.split(" ")).foldLeft((cpu, Seq(cpu))) {
    case ((cpu, signal_strengths), Array("addx", x)) =>
      val new_cpu = cpu.add(x.toInt)
      val new_signal_strengths = signal_strengths.appended(CPU(cpu.x, cpu.cycles + 1)).appended(new_cpu)
      (new_cpu, new_signal_strengths)
    case ((cpu, signal_strengths), Array("noop")) =>
      val new_cpu = cpu.noop()
      val new_signal_strengths = signal_strengths.appended(new_cpu)
      (new_cpu, new_signal_strengths)
  }._2

  val part1 = instants.filter(cpu => (cpu.cycles - 19) % 40 == 0 && cpu.cycles <= 220).map(cpu => cpu.x * (cpu.cycles + 1)).sum


  println(s"Part 1 solution: $part1")

  println(s"Part 2 solution:")
  instants.grouped(40).take(6).foreach { cpus =>
    (0 until 40).foreach { pos =>
      val sprite = cpus(pos).x
      if (sprite - 1 <= pos && pos <= sprite + 1)
        print('#')
      else
        print('.')
    }
    println("")
  }

  println(Option(null))
}
