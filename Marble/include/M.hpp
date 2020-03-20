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

namespace Marble {
class M;

/// Encrypt does not actually encrypt at runtime, but sets the variables with a constructor
M encrypt(long value, Wool::Library library);

M encrypt(long value);

std::vector<M> encrypt(std::vector<bool> v);

std::vector<M> encrypt(std::vector<int> v);

std::vector<M> encrypt(std::vector<long> v);

M batchEncrypt(std::vector<bool> v);

M batchEncrypt(std::vector<int> v);

/// Evaluates the expression in m according to the selected library (Plaintext default) and returns its result.
long decrypt(M m);

/// Vector decrypt
std::vector<long> decrypt(std::vector<M> mv);

/// Output imitates the return statement in HE functions.
void output(M value);

/// Pads an AST to the correct slot size.
/// \param ast
void pad(Ast* ast, Wool::Library l);

void pad(AbstractExpr* ae, Wool::Library l);

class M {
 public:
  /// The method generating an AST from some function written with M classes.
  static Ast *makeAST(std::function<void()> f);

  /// Method for multDepth analysis will (Wrapper for Wool).
  /// The multDepth returned  is ignoring anything previous all decrypt statements and only measure statements on which output depends on directly.
  /// \return Maximum multiplicative depth of circuit composed from AST.
  static int analyse(std::function<void()> f);

  /// Benchmarks the function. Not very accurate due to design issues.
  /// \param f function to be benchmarked
  /// \param l library with which the last expressions on which the output statement depends directly on should be evaluated with.
  /// \return total evaluation time in milliseconds. (encryption and decryption included) has some overhead from M polluting the result.
  static double evaluate(std::function<void()> f, Wool::Library l);

  /// Method to get the result of the computation
  /// \return result of f() decrypted (if it has an output statement)
  static long result(std::function<void()> f, Wool::Library l);

  /// Return statement imitation
  static void output(const M &m);

  /// Destructor;
  ~M();

  /// Dummy contructor
  M();

  /// full constructor for integers
  M(long value, bool plaintext, Wool::Library library);

  /// Copy constructor
  M(const M &other);

  /// Move constructor
  M(M &&other);

  /// Plaintext constructor
  M(long i);

  /// Plaintext constructor
  M(int i);

  /// Plaintext constructor
  M(bool b);

  /// Copy assignment
  M &operator=(const M &other);

  /// Move assignment
  M &operator=(M &&other);

  /// Plaintext encode assignment
  M &operator=(long i);

  /// Encrypt bool
  M &operator=(bool b);

  /// Encrypt int
  M &operator=(int i);

  /// Addition
  M &operator+=(const M &rhs);

  /// Addition
  M &operator+=(const long &rhs);

  /// Addition
  M &operator+=(const int &rhs);

  /// Subtraction
  M &operator-=(const M &rhs);

  /// Subtraction
  M &operator-=(const long &rhs);

  /// Subtraction
  M &operator-=(const int &rhs);

  /// Multiplication
  M &operator*=(const M &rhs);

  /// Multiplication
  M &operator*=(long &rhs);

  /// Multiplication
  M &operator*=(int &rhs);

  /// Increment operator
  M &operator++();

  /// Decrement operator
  M &operator--();

  /// Increment operator
  M operator++(int) {
    auto tmp = *this;
    operator++();
    return tmp;
  }

  /// Decrement operator
  M operator--(int) {
    auto tmp = *this;
    operator--();
    return tmp;
  }

  /// Logical negation operator
  M &operator!();

  /// Equality
  friend M operator==(const M &lhs, const M &rhs);

  /// Inequality
  friend M operator!=(const M &lhs, const M &rhs);

  /// Greater-than-or-equal
  friend M operator>=(const M &lhs, const M &rhs);

  /// Greater-than
  friend M operator>(const M &lhs, const M &rhs);

  /// Smaller-than-or-equal
  friend M operator<=(const M &lhs, const M &rhs);

  /// Smaller-than
  friend M operator<(const M &lhs, const M &rhs);

  /// Addition
  friend M operator+(const M &lhs, const M &rhs);

  /// Subtraction
  friend M operator-(const M &lhs, const M &rhs);

  /// Multiplication
  friend M operator*(const M &lhs, const M &rhs);

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

  M &fold(std::function<M(M, M)> f);

  static M sum(M, M);

private:
  /// This is the (temporary) Ast used by output(M m) to return the final AST to makeAST
  static Ast *output_ast;

  /// a pointer to the AST responsible for its construction
  AbstractExpr *expr;

  /// Library to use when evaluating
  Wool::Library library;

  /// true, if value is a plaintext
  bool plaintext;

  /// Size of the current expression (batched vector)
  int exprSize;

  /// Direct init with AST
  M(AbstractExpr *expr, bool plaintext);

  /// Direct init with all fields
  M(AbstractExpr *ae, bool plaintext, Wool::Library library, int expressionSize);

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
  friend M rotate(M m, int k);

  friend M encrypt(long value, Wool::Library library);

  friend  M batchEncrypt(std::vector<bool> v);

  friend  M batchEncrypt(std::vector<int> v);

  friend Ast make_AST(std::function<void()> f);

};

};

#endif //M_HPP
