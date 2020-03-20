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
#include "Dimension.h"
#include "CircuitHelpers.hpp"
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

void CircuitCompositionVisitor::visit(AbstractMatrix &elem) {
    if (dynamic_cast<Matrix<int>*> (&elem)) {
        vector<long> v;
        for (auto i = 0; i < elem.getDimensions().numColumns; i++) {
            auto el = (LiteralInt*) elem.getElementAt(0,i);
            v.push_back((long) el->getValue());
        }
        ptvec.push_back(v);
        cs.push(single_unary_gate_circuit(Gate::Alias));
    } else if(dynamic_cast<Matrix<LiteralInt>*> (&elem)) {
        vector<long> v;
        for (auto i = 0; i < elem.getDimensions().numColumns; i++) {
            auto matLit = elem.getElementAt(0,i);
            v.push_back(((LiteralInt*) matLit)->getValue());
        }
        ptvec.push_back(v);
        cs.push(single_unary_gate_circuit(Gate::Alias));
    } else if(dynamic_cast<Matrix<AbstractExpr*>*> (&elem)) {
        for (auto i = 0; i < elem.getDimensions().numColumns; i++) {
            elem.getElementAt(0, i)->accept(*this);
        }
    }
    //TODO bool, etc. support
}


void CircuitCompositionVisitor::visit(Rotate &elem){
    elem.getOperand()->accept(*this);
    auto c = seq(cs.top(),rotateCircuit());
    cs.pop();
    cs.push(c);
    cptvec.push_back(((LiteralInt* ) elem.getRotationFactor())->getValue());
}

void CircuitCompositionVisitor::visit(LiteralBool &elem) {
  throw std::runtime_error("LiteralBool not implemented. Can't feed into vector<vector<long>>");
  cs.push(single_unary_gate_circuit(Gate::Alias));
}

void CircuitCompositionVisitor::visit(LiteralInt &elem) {
    auto mat = elem.getMatrix();
    mat->accept(*this);
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
    throw std::runtime_error("not implemented");
    //TODO: setup mapping between variables and inputs to support variables.
    //Problem: currently constants in a circuit are fed into a vector. How do I leave empty spaces in a vector to feed in the values
    // of the variables later? This either requires AddConst, etc. gates... (don't support batching), or some other solution which
    // these mappings until evaluation.
    // Solution: variant vector
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
        switch (std::get<LogCompOp>(variant)) {
            case LogCompOp::GREATER:
                return single_binary_gate_circuit(Gate::Compare);
            case LogCompOp::GREATER_EQUAL:
                return greaterEqualCircuit();
            case LogCompOp::SMALLER_EQUAL:
                return smallerEqualCircuit();
            case LogCompOp::SMALLER:
                return smallerCircuit();
            case LogCompOp::EQUAL:
                return equalCircuit();
            case LogCompOp::UNEQUAL:
                return unequalCircuit();
            case LogCompOp::LOGICAL_OR:
                return single_binary_gate_circuit(Gate::Add); //TODO: Might not work?
            case LogCompOp::LOGICAL_AND:
                return single_binary_gate_circuit(Gate::Multiply); //TODO: Might not work?
            case LogCompOp::LOGICAL_XOR:
                return unequalCircuit();
            default:
                throw std::runtime_error(
                        "Gate not implemented in SHEEP: " + OpSymb::getTextRepr(variant));
        }
    case 2:
      switch (std::get<UnaryOp>(variant)) {
        case UnaryOp::NEGATION:return single_unary_gate_circuit(Gate::Negate);
      }
  }
}
