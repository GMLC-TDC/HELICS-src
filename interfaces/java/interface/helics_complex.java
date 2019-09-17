/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.0
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

public class helics_complex {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected helics_complex(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(helics_complex obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  @SuppressWarnings("deprecation")
  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        helicsJNI.delete_helics_complex(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setReal(double value) {
    helicsJNI.helics_complex_real_set(swigCPtr, this, value);
  }

  public double getReal() {
    return helicsJNI.helics_complex_real_get(swigCPtr, this);
  }

  public void setImag(double value) {
    helicsJNI.helics_complex_imag_set(swigCPtr, this, value);
  }

  public double getImag() {
    return helicsJNI.helics_complex_imag_get(swigCPtr, this);
  }

  public helics_complex() {
    this(helicsJNI.new_helics_complex(), true);
  }

}
