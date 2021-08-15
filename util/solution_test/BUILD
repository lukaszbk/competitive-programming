package(default_visibility = ["//visibility:public"])

cc_library(
    name = "solution_test",
    srcs = ["solution_test.cc"],
    hdrs = ["solution_test.h"],
    deps = [
        "@com_google_googletest//:gtest",
    ],
)

cc_test(
    name = "solution_test_test",
    srcs = ["solution_test_test.cc"],
    data = glob(["testdata/*.txt"]),
    deps = [
        ":solution_test",
        "@com_google_googletest//:gtest_main",
    ],
)
