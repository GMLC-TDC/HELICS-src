function v = helics_iteration_request_no_iteration()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 101);
  end
  v = vInitialized;
end
