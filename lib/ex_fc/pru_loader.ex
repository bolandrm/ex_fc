defmodule ExFC.PruLoader do
  @on_load :load_nifs

  def load_nifs do
    :erlang.load_nif('./priv/pru_loader', 0)
  end

  def enable_prus(text1, data1, text2, data2) do
    raise "NIF enable_prus/4 not implemented"
  end
end
