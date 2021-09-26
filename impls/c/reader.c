#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>

#include "node.h"
#include "reader.h"
#include "token.h"

Node READ(char *expr) { return read_str(expr); }

Node read_list(cvector_iterator_tokens_t *cvector_iterator_tokens) {
  cvector_nodes_t *cvector_nodes = malloc(sizeof(cvector_nodes_t));
  cvector__init(cvector_nodes);

  Node node = node_vector__new(cvector_nodes);
  // skip (
  cvector_iterator__next(cvector_iterator_tokens);

  bool found_closing_paren = false;

  for (;;) {
    if (cvector_iterator__done(cvector_iterator_tokens))
      break;
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


Node read_atom(cvector_iterator_tokens_t *cvector_iterator_tokens) {

  Token token = cvector_iterator__next(cvector_iterator_tokens);
  switch (token.tokentype) {
  case TOKEN__COMMENT:
    return node_comment__new(token.stringview.mem, token.stringview.len);
  case TOKEN__STRING:
    return node_string__new(token.stringview.mem, token.stringview.len);
  case TOKEN__INT:
    return node_int__new(token.stringview.mem, token.stringview.len);
  case TOKEN__SYMBOL:
    if (token__is_def(&token)) {
      return node_symbol__new(token.stringview.mem, token.stringview.len,
                              NODESYMBOL__SPECIAL_DEF_FORM);
    }
    if (token__is_let(&token)) {
      return node_symbol__new(token.stringview.mem, token.stringview.len,
                              NODESYMBOL__SPECIAL_LET_FORM);
    }

    return node_symbol__new(token.stringview.mem, token.stringview.len,
                            NODESYMBOL__NORM_FORM);

  default:
    assert(0 && printf("unreachable"));
  }
}

Node read_form(cvector_iterator_tokens_t *cvector_iterator_tokens) {
  for (;;) {
    if (cvector_iterator__done(cvector_iterator_tokens))
      break;

    Token token = cvector_iterator__peek(cvector_iterator_tokens);

    if (token.tokentype == TOKEN__LEFT_PAREN) {
      return read_list(cvector_iterator_tokens);
    }
    return read_atom(cvector_iterator_tokens);
  }

  return (Node){.nodetype = NODE__EMPTY};
}

Node read_str(char *input) {
  cvector_tokens_t tokens;
  cvector__init(&tokens);

  cvector_tokens__tokenize(&tokens, input);
  cvector_iterator_tokens_t cvector_iterator_tokens;
  cvector_iterator__init(&cvector_iterator_tokens, &tokens);

  Node node = read_form(&cvector_iterator_tokens);
  return node;
}
