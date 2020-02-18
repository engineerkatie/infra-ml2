#pragma once
//------------------------------------------------------------------------------
//
//   Copyright 2018-2019 Fetch.AI Limited
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
//------------------------------------------------------------------------------

#include <string>
#include <Python.h>

class PythonEngineException : public std::runtime_error
{
public:
  PythonEngineException(const std::string &message, const PyStatus &s);
  virtual ~PythonEngineException();
};

class PythonEngineFatalException : public std::runtime_error
{
public:
  PythonEngineFatalException(const std::string &message, const PyStatus &s);
  virtual ~PythonEngineFatalException();
};

void interpretPyStatus(const std::string &message, const PyStatus &s);
