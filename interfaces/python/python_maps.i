%{
#include "api-data.h"

static PyObject* pHelicsException;

/* throw a helics error */
static void throwHelicsPythonException(helics_error *err) {
  switch (err->error_code)
  {
  case helics_ok:
    return;
  case helics_error_registration_failure:
    PyErr_SetString(pHelicsException, strcat("helics:registration_failure", err->message));
    SWIG_fail;
    break;
  case   helics_error_connection_failure:
    PyErr_SetString(pHelicsException, strcat("helics:connection_failure", err->message));
    SWIG_fail;
    break;
  case   helics_error_invalid_object:
    PyErr_SetString(pHelicsException, strcat("helics:invalid_object", err->message));
    SWIG_fail;
    break;
  case   helics_error_invalid_argument:
    PyErr_SetString(pHelicsException, strcat("helics:invalid_argument", err->message));
    SWIG_fail;
    break;
  case   helics_error_discard:
    PyErr_SetString(pHelicsException, strcat("helics:discard", err->message));
    SWIG_fail;
    break;
  case helics_error_system_failure:
    PyErr_SetString(pHelicsException, strcat("helics:system_failure", err->message));
    SWIG_fail;
    break;
  case   helics_error_invalid_state_transition:
    PyErr_SetString(pHelicsException, strcat("helics:invalid_state_transition", err->message));
    SWIG_fail;
    break;
  case   helics_error_invalid_function_call:
    PyErr_SetString(pHelicsException, strcat("helics:invalid_function_call", err->message));
    SWIG_fail;
	break;
  case   helics_error_execution_failure:
    PyErr_SetString(pHelicsException, strcat("helics:execution_failure", err->message));
    SWIG_fail;
	break;
  case   helics_error_other:
  case   other_error_type:
  default:
    PyErr_SetString(pHelicsException, strcat("helics:error", err->message));
    SWIG_fail;
	break;
  }
}

%}

%init %{
    pHelicsException = PyErr_NewException("_helics.pHelicsException", NULL, NULL);
    Py_INCREF(pHelicsException);
    PyModule_AddObject(m, "HelicsException", pHelicsException);
%}

%typemap(in, numinputs=0) helics_error * (helics_error etemp) {
    etemp=helicsErrorInitialize();
    $1=&etemp;
}

%typemap(freearg) helics_error *
{
    if ($1->error_code!=helics_ok)
    {
        throwHelicsPythonException($1);
    }
}



//typemap for short maxlen strings
%typemap(in, numinputs=0) (char *outputString, int maxlen) {
  $2=256;
  $1=(char *)malloc(256);
}

%typemap(argout) (char *outputString, int maxlen) {
  PyObject *str=PyString_FromString($1);
  $result = SWIG_Python_AppendOutput($result, str);
}

%typemap(freearg) (char *outputString, int maxlen) {
   if ($1) free($1);
}


//typemap for large string output with a length return in C
%typemap(in, numinputs=0) (char *outputString, int maxStringLen, int *actualLength) {
  $3=&($2);
}

%typemap(freearg) (char *outputString, int maxStringLen, int *actualLength) {
   if ($1) free($1);
}

%typemap(check)(char *outputString, int maxStringLen, int *actualLength) {
    $2=helicsSubscriptionGetStringSize(arg1)+2;
    $1 = (char *) malloc($2);
}

%typemap(argout) (char *outputString, int maxStringLen, int *actualLength) {
  PyObject *o2=PyString_FromString($1);
  $result = SWIG_Python_AppendOutput($result, o2);
}


//typemap for the input arguments
%typemap(in) (int argc, const char *const *argv) {
  /* Check if is a list */
  if (PyList_Check($input)) {
    int i;
    $1 = PyList_Size($input);
    $2 = (char **) malloc(($1+1)*sizeof(char *));
    for (i = 0; i < $1; i++) {
      PyObject *o = PyList_GetItem($input,i);
      if (PyString_Check(o))
	$2[i] = PyString_AsString(PyList_GetItem($input,i));
      else {
	PyErr_SetString(PyExc_TypeError,"list must contain strings");
	free($2);
	return NULL;
      }
    }
    $2[i] = 0;
  } else {
    PyErr_SetString(PyExc_TypeError,"not a list");
    return NULL;
  }
}

