function v = HELICS_ITERATION_REQUEST_ITERATE_IF_NEEDED()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 121);
  end
  v = vInitialized;
end
