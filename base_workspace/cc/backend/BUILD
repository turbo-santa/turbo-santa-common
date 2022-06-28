cc_binary(
  name = "turbo",
  srcs = ["main.cc"],
  deps = [
    "//cc/backend/clocktroller",
    "//cc/backend/memory/joypad:joypad_module",
    "@usr//:ncurses"
  ],
  linkopts = [
    "-L/usr/local/lib",
    "-lncurses",
  ],
)
