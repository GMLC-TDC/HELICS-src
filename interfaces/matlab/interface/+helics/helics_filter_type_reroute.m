function v = helics_filter_type_reroute()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 91);
  end
  v = vInitialized;
end
