function v = helics_filtertype_delay()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1128095518);
  end
  v = vInitialized;
end
