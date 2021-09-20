#include <assert.h>
#include "stdio.h"
#include "reader.h"

void cvector_tokens__tokenize(cvector_tokens_t* tokens, char* input) {
  size_t index = 0;

  for(;;) {
    if (index >= strlen(input)) break;

    if (input[index] == ' ' || input[index] == '\n' || input[index] == ',') {
      ++index;
      continue;
    }

    if (input[index] == '(') {
      Token token = {.tokentype=TOKEN__LEFT_PAREN};
      cvector__add(tokens, token);
      ++index;
      continue;
    }

    if (input[index] ==')') {
      Token token = {.tokentype=TOKEN__RIGHT_PAREN};
      cvector__add(tokens, token);
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

      Token token = {.tokentype=TOKEN__COMMENT, .mem=mem, .len=(&(input[index-1]) - mem)+1};
      cvector__add(tokens, token);
      continue;
    }

    if (input[index] == '"') {
      size_t len = 0;
      char* mem = &(input[index]);
      for(;;) {
        if (index >= strlen(input)) {
          fprintf(stderr, "SyntaxError: EOL while scanning string literal");
          exit(1);
        }
        ++index;
        if (input[index] == '\\') {
          // ignore whatever comes next
          ++index;
          continue;
        }

        if (input[index] == '"') {
          ++index;
          break;
        }
      }

      Token token = {.tokentype=TOKEN__STRING, .mem=mem+1, .len=&(input[index-1]) - mem -1};
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
    Token token = {.tokentype=TOKEN__SYMBOL, .mem=mem, .len=len};
    cvector__add(tokens, token);
    continue;
  }
}

Node node_vector__new(cvector_nodes_t* cvector_nodes) {
  Node node = {.nodetype=NODE__VECTOR, .nodeval = {.nodevector = {.mem=cvector_nodes}}};
  return node;
}

Node node_symbol__new(char* mem, size_t len) {
  Node node = {.nodetype=NODE__SYMBOL, .nodeval = {.nodesymbol = {.mem=mem, .len=len}}};
  return node;
}

Node node_comment__new(char* mem, size_t len) {
  Node node = {.nodetype =NODE__COMMENT, .nodeval = {.nodecomment = {.mem=mem, .len=len}}};
  return node;
}

Node node_int__new(int val) {
  Node node = {.nodetype=NODE__INT, .nodeval={.nodeint={.val=val}}};
  return node;
}

Node node_string__new(char* mem, size_t len) {
  Node node = {.nodetype=NODE__STRING, .nodeval={.nodestring = {.mem=mem, .len=len}}};
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
    return node_comment__new(token.mem, token.len);
  }

  if (token.tokentype == TOKEN__STRING) {
    return node_string__new(token.mem, token.len);
  }

  return node_symbol__new(token.mem, token.len);
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
