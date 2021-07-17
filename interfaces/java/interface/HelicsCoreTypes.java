/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

/**
 *  pick a core type depending on compile configuration usually either ZMQ if available or TCP 
 */
public final class HelicsCoreTypes {
  /**
   *  a default core type that will default to something available
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_DEFAULT = new HelicsCoreTypes("HELICS_CORE_TYPE_DEFAULT", helicsJNI.HELICS_CORE_TYPE_DEFAULT_get());
  /**
   *  use the Zero MQ networking protocol 
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_ZMQ = new HelicsCoreTypes("HELICS_CORE_TYPE_ZMQ", helicsJNI.HELICS_CORE_TYPE_ZMQ_get());
  /**
   *  use MPI for operation on a parallel cluster 
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_MPI = new HelicsCoreTypes("HELICS_CORE_TYPE_MPI", helicsJNI.HELICS_CORE_TYPE_MPI_get());
  /**
   *  use the Test core if all federates are in the same process 
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_TEST = new HelicsCoreTypes("HELICS_CORE_TYPE_TEST", helicsJNI.HELICS_CORE_TYPE_TEST_get());
  /**
   *  interprocess uses memory mapped files to transfer data (for use when all federates are<br>
   *         on the same machine 
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_INTERPROCESS = new HelicsCoreTypes("HELICS_CORE_TYPE_INTERPROCESS", helicsJNI.HELICS_CORE_TYPE_INTERPROCESS_get());
  /**
   *  interprocess uses memory mapped files to transfer data (for use when all federates are<br>
   *         on the same machine ipc is the same as /ref HELICS_CORE_TYPE_interprocess
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_IPC = new HelicsCoreTypes("HELICS_CORE_TYPE_IPC", helicsJNI.HELICS_CORE_TYPE_IPC_get());
  /**
   *  use a generic TCP protocol message stream to send messages 
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_TCP = new HelicsCoreTypes("HELICS_CORE_TYPE_TCP", helicsJNI.HELICS_CORE_TYPE_TCP_get());
  /**
   *  use UDP packets to send the data 
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_UDP = new HelicsCoreTypes("HELICS_CORE_TYPE_UDP", helicsJNI.HELICS_CORE_TYPE_UDP_get());
  /**
   *  single socket version of ZMQ core usually for high fed count on the same system
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_ZMQ_SS = new HelicsCoreTypes("HELICS_CORE_TYPE_ZMQ_SS", helicsJNI.HELICS_CORE_TYPE_ZMQ_SS_get());
  /**
   *  for using the nanomsg communications 
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_NNG = new HelicsCoreTypes("HELICS_CORE_TYPE_NNG", helicsJNI.HELICS_CORE_TYPE_NNG_get());
  /**
   *  a single socket version of the TCP core for more easily handling firewalls
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_TCP_SS = new HelicsCoreTypes("HELICS_CORE_TYPE_TCP_SS", helicsJNI.HELICS_CORE_TYPE_TCP_SS_get());
  /**
   *  a core type using http for communication
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_HTTP = new HelicsCoreTypes("HELICS_CORE_TYPE_HTTP", helicsJNI.HELICS_CORE_TYPE_HTTP_get());
  /**
   *  a core using websockets for communication
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_WEBSOCKET = new HelicsCoreTypes("HELICS_CORE_TYPE_WEBSOCKET", helicsJNI.HELICS_CORE_TYPE_WEBSOCKET_get());
  /**
   *  an in process core type for handling communications in shared<br>
   *                                      memory it is pretty similar to the test core but stripped from<br>
   *                                      the "test" components
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_INPROC = new HelicsCoreTypes("HELICS_CORE_TYPE_INPROC", helicsJNI.HELICS_CORE_TYPE_INPROC_get());
  /**
   *  an explicit core type that is recognized but explicitly doesn't<br>
   *                                   exist, for testing and a few other assorted reasons
   */
  public final static HelicsCoreTypes HELICS_CORE_TYPE_NULL = new HelicsCoreTypes("HELICS_CORE_TYPE_NULL", helicsJNI.HELICS_CORE_TYPE_NULL_get());

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static HelicsCoreTypes swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + HelicsCoreTypes.class + " with value " + swigValue);
  }

  private HelicsCoreTypes(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private HelicsCoreTypes(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private HelicsCoreTypes(String swigName, HelicsCoreTypes swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static HelicsCoreTypes[] swigValues = { HELICS_CORE_TYPE_DEFAULT, HELICS_CORE_TYPE_ZMQ, HELICS_CORE_TYPE_MPI, HELICS_CORE_TYPE_TEST, HELICS_CORE_TYPE_INTERPROCESS, HELICS_CORE_TYPE_IPC, HELICS_CORE_TYPE_TCP, HELICS_CORE_TYPE_UDP, HELICS_CORE_TYPE_ZMQ_SS, HELICS_CORE_TYPE_NNG, HELICS_CORE_TYPE_TCP_SS, HELICS_CORE_TYPE_HTTP, HELICS_CORE_TYPE_WEBSOCKET, HELICS_CORE_TYPE_INPROC, HELICS_CORE_TYPE_NULL };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

