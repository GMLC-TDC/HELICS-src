/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

public final class federate_state {
  public final static federate_state helics_startup_state = new federate_state("helics_startup_state", helicsJNI.helics_startup_state_get());
  public final static federate_state helics_initialization_state = new federate_state("helics_initialization_state");
  public final static federate_state helics_execution_state = new federate_state("helics_execution_state");
  public final static federate_state helics_finalize_state = new federate_state("helics_finalize_state");
  public final static federate_state helics_error_state = new federate_state("helics_error_state");
  public final static federate_state helics_pending_init_state = new federate_state("helics_pending_init_state");
  public final static federate_state helics_pending_exec_state = new federate_state("helics_pending_exec_state");
  public final static federate_state helics_pending_time_state = new federate_state("helics_pending_time_state");
  public final static federate_state helics_pending_iterative_time_state = new federate_state("helics_pending_iterative_time_state");

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static federate_state swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + federate_state.class + " with value " + swigValue);
  }

  private federate_state(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private federate_state(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private federate_state(String swigName, federate_state swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static federate_state[] swigValues = { helics_startup_state, helics_initialization_state, helics_execution_state, helics_finalize_state, helics_error_state, helics_pending_init_state, helics_pending_exec_state, helics_pending_time_state, helics_pending_iterative_time_state };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

