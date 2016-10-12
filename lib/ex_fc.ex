defmodule ExFC do
  use Application

  def start(_type, _args) do
    import Supervisor.Spec, warn: false

    val = ExFC.PruLoader.enable_prus(
      String.to_char_list("priv/text1"),
      String.to_char_list("data1"),
      String.to_char_list("text2"),
      String.to_char_list("data2")
    )

    IO.puts "hello #{val}"

    children = [
      #worker(Task, [fn -> blink_forever(pid) end], restart: :transient),
      #worker(PiFc.RemoteControl, [], restart: :transient)
    ]

    opts = [strategy: :one_for_one, name: PiFc.Supervisor]
    Supervisor.start_link(children, opts)
  end

  #def blink_forever(pid) do
  #  Gpio.write(pid, @gpio_on)
  #  :timer.sleep @blink_duration
  #  Gpio.write(pid, @gpio_off)
  #  :timer.sleep @blink_duration

  #  blink_forever(pid)
  #end
end
