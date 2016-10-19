defmodule ExFC do
  use Application

  def start(_type, _args) do
    import Supervisor.Spec, warn: false

    configure_pins

    val = ExFC.PruLoader.enable_prus(
      String.to_char_list("text0"),
      String.to_char_list("data0"),
      priv_file("rc_pru1/text.bin") |> String.to_char_list(),
      priv_file("rc_pru1/data.bin") |> String.to_char_list()
    )

    IO.puts "hello #{val}"

    children = [
      worker(Task, [fn -> check_rc_values end], restart: :transient),
      #worker(PiFc.RemoteControl, [], restart: :transient)
    ]

    opts = [strategy: :one_for_one, name: PiFc.Supervisor]
    Supervisor.start_link(children, opts)
  end

  def check_rc_values do
    { ch1, ch2, ch3, ch4 } = ExFC.PruLoader.read_rc_values

    [ch1, ch2, ch3, ch4]
    |> Enum.map(fn(x) -> round(x * 0.005) end)
    |> IO.inspect

    :timer.sleep 300

    check_rc_values
  end

  def priv_file(name) do
    :filename.join(:code.priv_dir(:ex_fc), name)
  end

  def configure_pins do
    "sh #{priv_file("config-pins.sh")}"
    |> String.to_char_list()
    |> :os.cmd()
  end
end
