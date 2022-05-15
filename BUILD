load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "id3",
    srcs = ["id3.c"],
    deps = [":id3-lib"],
)

cc_library(
    name = "id3-lib",
    srcs = glob(["*.c"]),
    hdrs = glob(["*.h"]),
)
