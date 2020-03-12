//
// Created by mario on 17.02.20.
//

#ifndef GRANITE_WOOL_HPP
#define GRANITE_WOOL_HPP

#include <string>
#include "AbstractExpr.h"
#include "circuit.hpp"
#include "context.hpp"

namespace Wool {
class W;

/// Libraries supported by SHEEP
enum Library { Plaintext, HElib, LP, Palisade, SEALBFV, SEALCKKS, TFHEBool, TFHEInteger };

/// computes string from library enum
/// \param l
/// \return string to corresponding library enum
std::string toString(Wool::Library l);

class W {
 public:
  /// Basic constructor initializes Circuit c
  explicit W(AbstractExpr *ae);

  explicit W(Ast a);

  /// Evaluates Circuit c of instance with library l.
  /// \param l Library with which we want to evaluate.
  /// \return result of evaluation
  long evaluateWith(Library l);

  //TODO getCircuit();

  /// Benchmarks the Circuit  c of with library l.
  /// \return time in milliseconds
  double benchmarkWith(Library l);

  /// Returns multiplicative depth of circuit
  /// \return
  int getMultDepth();

  /// prints the circuit for debugging, etc...
  void printCircuit();
private:
  /// Circuit which was derived from AbstractExpr or AST
  Circuit c;

  /// input to feed into Circuit
  std::vector<long> ptvec;

  /// const input to feed into Circuit
  std::vector<long> cptvec;

  /// Multiplicative depth of circuit c
  int multDepth;

  /// Maximum number of slots required by some batched gate within the circuit
  int maxSlots;

  /// Composes Circuit of AbstractExpr and saves it in c
  /// \param ae AbstractExpr to compose circuit from
  /// \return Circuit
  void composeCircuit(AbstractExpr *ae);

  //TODO: composeCircuit(Ast a);

  /// Evaluates the Circuit c given a SHEEP HE context and integer type.
  /// \tparam genericContext
  /// \tparam intType_t
  /// \return first element is the result and second element is timing information
  template<typename genericContext, typename intType_t>
  std::tuple<std::vector<long>, DurationContainer> eval();

  /// Evaluates the Circuit c given a SHEEP context and int type.
  /// \tparam intType_t
  /// \param ctx
  /// \return first element is te result and the second element is timing information
  template <typename intType_t>
  std::tuple<std::vector<long>, DurationContainer> eval(BaseContext<intType_t> *ctx);

  /// Estimates an upper bound on the number of bits required at decryption.
  /// \return Number of bits required at decryption
  int estimatePlaintextSize();

  /// Generates a Context
  /// \tparam intType plaintext type
  /// \param l Library of context to be generated
  /// \return the cryptographic context
  template <typename intType>
  BaseContext<intType>* generateContext(Library l);
};

};

#endif //GRANITE_WOOL_HPP
