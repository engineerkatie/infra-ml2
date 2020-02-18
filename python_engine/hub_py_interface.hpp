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

#include "messages/raw_message.hpp"
#include <Python.h>
#include <boost/python.hpp>

namespace bp = boost::python;

class HubPyInterface
{
public:
  using AsyncMessage = boost::shared_ptr<RawMessage>;
  using AsyncMessageQ = std::list<AsyncMessage>;

  HubPyInterface()
  {
  }
  virtual ~HubPyInterface()
  {
  }

  HubPyInterface(HubPyInterface const &other) = delete; // { copy(other); }
  HubPyInterface &operator=(HubPyInterface const &other) = delete; // { copy(other); return *this; }
  bool operator==(HubPyInterface const &other) = delete; // const { return compare(other)==0; }
  bool operator<(HubPyInterface const &other) = delete; // const { return compare(other)==-1; }

protected:

  virtual bp::object queryMessages()
  {
    bool hasMessages = queue.size() > 0;
    return bp::object(hasMessages);
  }

  virtual bp::object getMessage()
  {
    AsyncMessage msg = queue.front();
    queue.pop_front();

    auto obj = PyBytes_FromStringAndSize(
                                         reinterpret_cast<const char*>(msg->accessBytes()),
                                         static_cast<Py_ssize_t>(msg->size())
                                         );
    // that copies the data.
    return boost::python::object(boost::python::handle<>(boost::python::borrowed(obj)));
  }

  virtual bool hasMessageCallback()
  {
    return messageCallback;
  }

  virtual void assignMessageCallback(boost::python::object &func)
  {
    messageCallback = func;
  }

private:
  boost::python::object messageCallback;
  AsyncMessageQ queue;
};
