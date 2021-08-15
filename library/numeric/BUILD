package(default_visibility = ["//visibility:public"])

cc_library(
    name = "gcd",
    hdrs = ["gcd.h"],
)

cc_test(
    name = "gcd_test",
    srcs = ["gcd_test.cc"],
    deps = [
        ":gcd",
        "@com_google_googletest//:gtest_main",
    ],
)
