#include "frozen_modules.hpp"

#include <Python.h>

namespace {
#include "3rd/python38/freezing/M_importlib.c"
#include "3rd/python38/freezing/M_codecs.c"
#include "3rd/python38/freezing/M_encodings.c"
#include "3rd/python38/freezing/M_encodings__utf_8.c"
#include "3rd/python38/freezing/M___main__.c"
#include "3rd/python38/freezing/M_io.c"
#include "3rd/python38/freezing/M_os.c"
#include "3rd/python38/freezing/M_abc.c"

#include "3rd/python38/freezing/M__frozen_importlib.c"
#include "3rd/python38/freezing/M__frozen_importlib_external.c"
#include "3rd/python38/freezing/M_importlib__machinery.c"
#include "3rd/python38/freezing/M_importlib__util.c"
}

void addFrozenPythonModules(std::vector<struct _frozen> &modules)
{
  {
      struct _frozen m;
      m.name = "os";
      m.code = M_os;
      m.size = sizeof(M_os);
      modules.push_back(m);
    }
    {
      struct _frozen m;
      m.name = "io";
      m.code = M_io;
      m.size = sizeof(M_io);
      modules.push_back(m);
    }
    {
      struct _frozen m;
      m.name = "_frozen_importlib";
      m.code = M__frozen_importlib;
      m.size = sizeof(M__frozen_importlib);
      modules.push_back(m);
    }
    {
      struct _frozen m;
      m.name = "_frozen_importlib_external";
      m.code = M__frozen_importlib_external;
      m.size = sizeof(M__frozen_importlib_external);
      modules.push_back(m);
    }
    {
      struct _frozen m;
      m.name = "abc";
      m.code = M_abc;
      m.size = sizeof(M_abc);
      modules.push_back(m);
    }
    {
      struct _frozen m;
      m.name = "codecs";
      m.code = M_codecs;
      m.size = sizeof(M_codecs);
      modules.push_back(m);
    }
    {
      struct _frozen m;
      m.name = "encodings";
      m.code = M_encodings;
      m.size = sizeof(M_encodings);
      modules.push_back(m);
    }
    {
      struct _frozen m;
      m.name = "M_encodings__utf_8";
      m.code = M_encodings__utf_8;
      m.size = sizeof(M_encodings__utf_8);
      modules.push_back(m);
    }
    // {
    //   struct _frozen m;
    //   m.name = "__main__";
    //   m.code = M___main__;
    //   m.size = sizeof(M___main__);
    //   modules.push_back(m);
    // }

    {
      struct _frozen terminator;
      terminator.name = 0;
      terminator.size = 0;
      terminator.code = 0;
      modules.push_back(terminator);
    }
}

