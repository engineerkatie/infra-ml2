package(default_visibility = ["//visibility:public"])

cc_library(
    name = "python_engine",
    srcs = glob(["*.cpp"]) + [
         "//3rd/boost:libboost_python.a",
    ],
    hdrs = glob(["*.hpp", "*.h"]) + [
         "//3rd/boost:boost-python-hdrs",
         "//3rd/python38:python38_frozen_modules_sources",
    ],
    deps = [
         "//3rd/boost:boost_python",
         "@boost//:bind",
         "@boost//:config",
         "@boost//:conversion",
         "@boost//:core",
         "@boost//:detail",
         "@boost//:foreach",
         "@boost//:function",
         "@boost//:iterator",
         "@boost//:lexical_cast",
         "@boost//:mpl",
         "@boost//:numeric_conversion",
         "@boost//:preprocessor",
         "@boost//:smart_ptr",
         "@boost//:static_assert",
         "@boost//:tuple",
         "@boost//:type_traits",
         "@boost//:utility",
         "//messages:messages",
         "//workloads:workloads",
    ] +
    [
         "//3rd/python38:python38",
         "//3rd/python38:python38_frozen_modules",
    ],
    linkopts = [
    ],
    copts = [
    ],
    data = [
    ],
)
