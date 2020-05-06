//
// Created by mario on 08.01.20.
//
/// The namespace Granite and class G provide a simple interface for the evaluation and analysis of a FHE function
/// A user can provide a function, written with the type G. This has the effect, that using the overloaded operators,
/// the instances can pass an AST, as the function is being executed.
/// This then results in a bottom-up construction of the unrolled AST of the function, which can be used by the library Wool for further use.
/// An example can be found in the examples folder, e.g. the hd_enc example.
///
/// The namespace mainly consists of convenience functions, which exist to provide a functional interface for G.
/// The class implements the public interface of overloaded operators.
/// It also contains the methods serving as a wrapper for the Wool methods, which are responsible for actual execution and benchmarking of the code.


#ifndef M_HPP
#define M_HPP

#include <vector>
#include <string>
#include <functional>

#include "Wool.hpp"
#include "ast_opt/ast/AbstractExpr.h"
#include "ast_opt/ast/OpSymbEnum.h"

namespace Granite {
class G;

/// Initializes an instance of G with a LiteralInt AST and marks it as encrypted with the given library.
/// \param value The value to be encrypted.
/// \param library The library to be used for a later decrypt(G) call
/// \return The initialized instance..
G encrypt(long value, Wool::Library library);

/// Initializes an instance of G with a LiteralInt AST and marks it as encrypted with a library to be specified at a later time.
/// \param value The value to be encrypted.
/// \return The initialized instance.
G encrypt(long value);

/// Initializes a whole vector of instances of G with an AST consisting of a Literal and marks it as encrypted with a library to be specified at a later time.
/// \param v The vector of values to be encrypted separately. (This is NOT batching)
/// \return A vector of the instances of G.
std::vector<G> encrypt(std::vector<bool> v);

/// Initializes a whole vector of instances of G with an AST consisting of a Literal and marks it as encrypted with a library to be specified at a later time.
/// \param v The vector of values to be encrypted separately. (This is NOT batching)
/// \return A vector of the instances of G.
std::vector<G> encrypt(std::vector<int> v);

/// Initializes a whole vector of instances of G with an AST consisting of a Literal and marks it as encrypted with a library to be specified at a later time.
/// \param v The vector of values to be encrypted separately. (This is NOT batching)
/// \return A vector of the instances of G.
std::vector<G> encrypt(std::vector<long> v);

/// Initializes an instance of G with an AST consisting of a Literal containing a one-dimensional matrix.
/// This can be used by Wool to support batching.
/// The instance is marked as encrypted with a library to be specified at a later time.
/// \param v The vector of values to be encrypted.
/// \return An instance of G
G batchEncrypt(std::vector<bool> v);

/// Initializes an instance of G with an AST consisting of a Literal containing a one-dimensional matrix.
/// This can be used by Wool to support batching.
/// The instance is marked as encrypted with a library to be specified at a later time.
/// \param v The vector of values to be encrypted.
/// \return An instance of G
G batchEncrypt(std::vector<int> v);

/// Triggers the encryption, computation and decryption of the expression stored in m.
/// The context to be used is determined by the library stored in m. (Plaintext default)
/// Use this only to support mixed computations within a function. Use output otherwise.
long decrypt(G m);

/// Triggers the encryption, computation and decryption of the expression stored in m.
/// The context to be used is determined by the library stored in m. (Plaintext default)
/// Use this only to support mixed computations within a function. Use output otherwise.
std::vector<long> decrypt(std::vector<G> mv);

/// Output imitates the return statement in HE functions. HE functions without output are undefined.
/// Legacy interface support.
void output(G value);

/// Folds vector v with function f using batching
/// \param v
/// \param f
/// \return An instance of G containing the AST produced by the fold.
G fold(std::vector<G> v, std::function<G(G, G)> f);

/// Component-wise Multiplication for vectors of G (Can be folded later)
/// \param lhs
/// \param rhs
/// \return Vector of instances containing the AST of lhs[i]*rhs[i]
std::vector<G> operator*(std::vector<G> lhs, std::vector<G> rhs);

/// Component-wise Subtraction for vectors of G (Can be folded later)
/// \param lhs
/// \param rhs
/// \return Vector of instances containing the AST of lhs[i]-rhs[i]
std::vector<G> operator-(std::vector<G> lhs, std::vector<G> rhs);

/// Component-wise Subtraction for vectors of G from a Vector initialized with the same value (Can be folded later)
/// \param i The value of which the left vector should consist of
/// \param rhs
/// \return Vector of instances containing the AST of i-rhs[k]
std::vector<G> operator-(long i, std::vector<G> rhs);

/// DEPRECATED
/// Returns two the padded versions of lhs and rhs for batching.
/// (Constants in the expressions of lhs or rhs get padded to a vector)
/// \param lhs
/// \param rhs
/// \return <lhsPadded, rhsPadded>
std::tuple<G,G> pad(const G &lhs, const G &rhs);

/// DEPRECATED
/// Pads an AST to the correct slot size.
/// \param ast
void pad(Ast* ast, Wool::Library l);

/// DEPRECATED
void pad(AbstractExpr* ae, Wool::Library l);

class G {
 public:
  /// The method generating an AST from some function f written with G classes.
  static Ast *makeAST(std::function<void()> f);

  /// Method for multDepth analysis will (Wrapper for Wool).
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

  /// Fold function to use after batching
  /// \param f The function to combine values
  /// \return An instance of G containing the AST after folding this with f
  G &fold(std::function<G(G, G)> f);

  /// Convenience wrapper for + to use in folds.
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

    /// Initializes an instance of G with a LiteralInt AST and marks it as encrypted with the given library.
    /// \param value The value to be encrypted.
    /// \param library The library to be used for a later decrypt(G) call
    /// \return The initialized instance..
  friend G encrypt(long value, Wool::Library library);

  /// Initializes an instance of G with an AST consisting of a Literal containing a one-dimensional matrix.
  /// This can be used by Wool to support batching.
  /// The instance is marked as encrypted with a library to be specified at a later time.
  /// \param v The vector of values to be encrypted.
  /// \return An instance of G
  friend  G batchEncrypt(std::vector<bool> v);

    /// Initializes an instance of G with an AST consisting of a Literal containing a one-dimensional matrix.
    /// This can be used by Wool to support batching.
    /// The instance is marked as encrypted with a library to be specified at a later time.
    /// \param v The vector of values to be encrypted.
    /// \return An instance of G
  friend  G batchEncrypt(std::vector<int> v);

    /// The method generating an AST from some function f written with G classes.
    friend Ast make_AST(std::function<void()> f);

    ///DEPRECATED
  friend std::tuple<G,G> pad(const G &lhs, const G &rhs);
};

};

#endif //M_HPP
