function v = HELICS_DATA_TYPE_ANY()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 26);
  end
  v = vInitialized;
end
