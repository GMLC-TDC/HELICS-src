/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

public final class helics_core_type {
  public final static helics_core_type helics_core_type_default = new helics_core_type("helics_core_type_default", helicsJNI.helics_core_type_default_get());
  public final static helics_core_type helics_core_type_zmq = new helics_core_type("helics_core_type_zmq", helicsJNI.helics_core_type_zmq_get());
  public final static helics_core_type helics_core_type_mpi = new helics_core_type("helics_core_type_mpi", helicsJNI.helics_core_type_mpi_get());
  public final static helics_core_type helics_core_type_test = new helics_core_type("helics_core_type_test", helicsJNI.helics_core_type_test_get());
  public final static helics_core_type helics_core_type_interprocess = new helics_core_type("helics_core_type_interprocess", helicsJNI.helics_core_type_interprocess_get());
  public final static helics_core_type helics_core_type_ipc = new helics_core_type("helics_core_type_ipc", helicsJNI.helics_core_type_ipc_get());
  public final static helics_core_type helics_core_type_tcp = new helics_core_type("helics_core_type_tcp", helicsJNI.helics_core_type_tcp_get());
  public final static helics_core_type helics_core_type_udp = new helics_core_type("helics_core_type_udp", helicsJNI.helics_core_type_udp_get());
  public final static helics_core_type helics_core_type_zmq_test = new helics_core_type("helics_core_type_zmq_test", helicsJNI.helics_core_type_zmq_test_get());
  public final static helics_core_type helics_core_type_nng = new helics_core_type("helics_core_type_nng", helicsJNI.helics_core_type_nng_get());
  public final static helics_core_type helics_core_type_tcp_ss = new helics_core_type("helics_core_type_tcp_ss", helicsJNI.helics_core_type_tcp_ss_get());
  public final static helics_core_type helics_core_type_http = new helics_core_type("helics_core_type_http", helicsJNI.helics_core_type_http_get());
  public final static helics_core_type helics_core_type_websocket = new helics_core_type("helics_core_type_websocket", helicsJNI.helics_core_type_websocket_get());
  public final static helics_core_type helics_core_type_inproc = new helics_core_type("helics_core_type_inproc", helicsJNI.helics_core_type_inproc_get());

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static helics_core_type swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + helics_core_type.class + " with value " + swigValue);
  }

  private helics_core_type(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private helics_core_type(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private helics_core_type(String swigName, helics_core_type swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static helics_core_type[] swigValues = { helics_core_type_default, helics_core_type_zmq, helics_core_type_mpi, helics_core_type_test, helics_core_type_interprocess, helics_core_type_ipc, helics_core_type_tcp, helics_core_type_udp, helics_core_type_zmq_test, helics_core_type_nng, helics_core_type_tcp_ss, helics_core_type_http, helics_core_type_websocket, helics_core_type_inproc };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

