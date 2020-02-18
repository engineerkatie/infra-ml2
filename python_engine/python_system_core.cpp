#include "python_system_core.hpp"

#include <mutex>
#include <iostream>
#include <Python.h>
#include <boost/python.hpp>

#include "python_engine/python_stdout_handler.hpp"
#include "python_engine/frozen_modules.hpp"
#include "python_engine/python_exceptions.hpp"
#include "python_engine/source_code.hpp"
#include "python_engine/python_engine_context.hpp"
#include "python_engine/gil_lock.hpp"

namespace {
  std::mutex mutex;
  bool initialised;
  std::vector<struct _frozen> modules;
};

extern const struct _frozen* PyImport_FrozenModules;

BOOST_PYTHON_MODULE(PythonStdoutHandlerModule)
{
  boost::python::class_<PythonStdoutHandler>("PythonStdoutHandler")
        .def("write", &PythonStdoutHandler::write)
    ;
};

void handleBoostPythonError()
{
  PyObject *ptype, *pvalue, *ptraceback;
  PyErr_Fetch(&ptype, &pvalue, &ptraceback);

  boost::python::handle<> hType(ptype);
  boost::python::object extype(hType);
  //boost::python::handle<> hTraceback(ptraceback);
  //boost::python::object traceback(hTraceback);

  //Extract error message
  std::string strErrorMessage = boost::python::extract<std::string>(pvalue);

  //Extract line number (top entry of call stack)
  // if you want to extract another levels of call stack
  // also process traceback.attr("tb_next") recurently
  //long lineno = boost::python::extract<long> (traceback.attr("tb_lineno"));
  //std::string filename = boost::python::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
  //std::string funcname = boost::python::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_name"));
  std::cerr << "fail " << strErrorMessage << std::endl;
  exit(78);
}


bool addMainModule()
{
    PyObject *m, *d, *loader, *ann_dict;
    m = PyImport_AddModule("__main__");
    if (m == NULL)
    {
      std::cout << ("can't create __main__ module") << std::endl;
      return false;
    }

    d = PyModule_GetDict(m);
    ann_dict = PyDict_New();
    if ((ann_dict == NULL) ||
        (PyDict_SetItemString(d, "__annotations__", ann_dict) < 0)) {
      std::cout << ("Failed to initialize __main__.__annotations__") << std::endl;
      return false;
    }
    Py_DECREF(ann_dict);

    if (PyDict_GetItemString(d, "__builtins__") == NULL) {
      PyObject *bimod = PyImport_ImportModule("builtins");
      if (bimod == NULL) {
        std::cout << ("Failed to retrieve builtins module") << std::endl;
        return false;
      }
      if (PyDict_SetItemString(d, "__builtins__", bimod) < 0) {
        std::cout << ("Failed to initialize __main__.__builtins__") << std::endl;
        return false;
      }
      Py_DECREF(bimod);
    }
    return true;
}

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

  addMainModule();

  std::vector<std::string> preload_frz = {
    "encodings",
    "io",
    "abc",
  };
  for(const auto &preload_frz_mod : preload_frz)
  {
    std::cout << "PythonSystemCore   PyImport_ImportFrozenModule  " << preload_frz_mod << std::endl;
    auto r = PyImport_ImportFrozenModule(preload_frz_mod.c_str());
    PyImport_ImportModule(preload_frz_mod.c_str());
    std::cout << "PythonSystemCore   PyImport_ImportFrozenModule  " << preload_frz_mod << " => " << r << std::endl;
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
    interp = 0;
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
  boost::shared_ptr<PythonStdoutHandler> stdout;
  SourceCode code;
  boost::python::dict main_module_dict;
  boost::python::dict main_namespace;
  boost::python::dict locals;

  PythonEngineContextInner(PythonEngineContextInner const &other) = delete;
  PythonEngineContextInner &operator=(PythonEngineContextInner const &other) = delete;
  bool operator==(PythonEngineContextInner const &other) = delete;
  bool operator<(PythonEngineContextInner const &other) = delete;

protected:
private:
};

std::shared_ptr<PythonEngineContext> PythonSystemCore::createContext(const SourceCode &source)
{
  //std::cout << "PythonSystemCore::createContext, 1" << std::endl;
  GilLock lock;
  //std::cout << "PythonSystemCore::createContext, 2" << std::endl;
  auto context = std::make_shared<PythonEngineContext>();
  //std::cout << "PythonSystemCore::createContext, 3" << std::endl;
  context -> inner = std::make_shared<PythonEngineContextInner>();
  //std::cout << "PythonSystemCore::createContext, 4" << std::endl;

  //std::cout << "PythonSystemCore::createContext, 5" << std::endl;
  context -> inner -> interp = Py_NewInterpreter();
  //std::cout << "PythonSystemCore::createContext, 6" << std::endl;

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
  return context;
}

void PythonSystemCore::executeContext(std::shared_ptr<PythonEngineContext> &context,
                                      ExecutionResult &result)
{
  try
  {
    PyInit_PythonStdoutHandlerModule();

    //std::cout << "PythonSystemCore::createContext, 8" << std::endl;
    auto main_dict_sys = context -> inner -> main_module_dict["sys"];
    auto main_dict_sys_dict = main_dict_sys.attr("__dict__");
    //std::cout << "PythonSystemCore::createContext, 9" << std::endl;
    auto stdoutHandler = boost::python::ptr(context -> inner -> stdout.get());
    main_dict_sys_dict["stdout"] = stdoutHandler;

    context -> inner -> stdout -> storeFunction = [&result](const std::string &text){ result.output.push_back(text); };
    boost::python::exec(boost::python::str(context -> inner -> code.asString().c_str()),
                        context -> inner -> main_module_dict,
                        context -> inner -> locals
                        );
    context -> inner -> stdout -> storeFunction = 0;
    result.success = true;
  }
  catch(boost::python::error_already_set&)
  {
    handleBoostPythonError();
    result.success = false;
  }

}
