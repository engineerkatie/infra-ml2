#include "python_stdout_handler.hpp"

#include <iostream>

PythonStdoutHandler::PythonStdoutHandler()
{
}

PythonStdoutHandler::~PythonStdoutHandler()
{
  std::cout << "PythonStdoutHandler::~PythonStdoutHandler" << std::endl;
}

void PythonStdoutHandler::write(const std::string& text)
{
  std::cout << text << std::endl;
  store.push_back(text);
}

PythonStdoutHandler::const_iterator PythonStdoutHandler::begin() const
{
  return store.begin();
}

PythonStdoutHandler::const_iterator PythonStdoutHandler::end() const
{
  return store.end();
}
