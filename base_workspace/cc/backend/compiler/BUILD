cc_library(
  name = "asm_lexer",
  hdrs = ["asm_lexer.h"],
  srcs = ["asm_lexer.cc"],
  deps = [":tokens"],
)

genrule(
  name = "generate_lexer",
  srcs = ["asm_flex_lexer.lex"],
  outs = ["asm_lexer.cc"],
  cmd = "flex -o $@ $<"
)

cc_library(
  name = "tokens",
  hdrs = ["tokens.h"],
  deps = ["//cc/backend/decompiler:instruction"],
)
