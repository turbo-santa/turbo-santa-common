cc_binary(
  name = "turbo",
  srcs = ["main.cc"],
  deps = [
    "//backend/clocktroller",
    "//backend/memory:joypad_module",
  ],
  linkopts = [
    "-L/usr/local/lib",
    "-lncurses",
  ],
)
