#include "token.h"
#include <stdio.h>

Token token__new(TokenType tokentype) {
  return (Token){.tokentype = tokentype};
}

Token token__new_with_val(TokenType tokentype, StringView stringview) {
  return (Token){.tokentype = tokentype, .stringview = stringview};
}

bool is_int(char *mem, size_t len) {
  size_t index = 0;

  if (mem[index] == '-' || mem[index] == '+') {
    ++index;
  }

  if (&(mem[index]) - mem >= len) {
    return false;
  }

  for (size_t i = index; i < len; i++) {
    if (mem[i] < 48 || mem[i] > 57) {
      return false;
    }
  }
  return true;
}

void cvector_tokens__tokenize(cvector_tokens_t *tokens, char *input) {
  size_t index = 0;

  for (;;) {
    if (index >= strlen(input))
      break;

    if (input[index] == ' ' || input[index] == '\n' || input[index] == ',' ||
        input[index] == '\r') {
      ++index;
      continue;
    }

    if (input[index] == '(') {
      cvector__add(tokens, token__new(TOKEN__LEFT_PAREN));
      ++index;
      continue;
    }

    if (input[index] == ')') {
      cvector__add(tokens, token__new(TOKEN__RIGHT_PAREN));
      ++index;
      continue;
    }

    if (input[index] == '[') {
      cvector__add(tokens, token__new(TOKEN__LEFT_SQ_BR));
      ++index;
      continue;
    }

    if (input[index] == ']') {
      cvector__add(tokens, token__new(TOKEN__RIGHT_SQ_BR));
      ++index;
      continue;
    }

    if (input[index] == ';') {
      char *mem = &(input[index]);
      for (;;) {
        if (index >= strlen(input) || input[index] == '\n') {
          break;
        }
        ++index;
      }

      Token token = token__new_with_val(
          TOKEN__COMMENT,
          stringview__new(mem, ((&(input[index - 1]) - mem + 1))));
      cvector__add(tokens, token);
      continue;
    }

    if (input[index] == '"') {
      char *mem = &(input[index]);
      for (;;) {
        if (index >= strlen(input)) {
          fprintf(stderr, "SyntaxError: EOL while scanning string literal");
          exit(1);
        }
        ++index;
        if (input[index] == '\\') {
          index++;
          continue;
        }

        if (input[index] == '"') {
          ++index;
          break;
        }
      }

      Token token = token__new_with_val(
          TOKEN__STRING,
          stringview__new(mem + 1, &(input[index - 1]) - mem - 1));
      cvector__add(tokens, token);
      continue;
    }

    char *mem = &(input[index]);
    size_t len = 0;
    for (;;) {
      if (index >= strlen(input) || input[index] == ' ' ||
          input[index] == '\n' || input[index] == '[' || input[index] == ']' ||
          input[index] == '(' || input[index] == ')' || input[index] == ',' ||
          input[index] == '"' || input[index] == ';' || input[index] == '`' ||
          input[index] == '{' || input[index] == '}' || input[index] == '\'') {
        break;
      }
      ++index;
      ++len;
    }
    if (is_int(mem, len)) {
      cvector__add(tokens,
                   token__new_with_val(TOKEN__INT, stringview__new(mem, len)));
    } else {
      cvector__add(tokens, token__new_with_val(TOKEN__SYMBOL,
                                               stringview__new(mem, len)));
    }
  }
}


char *tokentype__name(TokenType tokentype) {
  switch (tokentype) {
  case TOKEN__COMMENT:
    return "TOKEN__COMMENT";
  case TOKEN__SYMBOL:
    return "TOKEN__SYMBOL";
  case TOKEN__INT:
    return "TOKEN__INT";
  case TOKEN__STRING:
    return "TOKEN__STRING";
  case TOKEN__SEMICOLON:
    return "TOKEN__SEMICOLON";
  case TOKEN__OTHER:
    return "TOKEN__OTHER";
  case TOKEN__TILDA_AT:
    return "TOKEN__TILDA_AT";
  case TOKEN__AT:
    return "TOKEN__AT";
  case TOKEN__LEFT_SQ_BR:
    return "TOKEN__LEFT_SQ_BR";
  case TOKEN__RIGHT_SQ_BR:
    return "TOKEN__RIGHT_SQ_BR";
  case TOKEN__LEFT_CURLY_BR:
    return "TOKEN__LEFT_CURLY_BR";
  case TOKEN__RIGHT_CURLY_BR:
    return "TOKEN__RIGHT_CURLY_BR";
  case TOKEN__LEFT_PAREN:
    return "TOKEN__LEFT_PAREN";
  case TOKEN__RIGHT_PAREN:
    return "TOKEN__RIGHT_PAREN";
  case TOKEN__APOSTROPHE:
    return "TOKEN__APOSTROPHE";
  case TOKEN__WEIRD:
    return "TOKEN__WEIRD";
  case TOKEN__TILDA:
    return "TOKEN__TILDA";
  case TOKEN__CARRAT:
    return "TOKEN__CARRAT";
  }
}

/* Token Helpers */
bool token__is_def(const Token *token) {
  if (token->stringview.len != 4) {
    return false;
  }

  return strncmp(token->stringview.mem, "def!", 4) == 0;
}

bool token__is_let(const Token *token) {
  if (token->stringview.len != 4) {
    return false;
  }

  return strncmp(token->stringview.mem, "let*", 4) == 0;
}
