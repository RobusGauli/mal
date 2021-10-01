# LISP grammer

## Introduction to symbol expressions

* LISP programs are represented by s-expressions. Important aspect is both the program and data are represented by s-expression.

* I am not sure how it is powerful but will find out sometime in future. Finger crossed

* S-expressions contains one of the two things:
  a. Atom
  b. List

* Atom are smallest units of the LISP. It literraly like "atom(in your chemistry, physics)".
  **1. Atom in LISP are symbol, number, string, double, nil,

  2. symbol are special type of atom that is bound to S-expression/value.
  3. If the symbol is unbound, then evaluation of that unbounded symbol results in error.
* List is a sequence of atoms or other lists separated by blanks and eclosed in parentheses.
  (1 2 3 4)
  (name (call ram))
  (or (eats ram cake) (eats ram apple))
  (and (plays football) (plays basketball))
  ()
  NOTE: Nesting can be arbitary.

* Empty List "()" plays special role in construction and manipulation of Lisp data structures and given a special name nil. *nil* is the only s-expression that is both atom and list.

* When they mean, List in Lisp can represent both program and data. Is it something like this:
  1. As a Program:
    Sample s-expression: (+ 3 4), (+ (* 3 4) 5)
    These s-expression represents sometype of arithmetic operation against numbers(atoms). I think (but not sure) these arithmetic operations representation is called program. (f a b) => f(a, b), where f is the function applied to arguments a & b.

  2. As a Data:
    Sample s-expression: (list 1 23 4 ) (list (3 4 5 (list 4 5)))
    These s-expression allows us to construct vector of elements. In this case, we are defining data as s-expression. I think (not sure though) this type of s-expression that allows us to represent list of things is called data. Who knows?? meh!


### Formal definition of s-expression
An s-expression is defined recursively:
  An atom is s-expression. Let's consider atoms from 1 to n as
  s1, s2,... sn as s-expression, then so is the list of (s1, s2, ... sn).
  A list is non-atomic s-expression.
  A form is an s-expression that is intended to be evaluated.
  Here is how form gets evaluated.
    a. If s-expression is list, the first element is treated as a function and the subsequent elements are evaluated to obtain the function arguments.
    b. If s-expression is number, return the value of the number,
    c. If s-expression is symbol, return the value bound to that symbol.If the symbol is unbounded during evaluation, that is an error.
    d. If 


