cc_binary(
  name = "turbo",
  srcs = ["main.cc"],
  deps = [
    "//cc/backend/clocktroller",
    "//cc/backend/memory:joypad_module",
  ],
  linkopts = [
    "-L/usr/local/lib",
    "-lncurses",
  ],
)