%typemap(freearg) (int argc, const char *const *argv) {
  free((char *) $2);
}

// typemap for vector input functions
%typemap(in) (const double *vectorInput, int vectorlength) {
  int i;
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a list");
    return NULL;
  }
  $2=PyList_Size($input);
  $1 = (double *) malloc($2*sizeof(double));

  for (i = 0; i < $2; i++) {
    PyObject *o = PyList_GetItem($input,i);
    if (PyFloat_Check(o)) {
      $1[i] = PyFloat_AsDouble(o);
	}else if (PyInt_Check(o))
	{
		$1[i] = (double)(PyInt_AsLong(o));
	} else {
      PyErr_SetString(PyExc_ValueError,"List elements must be numbers");
      free($1);
      return NULL;
    }
  }
}

%typemap(argout) (const double *vectorInput, int vectorlength)
{
}

%typemap(freearg) (const double *vectorInput, int vectorlength) {
   if ($1) free($1);
}

// typemap for vector output functions

%typemap(arginit) (double data[], int maxlen, int *actualSize) {
  $1=(double *)(NULL);
}

%typemap(in, numinputs=0) (double data[], int maxlen, int *actualSize) {
  $3=&($2);
}

%typemap(freearg) (double data[], int maxlen, int *actualSize) {
   if ($1) free($1);
}

// Set argument to NULL before any conversion occurs
%typemap(check)(double data[], int maxlen, int *actualSize) {
    $2=helicsInputGetVectorSize(arg1);
    $1 = (double *) malloc($2*sizeof(double));
}

%typemap(argout) (double data[], int maxlen, int *actualSize) {
  int i;
  PyObject *o2=PyList_New(*$3);
  for (i = 0; i < *$3; i++) {
	PyObject *o_item=PyFloat_FromDouble($1[i]);
      PyList_SetItem(o2, i, o_item);
      }

  $result = SWIG_Python_AppendOutput($result, o2);
}


// typemap for raw data input
%typemap(in) (const void *data, int inputDataLength) {
  if (PyUnicode_Check($input)) {
	int kind=PyUnicode_KIND($input);
    $1=PyUnicode_DATA($input);
	switch(kind)
	{
	case PyUnicode_1BYTE_KIND:
	default:
		$2=PyUnicode_GetLength($input);
	break;
	case PyUnicode_2BYTE_KIND:
	case PyUnicode_WCHAR_KIND:
		$2=PyUnicode_GetLength($input)*2;
	break;
	case PyUnicode_4BYTE_KIND:
		$2=PyUnicode_GetLength($input)*4;
	break;
	}
  }
  else if (PyBytes_Check($input)) {
    $1=PyBytes_AsString($input);
	$2=PyBytes_Size($input);
  }
  else
  {
	PyErr_SetString(PyExc_ValueError,"Expected a string or bytes");
    return NULL;
  }
}

%typemap(argout) (const void *data, int inputDataLength)
{
}


// typemap for raw data output function
%typemap(in, numinputs=0) (void *data, int maxDatalen, int *actualSize) {
  $3=&($2);
}

%typemap(freearg) (void *data, int maxDatalen, int *actualSize) {
   if ($1) free($1);
}

// Set argument to NULL before any conversion occurs
%typemap(check)(void *data, int maxDatalen, int *actualSize) {
    $2=helicsInputGetValueSize(arg1)+2;
    $1 =  malloc($2);
}

%typemap(argout) (void *data, int maxDatalen, int *actualSize) {
  PyObject *o2=PyBytes_FromStringAndSize($1,*$3);
  $result = SWIG_Python_AppendOutput($result, o2);
}
