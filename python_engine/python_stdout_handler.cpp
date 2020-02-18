#include "python_stdout_handler.hpp"

#include <iostream>

PythonStdoutHandler::PythonStdoutHandler()
{
}

PythonStdoutHandler::~PythonStdoutHandler()
{
}

void PythonStdoutHandler::write(const std::string& text)
{
  if (storeFunction)
  {
    storeFunction(text);
  }
  else
  {
    std::cout << text << std::endl;
  }
}

void PythonStdoutHandler::copy(PythonStdoutHandler const &other)
{
  this -> storeFunction = other.storeFunction;
}
