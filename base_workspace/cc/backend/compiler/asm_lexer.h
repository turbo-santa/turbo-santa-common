#ifndef TURBO_SANTA_COMMON_BACKEND_COMPILER_ASM_LEXER_H_
#define TURBO_SANTA_COMMON_BACKEND_COMPILER_ASM_LEXER_H_

#ifndef __FLEX_LEXER_H
#define yyFlexLexer AsmFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

#include "cc/backend/compiler/tokens.h"

namespace backend {
namespace compiler {

class AsmLexer : public AsmFlexLexer {
 public:
  AsmLexer(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0)
      : AsmFlexLexer(arg_yyin, arg_yyout) {}

  ~AsmLexer() {}

  TokenType Lex(TokenValue* value) {
    TokenType token_type = static_cast<TokenType>(yylex());
    *value = current_token_value_;
    return token_type;
  }

  int yylex() override;

 private:
  TokenValue current_token_value_;
};

} // namespace compiler
} // backend

#endif // TURBO_SANTA_COMMON_BACKEND_COMPILER_ASM_LEXER_H_
