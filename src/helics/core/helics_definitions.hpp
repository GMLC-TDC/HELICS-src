/*
Copyright © 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/
#pragma once
/** @file
@details base helics enumerations for C++ API's
*/

namespace helics
{
namespace defs
{
enum flags : int
{
    /** flag indicating that a federate is observe only*/
    observer = helics_flag_observer,
    /** flag indicating that a federate can only return requested times*/
    uninterruptible = helics_flag_uninterruptible,
    /** flag indicating that a federate can be interrupted*/
    interruptible = helics_flag_interruptible,
    /** flag indicating that a federate/interface is a signal generator only*/
    source_only = helics_flag_source_only,
    /** flag indicating a federate/interface should only transmit values if they have changed(binary equivalence)*/
    only_transmit_on_change = helics_flag_only_transmit_on_change,
    /** flag indicating a federate/interface should only trigger an update if a value has changed (binary
     * equivalence)*/
    only_update_on_change = helics_flag_only_update_on_change,
    /** flag indicating a federate should only grant time if all other federates have already passed the requested
     * time*/
    wait_for_current_time_update = helics_flag_wait_for_current_time_update,
    /** flag indicating that a federate has rollback capability*/
    rollback = helics_flag_rollback,
    /** flag indicating that a federate performs forward computation and does internal rollback*/
    forward_compute = helics_flag_forward_compute,
    /** flag indicating that a federate needs to run in real time*/
    realtime = helics_flag_realtime,
    /** flag indicating that the federate will only interact on a single thread*/
    single_thread_federate = helics_flag_single_thread_federate,
    /** used to delay a core from entering initialization mode even if it would otherwise be ready*/
    delay_init_entry = helics_flag_delay_init_entry,
    /** used to clear the HELICS_DELAY_INIT_ENTRY flag in cores*/
    enable_init_entry = helics_flag_enable_init_entry,
    /** used to not display warnings on mismatched requested times*/
    ignore_time_mismatch_warnings = helics_flag_ignore_time_mismatch_warnings
};

enum errors : int
{

    ok = helics_ok, /*!< the function executed successfully */
    registration_failure = helics_error_registration_failure, /*!< registration has failed*/
    connection_failure = helics_error_connection_failure, /*!< the operation to connect has failed*/
    invalid_object = helics_error_invalid_object, /*!< indicator that the object used was not a valid object */
    invalid_argument = helics_error_invalid_argument, /*!< the parameter passed was invalid and unable to be used*/
    discard = helics_error_discard, /*!< the input was discarded and not used for some reason */
    system_failure =
      helics_error_system_failure, /*!< the federate has terminated unexpectedly and the call cannot be completed*/
    warning = helics_warning, /*!< the function issued a warning of some kind */
    invalid_state_transition =
      helics_error_invalid_state_transition, /*!< error issued when an invalid state transition occurred */
    invalid_function_call = helics_error_invalid_function_call, /*!< the call made was invalid in the present state
                                                                   of the calling object*/
    execution_failure = helics_error_execution_failure, /*!< the function execution has failed*/
    other = helics_error_other, /*!< the function produced a helics error of some other type */
};

enum properties : int
{
    time_delta = helics_property_time_delta,
    period = helics_property_time_period,
    offset = helics_property_time_offset,
    rt_lag = helics_property_time_rt_lag,
    rt_lead = helics_property_time_rt_lead,
    rt_tolerance = helics_property_time_rt_tolerance,
    input_delay = helics_property_time_input_delay,
    output_delay = helics_property_time_output_delay,
    max_iterations = helics_property_int_max_iterations,
    log_level = helics_property_int_log_level
};

enum options : int
{
    connection_required = helics_handle_option_connection_required,
    connection_optional = helics_handle_option_connection_optional,
    single_connection_only = helics_handle_option_single_connection_only,
    multiple_connections_allowed = helics_handle_option_multiple_connections_allowed,
    handle_only_transmit_on_change = helics_handle_option_only_transmit_on_change,
    handle_only_update_on_change = helics_handle_option_only_update_on_change
};

}  // namespace defs
}  // namespace helics
