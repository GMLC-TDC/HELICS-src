/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

/**
 * enumeration of possible federate states
 */
public final class helics_federate_state {
  /**
   *  when created the federate is in startup state 
   */
  public final static helics_federate_state helics_state_startup = new helics_federate_state("helics_state_startup", helicsJNI.helics_state_startup_get());
  /**
   *  entered after the enterInitializingMode call has returned 
   */
  public final static helics_federate_state helics_state_initialization = new helics_federate_state("helics_state_initialization");
  /**
   *  entered after the enterExectuationState call has returned 
   */
  public final static helics_federate_state helics_state_execution = new helics_federate_state("helics_state_execution");
  /**
   *  the federate has finished executing normally final values may be retrieved 
   */
  public final static helics_federate_state helics_state_finalize = new helics_federate_state("helics_state_finalize");
  /**
   *  error state no core communication is possible but values can be retrieved 
   */
  public final static helics_federate_state helics_state_error = new helics_federate_state("helics_state_error");
  /**
   *  indicator that the federate is pending entry to initialization state 
   */
  public final static helics_federate_state helics_state_pending_init = new helics_federate_state("helics_state_pending_init");
  /**
   *  state pending EnterExecution State 
   */
  public final static helics_federate_state helics_state_pending_exec = new helics_federate_state("helics_state_pending_exec");
  /**
   *  state that the federate is pending a timeRequest 
   */
  public final static helics_federate_state helics_state_pending_time = new helics_federate_state("helics_state_pending_time");
  /**
   *  state that the federate is pending an iterative time request 
   */
  public final static helics_federate_state helics_state_pending_iterative_time = new helics_federate_state("helics_state_pending_iterative_time");
  /**
   *  state that the federate is pending a finalize request 
   */
  public final static helics_federate_state helics_state_pending_finalize = new helics_federate_state("helics_state_pending_finalize");

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static helics_federate_state swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + helics_federate_state.class + " with value " + swigValue);
  }

  private helics_federate_state(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private helics_federate_state(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private helics_federate_state(String swigName, helics_federate_state swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static helics_federate_state[] swigValues = { helics_state_startup, helics_state_initialization, helics_state_execution, helics_state_finalize, helics_state_error, helics_state_pending_init, helics_state_pending_exec, helics_state_pending_time, helics_state_pending_iterative_time, helics_state_pending_finalize };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

