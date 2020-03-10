//
// Created by mario on 21.02.20.
//

#include "CircuitCompositionVisitor.hpp"
#include "simple-circuits.hpp"
#include "circuit-util.hpp"
#include "LiteralBool.h"
#include "LiteralInt.h"
#include "LiteralString.h"
#include "LiteralFloat.h"
#include "LogicalExpr.h"
#include "UnaryExpr.h"
#include "AbstractBinaryExpr.h"
#include "ArithmeticExpr.h"
#include "Function.h"

using namespace std;

std::vector<long> CircuitCompositionVisitor::getPtvec() {
  return this->ptvec;
}

std::vector<long> CircuitCompositionVisitor::getCptvec() {
  return this->cptvec;
}

Circuit CircuitCompositionVisitor::getCircuit() {
  if (this->cs.size()!=1) {
    throw std::runtime_error("Circuit stack contains " + std::to_string(cs.size()) + " Circuit(s).");
  }
  return this->cs.top();
}

void CircuitCompositionVisitor::visit(AbstractExpr &elem) {
  elem.accept(*this);
}

void CircuitCompositionVisitor::visit(LiteralBool &elem) {
  cs.push(single_unary_gate_circuit(Gate::Alias));
  throw std::runtime_error("LiteralBool not implemented. Can't feed into vector<long>");
}

void CircuitCompositionVisitor::visit(LiteralInt &elem) {
  cs.push(single_unary_gate_circuit(Gate::Alias));
  ptvec.push_back(elem.getValue());
}

void CircuitCompositionVisitor::visit(LiteralString &elem) {
  cs.push(single_unary_gate_circuit(Gate::Alias));
  throw std::runtime_error("LiteralString not implemented. Can't feed into vector<long>");
}

void CircuitCompositionVisitor::visit(LiteralFloat &elem) {
  cs.push(single_unary_gate_circuit(Gate::Alias));
  ptvec.push_back(elem.getValue());
}

void CircuitCompositionVisitor::visit(LogicalExpr &elem) {
  elem.getLeft()->accept(*this);
  auto l = cs.top();
  cs.pop();
  elem.getRight()->accept(*this);
  auto r = cs.top();
  cs.pop();

  Circuit gateCircuit = toGateCircuit(elem.getOp()->getOperatorSymbol());

  cs.push(seq(par(l, r), gateCircuit));
}

void CircuitCompositionVisitor::visit(UnaryExpr &elem) {
  elem.getRight()->accept(*this);
  auto r = cs.top();
  cs.pop();

  Circuit gateCircuit = toGateCircuit(elem.getOp()->getOperatorSymbol());

  cs.push(seq(r, gateCircuit));
}

void CircuitCompositionVisitor::visit(ArithmeticExpr &elem) {
  elem.getLeft()->accept(*this);
  auto l = cs.top();
  cs.pop();
  elem.getRight()->accept(*this);
  auto r = cs.top();
  cs.pop();

  Circuit gateCircuit = toGateCircuit(elem.getOp()->getOperatorSymbol());

  cs.push(seq(par(l, r), gateCircuit));
}

void CircuitCompositionVisitor::visit(Function &elem){
    vector<FunctionParameter *> params = elem.getParameters();
}

Circuit
CircuitCompositionVisitor::toGateCircuit(
    const OpSymbolVariant &variant) {
  switch (variant.index()) {
    case 0:
      try {
        return binopCircuitMap.at(std::get<ArithmeticOp>(variant));
      }
      catch (const std::out_of_range &e) {
        throw std::runtime_error("Gate not implemented in SHEEP: " +
            OpSymb::getTextRepr(variant)); //TODO: implement Division, Modulo...
      }
    case 1:
      throw std::runtime_error(
          "Gate not implemented in SHEEP: " + OpSymb::getTextRepr(variant)); //TODO: implement logic gates
    case 2:
      switch (std::get<UnaryOp>(variant)) {
        case UnaryOp::negation:return single_unary_gate_circuit(Gate::Negate);
      }
  }
}
