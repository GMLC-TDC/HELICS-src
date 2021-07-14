function v = HELICS_HANDLE_OPTION_SINGLE_CONNECTION_ONLY()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 97);
  end
  v = vInitialized;
end