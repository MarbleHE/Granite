//
// Created by mario on 21.02.20.
//

#ifndef GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP
#define GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP

#include <vector>
#include <stack>
#include <variant>
#include <map>
#include <context.hpp>
#include "Visitor.h"
#include "circuit.hpp"
#include "OpSymbEnum.h"
#include "simple-circuits.hpp"
#include "circuit-util.hpp"
#include "AbstractExpr.h"
#include "AbstractBinaryExpr.h"

class CircuitCompositionVisitor : Visitor {
 private:
  /// plaintext vector
  std::vector<long> ptvec;

  /// plaintext vector of constants
  std::vector<long> cptvec;

  /// Stack of circuits, which gets gradually built until only the final Circuit is left.
  std::stack<Circuit> cs;

  /// converts an OpSymb to the corresponding SHEEP gate
  Circuit toGateCircuit(const OpSymbolVariant &variant);

  /// Mapping between OpSymb::BinaryOp from ast_lib and Gate from sheep
  const std::map<ArithmeticOp, Circuit> binopCircuitMap = {
      {ArithmeticOp::addition, single_binary_gate_circuit(Gate::Add)},
      {ArithmeticOp::subtraction, single_binary_gate_circuit((Gate::Subtract))},
      {ArithmeticOp::multiplication, single_binary_gate_circuit(Gate::Multiply)}
  };
 public:
  std::vector<long> getPtvec();

  std::vector<long> getCptvec();

  void visit(AbstractExpr &elem) override;

  void visit(LiteralBool &elem) override;

  void visit(LiteralInt &elem) override;

  void visit(LiteralString &elem) override;

  void visit(LiteralFloat &elem) override;

  void visit(LogicalExpr &elem) override;

  void visit(UnaryExpr &elem) override;

  void visit(ArithmeticExpr &elem) override;

  void visit(Function &elem);

  Circuit getCircuit();

};

#endif //GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP
