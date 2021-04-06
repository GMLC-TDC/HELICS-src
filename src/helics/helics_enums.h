/*
Copyright (c) 2017-2020,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

/*defines left in this code as it is used in the shared library*/
#ifndef _HELICS_ENUMS_
#define _HELICS_ENUMS_
#pragma once

/** @file
@brief base helics enumerations for C and C++ API's
*/

#ifdef __cplusplus
extern "C" {
#endif

/** pick a core type depending on compile configuration usually either ZMQ if available or TCP */
typedef enum {
    /** a default core type that will default to something available*/
    HELICS_CORE_TYPE_DEFAULT = 0,
    /** use the Zero MQ networking protocol */
    HELICS_CORE_TYPE_ZMQ = 1,
    /** use MPI for operation on a parallel cluster */
    HELICS_CORE_TYPE_MPI = 2,
    /** use the Test core if all federates are in the same process */
    HELICS_CORE_TYPE_TEST = 3,
    /** interprocess uses memory mapped files to transfer data (for use when all federates are
        on the same machine */
    HELICS_CORE_TYPE_INTERPROCESS = 4,
    /** interprocess uses memory mapped files to transfer data (for use when all federates are
        on the same machine ipc is the same as /ref HELICS_CORE_TYPE_interprocess*/
    HELICS_CORE_TYPE_IPC = 5,
    /** use a generic TCP protocol message stream to send messages */
    HELICS_CORE_TYPE_TCP = 6,
    /** use UDP packets to send the data */
    HELICS_CORE_TYPE_UDP = 7,
    /** single socket version of ZMQ core usually for high fed count on the same system*/
    HELICS_CORE_TYPE_ZMQ_SS = 10,
    /** for using the nanomsg communications */
    HELICS_CORE_TYPE_NNG = 9,
    /** a single socket version of the TCP core for more easily handling firewalls*/
    HELICS_CORE_TYPE_TCP_SS = 11,
    /** a core type using http for communication*/
    HELICS_CORE_TYPE_HTTP = 12,
    /** a core using websockets for communication*/
    HELICS_CORE_TYPE_WEBSOCKET = 14,
    /** an in process core type for handling communications in shared
                                     memory it is pretty similar to the test core but stripped from
                                     the "test" components*/
    HELICS_CORE_TYPE_INPROC = 18,
    /** an explicit core type that is recognized but explicitly doesn't
                                  exist, for testing and a few other assorted reasons*/
    HELICS_CORE_TYPE_NULL = 66
} HelicsCoreTypes;

/** enumeration of allowable data types for publications and inputs*/
typedef enum {
    /** a sequence of characters*/
    HELICS_DATA_TYPE_STRING = 0,
    /** a double precision floating point number*/
    HELICS_DATA_TYPE_DOUBLE = 1,
    /** a 64 bit integer*/
    HELICS_DATA_TYPE_INT = 2,
    /** a pair of doubles representing a complex number*/
    HELICS_DATA_TYPE_COMPLEX = 3,
    /** an array of doubles*/
    HELICS_DATA_TYPE_VECTOR = 4,
    /** a complex vector object*/
    HELICS_DATA_TYPE_COMPLEX_VECTOR = 5,
    /** a named point consisting of a string and a double*/
    HELICS_DATA_TYPE_NAMED_POINT = 6,
    /** a boolean data type*/
    HELICS_DATA_TYPE_BOOLEAN = 7,
    /** time data type*/
    HELICS_DATA_TYPE_TIME = 8,
    /** raw data type*/
    HELICS_DATA_TYPE_RAW = 25,
    /** the data type can change*/
    HELICS_DATA_TYPE_MULTI = 33,
    /** open type that can be anything*/
    HELICS_DATA_TYPE_ANY = 25262
} HelicsDataTypes;

/** single character data type  this is intentionally the same as string*/
#define HELICS_DATA_TYPE_CHAR HELICS_DATA_TYPE_STRING

/** enumeration of possible federate flags*/
typedef enum {
    /** flag indicating that a federate is observe only*/
    HELICS_FLAG_OBSERVER = 0,
    /** flag indicating that a federate can only return requested times*/
    HELICS_FLAG_UNINTERRUPTIBLE = 1,
    /** flag indicating that a federate can be interrupted*/
    HELICS_FLAG_INTERRUPTIBLE = 2,
    /** flag indicating that a federate/interface is a signal generator only*/
    HELICS_FLAG_SOURCE_ONLY = 4,
    /** flag indicating a federate/interface should only transmit values if they have changed(binary
           equivalence)*/
    HELICS_FLAG_ONLY_TRANSMIT_ON_CHANGE = 6,
    /** flag indicating a federate/interface should only trigger an update if a value has changed
     * (binary equivalence)*/
    HELICS_FLAG_ONLY_UPDATE_ON_CHANGE = 8,
    /** flag indicating a federate should only grant time if all other federates have already passed
     * the requested time*/
    HELICS_FLAG_WAIT_FOR_CURRENT_TIME_UPDATE = 10,
    /** flag indicating a federate should operate on a restrictive time policy, which disallows some
       2nd order time evaluation and can be useful for certain types of dependency cycles
        and update patterns, but generally shouldn't be used as it can lead to some very slow update
       conditions*/
    HELICS_FLAG_RESTRICTIVE_TIME_POLICY = 11,
    /** flag indicating that a federate has rollback capability*/
    HELICS_FLAG_ROLLBACK = 12,
    /** flag indicating that a federate performs forward computation and does internal rollback*/
    HELICS_FLAG_FORWARD_COMPUTE = 14,
    /** flag indicating that a federate needs to run in real time*/
    HELICS_FLAG_REALTIME = 16,
    /** flag indicating that the federate will only interact on a single thread*/
    HELICS_FLAG_SINGLE_THREAD_FEDERATE = 27,
    /** used to not display warnings on mismatched requested times*/
    HELICS_FLAG_IGNORE_TIME_MISMATCH_WARNINGS = 67,
    /** specify that checking on configuration files should be strict and throw and error on any
   invalid values */
    HELICS_FLAG_STRICT_CONFIG_CHECKING = 75,
    /** specify that the federate is event triggered-meaning (all/most) events are triggered by
       incoming events*/
    HELICS_FLAG_EVENT_TRIGGERED = 81
} HelicsFederateFlags;
    
/** enumeration of additional core flags*/
typedef enum {
    /** used to delay a core from entering initialization mode even if it would otherwise be ready*/
    HELICS_FLAG_DELAY_INIT_ENTRY = 45,
    /** used to clear the HELICS_DELAY_INIT_ENTRY flag in cores*/
    HELICS_FLAG_ENABLE_INIT_ENTRY = 47
} HelicsCoreFlags;

/** enumeration of general flags that can be used in federates/cores/brokers */
typedef enum {
    /** flag specifying that a federate, core, or broker may be slow to respond to pings
        If the federate goes offline there is no good way to detect it so use with caution
        */
    HELICS_FLAG_SLOW_RESPONDING = 29,
    /** flag specifying the federate/core/broker is operating in a user debug mode so deadlock
    timers and timeout are disabled this flag is a combination of slow_responding and disabling of
    some timeouts*/
    HELICS_FLAG_DEBUGGING = 31,
    /** specify that a federate error should terminate the federation*/
    HELICS_FLAG_TERMINATE_ON_ERROR = 72,
    /** specify that the log files should be flushed on every log message*/
    HELICS_FLAG_FORCE_LOGGING_FLUSH = 88,
    /** specify that a full log should be dumped into a file*/
    HELICS_FLAG_DUMPLOG = 89
} HelicsFlags;

/** log level definitions
 */
typedef enum {
    /** don't print anything except a few catastrophic errors*/
    HELICS_LOG_LEVEL_NO_PRINT = -1,
    /** only print error level indicators*/
    HELICS_LOG_LEVEL_ERROR = 0,
    /** only print warnings and errors*/
    HELICS_LOG_LEVEL_WARNING = 1,
    /** warning errors and summary level information*/
    HELICS_LOG_LEVEL_SUMMARY = 2,
    /** summary+ notices about federate and broker connections +messages about network
       connections*/
    HELICS_LOG_LEVEL_CONNECTIONS = 3,
    /** connections+ interface definitions*/
    HELICS_LOG_LEVEL_INTERFACES = 4,
    /** interfaces + timing message*/
    HELICS_LOG_LEVEL_TIMING = 5,
    /** timing+ data transfer notices*/
    HELICS_LOG_LEVEL_DATA = 6,
    /** all internal messages*/
    HELICS_LOG_LEVEL_TRACE = 7
} HelicsLogLevels;

/** enumeration of return values from the C interface functions
 */
typedef enum {
    /** global fatal error for federation */
    HELICS_ERROR_FATAL = -404,
    /** an unknown non-helics error was produced */
    HELICS_ERROR_EXTERNAL_TYPE = -203,
    /** the function produced a helics error of some other type */
    HELICS_ERROR_OTHER = -101,
    /** insufficient space is available to store requested data */
    HELICS_ERROR_INSUFFICIENT_SPACE = -18,
    HELICS_ERROR_EXECUTION_FAILURE = -14, /*!< the function execution has failed */
    /** the call made was invalid in the present state of the calling object */
    HELICS_ERROR_INVALID_FUNCTION_CALL = -10,
    /** error issued when an invalid state transition occurred */
    HELICS_ERROR_INVALID_STATE_TRANSITION = -9,
    /** the function issued a warning of some kind */
    HELICS_WARNING = -8,
    /** the federate has terminated unexpectedly and the call cannot be completed */
    HELICS_ERROR_SYSTEM_FAILURE = -6,
    /** the input was discarded and not used for some reason */
    HELICS_ERROR_DISCARD = -5,
    /** the parameter passed was invalid and unable to be used */
    HELICS_ERROR_INVALID_ARGUMENT = -4,
    /** indicator that the object used was not a valid object */
    HELICS_ERROR_INVALID_OBJECT = -3,
    /** the operation to connect has failed */
    HELICS_ERROR_CONNECTION_FAILURE = -2,
    /** registration has failed */
    HELICS_ERROR_REGISTRATION_FAILURE = -1,
    /** the function executed successfully */
    HELICS_OK = 0
} HelicsErrorTypes;

/** enumeration of properties that apply to federates*/
typedef enum {
    /** the property controlling the minimum time delta for a federate*/
    HELICS_PROPERTY_TIME_DELTA = 137,
    /** the property controlling the period for a federate*/
    HELICS_PROPERTY_TIME_PERIOD = 140,
    /** the property controlling time offset for the period of federate*/
    HELICS_PROPERTY_TIME_OFFSET = 141,
    /** the property controlling real time lag for a federate the max time a federate can lag
       real time*/
    HELICS_PROPERTY_TIME_RT_LAG = 143,
    /** the property controlling real time lead for a federate the max time a federate can be
       ahead of real time*/
    HELICS_PROPERTY_TIME_RT_LEAD = 144,
    /** the property controlling real time tolerance for a federate sets both rt_lag and
       rt_lead*/
    HELICS_PROPERTY_TIME_RT_TOLERANCE = 145,
    /** the property controlling input delay for a federate*/
    HELICS_PROPERTY_TIME_INPUT_DELAY = 148,
    /** the property controlling output delay for a federate*/
    HELICS_PROPERTY_TIME_OUTPUT_DELAY = 150,
    /** integer property controlling the maximum number of iterations in a federate*/
    HELICS_PROPERTY_INT_MAX_ITERATIONS = 259,
    /** integer property controlling the log level in a federate see \ref helics_log_levels*/
    HELICS_PROPERTY_INT_LOG_LEVEL = 271,
    /** integer property controlling the log level for file logging in a federate see \ref
       helics_log_levels*/
    HELICS_PROPERTY_INT_FILE_LOG_LEVEL = 272,
    /** integer property controlling the log level for file logging in a federate see \ref
       helics_log_levels*/
    HELICS_PROPERTY_INT_CONSOLE_LOG_LEVEL = 274
} HelicsProperties;

/** enumeration of the multi_input operations*/
typedef enum {
    /** time and priority order the inputs from the core library*/
    HELICS_MULTI_INPUT_NO_OP = 0,
    /** vectorize the inputs either double vector or string vector*/
    HELICS_MULTI_INPUT_VECTORIZE_OPERATION = 1,
    /** all inputs are assumed to be boolean and all must be true to return true*/
    HELICS_MULTI_INPUT_AND_OPERATION = 2,
    /** all inputs are assumed to be boolean and at least one must be true to return true*/
    HELICS_MULTI_INPUT_OR_OPERATION = 3,
    /** sum all the inputs*/
    HELICS_MULTI_INPUT_SUM_OPERATION = 4,
    /** do a difference operation on the inputs, first-sum(rest)
    for double input, vector diff for vector input*/
    HELICS_MULTI_INPUT_DIFF_OPERATION = 5,
    /** find the max of the inputs*/
    HELICS_MULTI_INPUT_MAX_OPERATION = 6,
    /** find the min of the inputs*/
    HELICS_MULTI_INPUT_MIN_OPERATION = 7,
    /** take the average of the inputs*/
    HELICS_MULTI_INPUT_AVERAGE_OPERATION = 8
} HelicsMultiInputModes;

/** enumeration of options that apply to handles*/
typedef enum {
    /** specify that a connection is required for an interface and will generate an error if not
       available*/
    HELICS_HANDLE_OPTION_CONNECTION_REQUIRED = 397,
    /** specify that a connection is NOT required for an interface and will only be made if
       available no warning will be issues if not available*/
    HELICS_HANDLE_OPTION_CONNECTION_OPTIONAL = 402,
    /** specify that only a single connection is allowed for an interface*/
    HELICS_HANDLE_OPTION_SINGLE_CONNECTION_ONLY = 407,
    /** specify that multiple connections are allowed for an interface*/
    HELICS_HANDLE_OPTION_MULTIPLE_CONNECTIONS_ALLOWED = 409,
    /** specify that the last data should be buffered and sent on subscriptions after init*/
    HELICS_HANDLE_OPTION_BUFFER_DATA = 411,
    /** specify that the types should be checked strictly for pub/sub and filters*/
    HELICS_HANDLE_OPTION_STRICT_TYPE_CHECKING = 414,
    /** specify that the mismatching units should be ignored*/
    HELICS_HANDLE_OPTION_IGNORE_UNIT_MISMATCH = 447,
    /** specify that an interface will only transmit on change(only applicable to
       publications)*/
    HELICS_HANDLE_OPTION_ONLY_TRANSMIT_ON_CHANGE = 452,
    /** specify that an interface will only update if the value has actually changed*/
    HELICS_HANDLE_OPTION_ONLY_UPDATE_ON_CHANGE = 454,
    /** specify that an interface does not participate in determining time interrupts*/
    HELICS_HANDLE_OPTION_IGNORE_INTERRUPTS = 475,
    /** specify the multi-input processing method for inputs*/
    HELICS_HANDLE_OPTION_MULTI_INPUT_HANDLING_METHOD = 507,
    /** specify the source index with the highest priority*/
    HELICS_HANDLE_OPTION_INPUT_PRIORITY_LOCATION = 510,
    /** specify that the priority list should be cleared or question if it is cleared*/
    HELICS_HANDLE_OPTION_CLEAR_PRIORITY_LIST = 512,
    /** specify the required number of connections or get the actual number of connections*/
    HELICS_HANDLE_OPTION_CONNECTIONS = 522
} HelicsHandleOptions;

/** enumeration of the predefined filter types*/
typedef enum {
    /** a custom filter type that executes a user defined callback*/
    HELICS_FILTER_TYPE_CUSTOM = 0,
    /** a filter type that executes a fixed delay on a message*/
    HELICS_FILTER_TYPE_DELAY = 1,
    /** a filter type that executes a random delay on the messages*/
    HELICS_FILTER_TYPE_RANDOM_DELAY = 2,
    /** a filter type that randomly drops messages*/
    HELICS_FILTER_TYPE_RANDOM_DROP = 3,
    /** a filter type that reroutes a message to a different destination than originally
       specified*/
    HELICS_FILTER_TYPE_REROUTE = 4,
    /** a filter type that duplicates a message and sends the copy to a different destination*/
    HELICS_FILTER_TYPE_CLONE = 5,
    /** a customizable filter type that can perform different actions on a message based on
       firewall like rules*/
    HELICS_FILTER_TYPE_FIREWALL = 6
} HelicsFilterTypes;

/** enumeration of sequencing modes for queries
fast is the default, meaning the query travels along priority channels and takes precedence of over
existing messages; ordered means it follows normal priority patterns and will be ordered along with
existing messages
*/
typedef enum { HELICS_QUERY_MODE_FAST = 0, HELICS_QUERY_MODE_ORDERED = 1 } HelicsQueryModes;

#ifdef __cplusplus
} /* end of extern "C" { */
#endif
#endif
