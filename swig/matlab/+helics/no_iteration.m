function v = no_iteration()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1432107597);
  end
  v = vInitialized;
end
