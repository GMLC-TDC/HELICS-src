function v = HELICS_MULTI_INPUT_OR_OPERATION()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 90);
  end
  v = vInitialized;
end
