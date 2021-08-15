cc_library(
    name = "solution",
    srcs = ["solution.cc"],
    deps = [
        "//library/graph",
        "//library/graph:edge_list",
        "//library/graph:minimum_spanning_tree",
    ],
)

cc_test(
    name = "solution_test",
    srcs = ["solution_test.cc"],
    data = glob(["testdata/*.txt"]),
    deps = [
        ":solution",
        "//util/solution_test",
        "@com_google_googletest//:gtest_main",
    ],
)
