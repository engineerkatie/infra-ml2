#include "gtest/gtest.h"
#include <dlfcn.h>

#include "python_engine/execution_result.hpp"
#include "python_engine/python_system_core.hpp"


TEST(OefMonitoringIntegration, canDoPython2)
{
  PythonSystemCore psc;

  std::vector<std::string> code1 = {
                 "",
                 "def foo():",
                 "  print('hello1')",
                 "",
                 "if __name__ == '__main__':",
                 "  foo()",
  };

  std::vector<std::string> code2 = {
                 "",
                 "def foo():",
                 "  print('hello2')",
                 "",
                 "if __name__ == '__main__':",
                 "  foo()",
  };

  ExecutionResult er1;
  ExecutionResult er2;

  auto runner1 = psc.createContext(SourceCode(code1));
  auto runner2 = psc.createContext(SourceCode(code2));

  psc.executeContext(runner1, er1);
  psc.executeContext(runner2, er2);

  EXPECT_EQ(er1.output.size(), 2);
  EXPECT_EQ(er2.output.size(), 2);

  EXPECT_EQ(er1.output[0], "hello1");
  EXPECT_EQ(er2.output[0], "hello2");
}


}  // namespace dmlf
}  // namespace fetch
