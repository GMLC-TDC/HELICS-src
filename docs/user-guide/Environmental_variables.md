# Environmental variables

The HELICS command line processor has some ability to read and interpret command line variables.  These can assist in setting up a co-simulations.
In general the configuration of HELICS comes from 3 sources during setup.  After setup API's exist for changing the configuration.  The highest priority is given to command line specifications.  The second priority is given to configuration files.  The files can be given through a command string such as `--config=configFile.ini`  The file can be a `.ini, .toml, or .json`.  By default HELICS looks for a `helicsConfig.ini` file.    The lowest priority option is though environmental variables.  Only a subset of controls work with environmental variables.  All Environmental variables used by HELICS, begin with `HELICS_`.  

## Federate Environmental variables

For setting up a federate a few environmental variables are used

-   `HELICS_LOG_LEVEL` : specifying the log level for the federate to use.  Equivalent to `--loglevel=X`
-   `HELICS_CORE_INIT_STRING` :  specifying the init string to pass to the core when creating it.  Equivalent to `--coreinit=X`
-   `HELICS_CORE_TYPE` : the type of core to use e.g. "ZMQ", "TCP", "IPC", "MPI", etc. Equivalent to specifying `--coretype=X`  

## Core and Broker Environmental variables

-   `HELICS_BROKER_LOG_LEVEL` :  specifying the log level for the broker to use.  Equivalent to `--loglevel=X`

-   `HELICS_BROKER_KEY` : A key to use for connecting a core to a broker see [broker key]()
-   `HELICS_BROKER_ADDRESS` : the interface address of the broker.  Equivalent to `--brokeraddress=X`
-   `HELICS_BROKER_PORT` : the port number of the broker.  Equivalent to `--brokerport=X`
-   `HELICS_CONNECTION_PORT` : the port number to use for connecting.  This has different behavior for cores and brokers.  For cores this is the broker port and for brokers this is the local port
-   `HELICS_CONNECTION_ADDRESS` : the interface address to use for connecting.  This has different behavior for cores and brokers.  For cores this is the broker address and for brokers this is the interface.
-   `HELICS_LOCAL_PORT` : the port number to use on the local interface for external connections.  Equivalent to `--localport=X`
-   `HELICS_BROKER_INIT` : the command line arguments to give to an autogenerated broker.  Equivalent to `--brokerinit=X`
-   `HELICS_CORE_TYPE` : the type of core to use e.g. "ZMQ", "TCP", "IPC", "MPI", etc. Equivalent to specifying `--coretype=X` 