function v = HELICS_STATE_PENDING_TIME()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 133);
  end
  v = vInitialized;
end
