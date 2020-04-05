//
// Created by mario on 08.01.20.
//
#ifndef M_HPP
#define M_HPP

#include <vector>
#include <string>
#include <functional>
#include <OpSymbEnum.h>

#include "AbstractExpr.h"
#include "Wool.hpp"

namespace Granite {
class G;

/// Encrypt does not actually encrypt at runtime, but sets the variables with a constructor
G encrypt(long value, Wool::Library library);

G encrypt(long value);

std::vector<G> encrypt(std::vector<bool> v);

std::vector<G> encrypt(std::vector<int> v);

std::vector<G> encrypt(std::vector<long> v);

G batchEncrypt(std::vector<bool> v);

G batchEncrypt(std::vector<int> v);

/// Evaluates the expression in m according to the selected library (Plaintext default) and returns its result.
long decrypt(G m);

/// Vector decrypt
std::vector<long> decrypt(std::vector<G> mv);

/// Output imitates the return statement in HE functions.
void output(G value);

/// folds vector v with function f
/// \param v
/// \param f
/// \return
G fold(std::vector<G> v, std::function<G(G, G)> f);

/// Component-wise Multiplication for vectors of G (Can be folded later)
/// \param lhs
/// \param rhs
/// \return
std::vector<G> operator*(std::vector<G> lhs, std::vector<G> rhs);

/// Pads an AST to the correct slot size.
/// \param ast
void pad(Ast* ast, Wool::Library l);

void pad(AbstractExpr* ae, Wool::Library l);

class G {
 public:
  /// The method generating an AST from some function written with G classes.
  static Ast *makeAST(std::function<void()> f);

  /// Method for multDepth analysis will (Wrapper for Wool).
  /// The multDepth returned  is ignoring anything previous all decrypt statements and only measure statements on which output depends on directly.
  /// \return Maximum multiplicative depth of circuit composed from AST.
  static int analyse(std::function<void()> f);

  /// Benchmarks the function. Not very accurate due to design issues.
  /// \param f function to be benchmarked
  /// \param l library with which the last expressions on which the output statement depends directly on should be evaluated with.
  /// \return total evaluation time in milliseconds. (encryption and decryption included) has some overhead from G polluting the result.
  static double evaluate(std::function<void()> f, Wool::Library l);

  /// Method to get the result of the computation
  /// \return result of f() decrypted (if it has an output statement)
  static long result(std::function<void()> f, Wool::Library l);

  /// Return statement imitation
  static void output(const G &m);

  /// Destructor;
  ~G();

  /// Dummy contructor
  G();

  /// full constructor for integers
  G(long value, bool plaintext, Wool::Library library);

  /// Copy constructor
  G(const G &other);

  /// Move constructor
  G(G &&other);

  /// Plaintext constructor
  G(long i);

  /// Plaintext constructor
  G(int i);

  /// Plaintext constructor
  G(bool b);

  /// Copy assignment
  G &operator=(const G &other);

  /// Move assignment
  G &operator=(G &&other);

  /// Plaintext encode assignment
  G &operator=(long i);

  /// Encrypt bool
  G &operator=(bool b);

  /// Encrypt int
  G &operator=(int i);

  /// Addition
  G &operator+=(const G &rhs);

  /// Addition
  G &operator+=(const long &rhs);

  /// Addition
  G &operator+=(const int &rhs);

  /// Subtraction
  G &operator-=(const G &rhs);

  /// Subtraction
  G &operator-=(const long &rhs);

  /// Subtraction
  G &operator-=(const int &rhs);

  /// Multiplication
  G &operator*=(const G &rhs);

  /// Multiplication
  G &operator*=(long &rhs);

  /// Multiplication
  G &operator*=(int &rhs);

  /// Increment operator
  G &operator++();

  /// Decrement operator
  G &operator--();

  /// Increment operator
  G operator++(int) {
    auto tmp = *this;
    operator++();
    return tmp;
  }

  /// Decrement operator
  G operator--(int) {
    auto tmp = *this;
    operator--();
    return tmp;
  }

  /// Logical negation operator
  G &operator!();

  /// Equality
  friend G operator==(const G &lhs, const G &rhs);

  /// Inequality
  friend G operator!=(const G &lhs, const G &rhs);

  /// Greater-than-or-equal
  friend G operator>=(const G &lhs, const G &rhs);

  /// Greater-than
  friend G operator>(const G &lhs, const G &rhs);

  /// Smaller-than-or-equal
  friend G operator<=(const G &lhs, const G &rhs);

  /// Smaller-than
  friend G operator<(const G &lhs, const G &rhs);

  /// Addition
  friend G operator+(const G &lhs, const G &rhs);

  /// Subtraction
  friend G operator-(const G &lhs, const G &rhs);

  /// Multiplication
  friend G operator*(const G &lhs, const G &rhs);

  /// Rotates a batched vector by amount k.
  /// \param k Rotation amount. Positive rotates
  /// \return
  void rotate(int k);

  /// \return plaintext bool, true if the value is plaintext
  bool isPlaintext() const;

  /// \return library to be used at evaluation time
  Wool::Library getLib() const;

  /// set Library to be used at evaluation time
  void setLib(Wool::Library l);

  /// \return the pointer to the Expression responsible for its construction
  AbstractExpr *getExpr();

  /// \return Size of the current expression (batched vector)
  int getExprSize();

  G &fold(std::function<G(G, G)> f);

  static G sum(G, G);

private:
  /// This is the (temporary) Ast used by output(G m) to return the final AST to makeAST
  static Ast *output_ast;

  /// a pointer to the AST responsible for its construction
  AbstractExpr *expr;

  /// Library to use when evaluating
  Wool::Library library;

  /// true, if value is a plaintext
  bool plaintext;

  /// Size of the current expression (batched vector)
  int exprSize;

  /// Direct init with all fields
  G(AbstractExpr *ae, bool plaintext, Wool::Library library, int expressionSize);

  /// \return true, if the library is well suited for int
  bool isWellSuited(Wool::Library l, int i);

  /// \return true, if the library is well suited for long
  bool isWellSuited(Wool::Library l, long x);

  /// \return true, if the library is well suited for bool
  bool isWellSuited(Wool::Library l, bool b);

  /// This function tries to resolve user selected library choices and default libraries s.t. a user selection "wins" over default libraries
  /// and a runtime error is thrown, if there are contradicting constraints.
  /// Default library is Plaintext.
  /// \param l
  /// \param r
  /// \return the library, which "won"
  static Wool::Library resolveLibraries(Wool::Library l, Wool::Library r);

  /// Rotates a batched vector by amount k.
  /// \param k Rotation amount. Positive rotates
  /// \return
  friend G rotate(G m, int k);

  friend G encrypt(long value, Wool::Library library);

  friend  G batchEncrypt(std::vector<bool> v);

  friend  G batchEncrypt(std::vector<int> v);

  friend Ast make_AST(std::function<void()> f);

};

};

#endif //M_HPP
