function v = helics_reroute_filter()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1329876583);
  end
  v = vInitialized;
end
