function v = HELICS_PROPERTY_TIME_PERIOD()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 76);
  end
  v = vInitialized;
end
