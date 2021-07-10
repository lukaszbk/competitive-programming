cc_library(
    name = "edge_list",
    hdrs = ["edge_list.h"],
)

cc_test(
    name = "edge_list_test",
    srcs = ["edge_list_test.cc"],
    deps = [
        ":edge_list",
        ":graph",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "graph",
    hdrs = ["graph.h"],
    deps = [
        ":vertex_list",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "vertex_list",
    hdrs = ["vertex_list.h"],
)

cc_test(
    name = "vertex_list_test",
    srcs = ["vertex_list_test.cc"],
    deps = [
        ":edge_list",
        ":graph",
        ":vertex_list",
        "@com_google_googletest//:gtest_main",
    ],
)
