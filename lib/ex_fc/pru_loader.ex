defmodule ExFC.PruLoader do
  @on_load :load_nifs

  def load_nifs do
    path = :filename.join(:code.priv_dir(:ex_fc), 'pru_loader')
    :ok = :erlang.load_nif(path, 0)
  end

  def enable_prus(text1, data1, text2, data2) do
    raise "NIF enable_prus/4 not implemented"
  end
end
