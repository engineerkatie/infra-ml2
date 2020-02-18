#include "python_exceptions.hpp"

#include <iostream>

PythonEngineException::PythonEngineException(const std::string &message, const PyStatus &s)
  : std::runtime_error(
      std::string("Python Error: ")
      + message
      + " -- "
      + s.err_msg
  )
{
}
PythonEngineException::~PythonEngineException()
{
}


PythonEngineFatalException::PythonEngineFatalException(const std::string &message, const PyStatus &s)
  : std::runtime_error(
      std::string("Python Fatal: ")
      + message
      + " -- "
      + s.err_msg
  )
{
}

PythonEngineFatalException::~PythonEngineFatalException()
{
}

void interpretPyStatus(const std::string &message, const PyStatus &s)
{
  if (PyStatus_IsError(s))
  {
    throw PythonEngineException(message, s);
  }
    if (PyStatus_IsExit(s))
  {
    throw PythonEngineFatalException(message, s);
  }
  std::cerr
    << "Python:"
    << message
    << " - "
    << "OK"
    << std::endl;
}

