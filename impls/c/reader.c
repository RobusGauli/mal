#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <inttypes.h>

#include "reader.h"
#include "token.h"


bool is_valid_integer(char* mem, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (mem[i] < 48 || mem[i] > 57) {
      return false;
    }
  }
  return true;
}

void cvector_tokens__tokenize(cvector_tokens_t* tokens, char* input) {
  size_t index = 0;

  for(;;) {
    if (index >= strlen(input)) break;

    if (input[index] == ' ' || input[index] == '\n' || input[index] == ',' || input[index] == '\r') {
      ++index;
      continue;
    }

    if (input[index] == '(') {
      cvector__add(tokens, token__new(TOKEN__LEFT_PAREN));
      ++index;
      continue;
    }

    if (input[index] ==')') {
      cvector__add(tokens,token__new(TOKEN__RIGHT_PAREN));
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
      char* mem = &(input[index]);
      for (;;) {
        if (index >= strlen(input) || input[index] == '\n') {
          break;
        }
        ++index;
      }

      Token token = token__new_with_val(TOKEN__COMMENT, stringview__new(mem, ((&(input[index-1]) - mem + 1))));
      cvector__add(tokens, token);
      continue;
    }

    if (input[index] == '"') {
      char* mem = &(input[index]);
      for(;;) {
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
          stringview__new(mem+1, &(input[index-1]) - mem - 1)
          );
      cvector__add(tokens, token);
      continue;
    }

    char* mem = &(input[index]);
    size_t len=0;
    for(;;) {
      if (index >= strlen(input)
          || input[index] == ' '
          || input[index] == '\n'
          || input[index] == '['
          || input[index] == ']'
          || input[index] == '('
          || input[index] == ')'
          || input[index] == ','
          || input[index] == '"'
          || input[index] == ';'
          || input[index] == '`'
          || input[index] == '{'
          || input[index] == '}'
          || input[index] == '\''
         ) {
        break;
      }
      ++index;
      ++len;
    }
    if (is_valid_integer(mem, len)) {
      cvector__add(tokens, token__new_with_val(TOKEN__INT, stringview__new(mem, len)));
    } else {
      cvector__add(tokens, token__new_with_val(TOKEN__SYMBOL, stringview__new(mem, len)));
    }

    }
}

Node node_vector__new(cvector_nodes_t* cvector_nodes) {
  Node node = {.nodetype=NODE__VECTOR, .nodeval = {.nodevector = {.mem=cvector_nodes}}};
  return node;
}

Node node_symbol__new(char* mem, size_t len) {
  return (Node){
    .nodetype = NODE__SYMBOL,
      .nodeval = {
        .nodesymbol = {
          .stringview = {
            .mem = mem,
            .len = len
          }
        }
      }
  };
}

Node node_comment__new(char* mem, size_t len) {
  return (Node) {
    .nodetype = NODE__COMMENT,
      .nodeval = {
        .nodecomment = {
          .stringview = {
            .mem = mem,
            .len = len
          }
        }
      }
  };
}

Node node_int__new(char* mem, size_t len) {
  // convert that thint to actual int
  char* endptr = mem+len-1;
  // converto to int
  int num = strtoumax(mem, &endptr, 10);
  Node node = {.nodetype=NODE__INT, .nodeval={.nodeint={.val=num}}};
  return node;
}

Node node_eof__new(void) {
  Node node = {.nodetype=NODE__EOF};
  return node;
}

Node node_string__new(char* mem, size_t len) {
  cvector_chars_t cvector_chars;
  cvector__init(&cvector_chars);

  size_t index = 0;
  while(index < len) {
    char ch = mem[index++];
    if (ch == '\\') {
      if (index >= len) {
        continue;
      }
      ch = mem[index];
      cvector__add(&cvector_chars, ch);
      ++index;
      continue;
    }
    cvector__add(&cvector_chars, ch);
  }
  Node node = {.nodetype=NODE__STRING,
    .nodeval={.nodestring = {.cvector_chars=cvector_chars}}};

  return node;
}

Node read_list(cvector_iterator_tokens_t* cvector_iterator_tokens) {
  cvector_nodes_t* cvector_nodes = malloc(sizeof(cvector_nodes_t));
  cvector__init(cvector_nodes);

  Node node = node_vector__new(cvector_nodes);
  // skip (
  cvector_iterator__next(cvector_iterator_tokens);

  bool found_closing_paren = false;

  for (;;) {
    if (cvector_iterator__done(cvector_iterator_tokens)) break;
    Token token = cvector_iterator__peek(cvector_iterator_tokens);
    if (token.tokentype == TOKEN__RIGHT_PAREN) {
      found_closing_paren = true;
      cvector_iterator__next(cvector_iterator_tokens);
      break;
    }
    cvector__add(cvector_nodes, read_form(cvector_iterator_tokens));
  }

  if (!found_closing_paren) {
    fprintf(stderr, "SyntaxError: missing ')'");
    exit(1);
  }

  return node;
}

Node read_atom(cvector_iterator_tokens_t* cvector_iterator_tokens) {
  Token token = cvector_iterator__next(cvector_iterator_tokens);
  if (token.tokentype == TOKEN__COMMENT) {
    return node_comment__new(token.stringview.mem, token.stringview.len);
  }

  if (token.tokentype == TOKEN__STRING) {
    return node_string__new(token.stringview.mem, token.stringview.len);
  }

  if (token.tokentype == TOKEN__INT) {
    return node_int__new(token.stringview.mem, token.stringview.len);
  }

  return node_symbol__new(token.stringview.mem, token.stringview.len);
}

Node read_form(cvector_iterator_tokens_t* cvector_iterator_tokens) {
  for (;;) {
    if (cvector_iterator__done(cvector_iterator_tokens)) break;

    Token token = cvector_iterator__peek(cvector_iterator_tokens);

    if (token.tokentype == TOKEN__LEFT_PAREN) {
      return read_list(cvector_iterator_tokens);
    }

    return read_atom(cvector_iterator_tokens);
  }

  return ( Node ){.nodetype=NODE__EMPTY};
}

Node read_str(char* input) {
  cvector_tokens_t tokens;
  cvector__init(&tokens);

  cvector_tokens__tokenize(&tokens, input);
  cvector_iterator_tokens_t cvector_iterator_tokens;
  cvector_iterator__init(&cvector_iterator_tokens, &tokens);

  Node node = read_form(&cvector_iterator_tokens);
  return node;
}
