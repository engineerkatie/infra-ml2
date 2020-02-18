#include "gil_lock.hpp"

#include <Python.h>

PyGILState_STATE state;

GilLock::GilLock()
{
  state = PyGILState_Ensure();
}

GilLock::~GilLock()
{
  PyGILState_Release(state);
}
