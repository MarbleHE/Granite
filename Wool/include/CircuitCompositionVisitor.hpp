//
// Created by mario on 21.02.20.
//

#ifndef GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP
#define GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP

#include <vector>
#include <stack>
#include <variant>
#include <map>
#include "Visitor.h"
#include "circuit.hpp"
#include "OpSymbEnum.h"
#include "simple-circuits.hpp"
#include "AbstractExpr.h"
#include "AbstractBinaryExpr.h"
#include "AbstractMatrix.h"
#include "Rotate.h"
#include "Matrix.h"

class CircuitCompositionVisitor : Visitor {
 private:
  /// plaintext vector
  std::vector<std::vector<long>> ptvec;

  /// plaintext vector of constants
  std::vector<long> cptvec;

  /// Stack of circuits, which gets gradually built until only the final Circuit is left.
  std::stack<Circuit> cs;

  /// converts an OpSymb to the corresponding SHEEP gate
  Circuit toGateCircuit(const OpSymbolVariant &variant);

  /// Mapping between OpSymb::BinaryOp from ast_lib and Gate from sheep
  const std::map<ArithmeticOp, Circuit> binopCircuitMap = {
      {ArithmeticOp::ADDITION, single_binary_gate_circuit(Gate::Add)},
      {ArithmeticOp::SUBTRACTION, single_binary_gate_circuit((Gate::Subtract))},
      {ArithmeticOp::MULTIPLICATION, single_binary_gate_circuit(Gate::Multiply)}
  };

  //TODO: use map<String, Circuit*> to optimize partially duplicate Circuits
 public:
  std::vector<std::vector<long>> getPtvec();

  std::vector<long> getCptvec();

  void visit(AbstractExpr &elem) override;

  void visit(AbstractMatrix &elem) override;

  void visit(Rotate &elem) override;

  void visit(LiteralBool &elem) override;

  void visit(LiteralInt &elem) override;

  void visit(LiteralString &elem) override;

  void visit(LiteralFloat &elem) override;

  void visit(LogicalExpr &elem) override;

  void visit(UnaryExpr &elem) override;

  void visit(ArithmeticExpr &elem) override;

  void visit(Function &elem) override;

  Circuit getCircuit();

};

#endif //GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP
