#include "python_system_core.hpp"

#include <mutex>
#include <iostream>
#include <Python.h>

#include "python_engine/python_stdout_handler.hpp"
#include "python_engine/frozen_modules.hpp"
#include "python_engine/python_exceptions.hpp"
#include "python_engine/python_engine_context.hpp"
#include "python_engine/gil_lock.hpp"

namespace {
  std::mutex mutex;
  bool initialised;
  std::vector<struct _frozen> modules;
};

extern const struct _frozen* PyImport_FrozenModules;

PythonSystemCore::PythonSystemCore()
{
  std::lock_guard<std::mutex> lock(mutex);

  if (initialised)
  {
    return;
  }

  initialised = true;

  PyConfig conf;
  PyPreConfig pconf;

  PyPreConfig_InitIsolatedConfig(&pconf);
  PyConfig_InitIsolatedConfig(&conf);

  pconf.coerce_c_locale = 2; // C locale
  pconf.dev_mode = 1;
  pconf.utf8_mode = 1;
  conf.site_import = 0;
  conf.pathconfig_warnings = 0;
  conf._install_importlib = 0;
  conf._init_main = 1;
  conf.configure_c_stdio = 1;

  {
    auto s = PyConfig_SetString(&conf, &conf.program_name,
                                L"./apphost");
    interpretPyStatus("set program_name", s);
  }

  {
    auto s = PyConfig_SetString(&conf, &conf.filesystem_encoding, L"utf_8");
    interpretPyStatus("set filesystem_encoding", s);
  }

  {
    auto s = Py_PreInitialize(&pconf);
    interpretPyStatus("calling Py_PreInitialize", s);
  }

  addFrozenPythonModules(modules);
  PyImport_FrozenModules = modules.data();

  {
    auto s = Py_InitializeFromConfig(&conf);
    interpretPyStatus("calling Py_Initialize", s);
  }

  PyConfig_Clear(&conf);
  std::cout << "PythonSystemCore::PythonSystemCore; Py_IsInitialized => true" << std::endl;
}

PythonSystemCore::~PythonSystemCore()
{
}


class PythonEngineContextInner
{
public:
  friend class PythonEngine;
  friend class PythonEngineContext;

  PythonEngineContextInner()
  {
  }
  virtual ~PythonEngineContextInner()
  {
    if (interp)
    {
      Py_EndInterpreter(interp);
      interp = 0;
    }
  }

  PyThreadState *interp;

  PythonEngineContextInner(PythonEngineContextInner const &other) = delete;
  PythonEngineContextInner &operator=(PythonEngineContextInner const &other) = delete;
  bool operator==(PythonEngineContextInner const &other) = delete;
  bool operator<(PythonEngineContextInner const &other) = delete;

protected:
private:
};

std::shared_ptr<PythonEngineContext> PythonSystemCore::createContext(const SourceCode &source)
{
  GilLock lock;
  auto context = std::make_shared<PythonEngineContext>();
  context -> inner = std::make_shared<PythonEngineContextInner>();
  context -> inner -> interp = Py_NewInterpreter();


  /*
  context -> inner -> stdout = boost::shared_ptr<PythonStdoutHandler>(new PythonStdoutHandler());
  context -> inner -> code = source;

  boost::python::object main_module = boost::python::import("__main__");
  context -> inner -> main_module_dict = boost::python::extract<boost::python::dict>(main_module.attr("__dict__"));

  std::vector<std::string> preloads = {
    "sys",
  };

  //std::cout << "PythonSystemCore::createContext, 7" << std::endl;
  for(const auto &mod : preloads)
  {
    auto modulePyObject = PyImport_ImportModule(mod.c_str());
    if (modulePyObject == 0)
    {
      std::cout << "PyImport_ImportModule failed "<<mod<<std::endl;
      PyErr_Print();
      exit(66);
    }
    context -> inner -> main_module_dict[mod] = boost::python::handle<>(modulePyObject);
  }

  
  //std::cout << "PythonSystemCore::createContext, 10" << std::endl;
  */
  return context;
}

