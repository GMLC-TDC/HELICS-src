function v = iteration_error()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 14);
  end
  v = vInitialized;
end
