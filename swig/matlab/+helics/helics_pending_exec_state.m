function v = helics_pending_exec_state()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1329876576);
  end
  v = vInitialized;
end
