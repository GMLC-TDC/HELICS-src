function v = next_step()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1329876566);
  end
  v = vInitialized;
end
