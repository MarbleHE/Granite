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
#include "AbstractMatrix.h"
#include "Rotate.h"
#include "GetMatrixElement.h"
#include "Matrix.h"

using namespace std;

std::vector<std::vector<long>> CircuitCompositionVisitor::getPtvec() {
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

void CircuitCompositionVisitor::visit(AbstractMatrix &elem){
  //nothing to do. we already handle matrices in the literals...
  throw runtime_error("AbstractMatrix CircuitCompositionVisitor not implemented");
}

void CircuitCompositionVisitor::visit(Rotate &elem){
    auto c = seq(cs.top(),single_binary_gate_circuit(Gate::Rotate)); //TODO: verify that const gets fed in correctly from cptvec and not from ptvec
    cs.pop();
    cs.push(c);
    cptvec.push_back(((LiteralInt* ) elem.getRotationFactor())->getValue()); //TODO: verify that rotation amount is in constants input.
}

void CircuitCompositionVisitor::visit(LiteralBool &elem) {
  throw std::runtime_error("LiteralBool not implemented. Can't feed into vector<vector<long>>");
  cs.push(single_unary_gate_circuit(Gate::Alias));
}

void CircuitCompositionVisitor::visit(LiteralInt &elem) {
  cs.push(single_unary_gate_circuit(Gate::Alias));
    try {
        ptvec.push_back({elem.getValue()});
    }
    catch (const std::logic_error &e){
        auto ae = (AbstractExpr*) elem.getMatrix();
        vector<long> v;
        for (auto i = 0; i < elem.getMatrix()->getDimensions().numColumns; i++){
            GetMatrixElement gme = GetMatrixElement(ae, 0, i);
            LiteralInt* li = (LiteralInt *) gme.getOperand();
            v.push_back(li->getValue());
        }
            ptvec.push_back(v);
    }
}

void CircuitCompositionVisitor::visit(LiteralString &elem) {
  throw std::runtime_error("LiteralString not implemented. Can't feed into vector<vector<long>>");
  cs.push(single_unary_gate_circuit(Gate::Alias));
}

void CircuitCompositionVisitor::visit(LiteralFloat &elem) {
  throw std::runtime_error("LiteralFloat not implemented. Can't feed into vector<vector<long>>");
  cs.push(single_unary_gate_circuit(Gate::Alias));
  ptvec.push_back({(long) elem.getValue()});
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
        case UnaryOp::NEGATION:return single_unary_gate_circuit(Gate::Negate);
      }
  }
}
