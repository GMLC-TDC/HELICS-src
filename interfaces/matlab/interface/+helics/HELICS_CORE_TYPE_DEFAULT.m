function v = HELICS_CORE_TYPE_DEFAULT()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 0);
  end
  v = vInitialized;
end
