function v = HELICS_PROPERTY_INT_MAX_ITERATIONS()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 82);
  end
  v = vInitialized;
end