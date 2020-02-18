package(default_visibility = ["//visibility:public"])

cc_test(
    name = "pytests",
    srcs = glob(["*.cpp"]) + [
    ],
    linkstatic = 1,
    copts = [
    ],
    linkopts = ["-v", "-lutil",
      "-lz"],
    deps = [
         "//3rd/boost:boost_python",
        "@googletest//:gtest_main",
        "//python-support:python-support",
        "//python_engine:python_engine",
    ],
)
