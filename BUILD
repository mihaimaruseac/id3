load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "id3",
    srcs = ["id3.c"],
    deps = [
        ":globals",
        ":id3graph",
        ":id3learn",
        ":id3test",
    ],
)

cc_library(
    name = "globals",
    srcs = ["globals.c"],
    hdrs = ["globals.h"],
)

cc_library(
    name = "id3graph",
    srcs = ["id3graph.c"],
    hdrs = ["id3graph.h"],
    deps = [":globals"],
)

cc_library(
    name = "id3learn",
    srcs = ["id3learn.c"],
    hdrs = ["id3learn.h"],
    deps = [
        ":globals",
        ":id3math",
        ":id3missing",
    ],
)

cc_library(
    name = "id3math",
    srcs = ["id3math.c"],
    hdrs = ["id3math.h"],
    deps = [":globals"],
)

cc_library(
    name = "id3missing",
    srcs = ["id3missing.c"],
    hdrs = ["id3missing.h"],
    deps = [":globals"],
)

cc_library(
    name = "id3test",
    srcs = ["id3test.c"],
    hdrs = ["id3test.h"],
    deps = [":globals"],
)
