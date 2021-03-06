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

#include <vector>
#include <string>

class SourceCode
{
public:
  using const_iterator = std::vector<std::string>::const_iterator;

  template<class CONTAINER>
  SourceCode(const CONTAINER &input)
  {
    for(const auto &i : input)
    {
      store.push_back(i);
    }
  }
  virtual ~SourceCode()
  {
  }

  SourceCode()
  {
  }

  SourceCode(SourceCode const &other)
  {
    copy(other);
  }
  SourceCode &operator=(SourceCode const &other)
  {
    copy(other);
    return *this;
  }
  bool operator==(SourceCode const &other) = delete;
  bool operator<(SourceCode const &other) = delete;

  virtual void copy(SourceCode const &other)
  {
    for(const auto &i : other)
    {
      store.push_back(i);
    }
  }

  virtual const_iterator begin() const
  {
    return store.begin();
  }

  virtual const_iterator end() const
  {
    return store.end();
  }

  std::vector<std::string> store;

  std::string asString()
  {
    std::string s;
    for( auto const &line : store)
    {
      s += line;
      s += "\n";
    }
    return s;
  }

protected:

private:

};
