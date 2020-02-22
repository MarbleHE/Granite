//
// Created by mario on 21.02.20.
//

#include "CircuitCompositionVisitor.hpp"
#include "simple-circuits.hpp"

std::vector<long> CircuitCompositionVisitor::getPtvec() {
    return this->ptvec;
}

Circuit CircuitCompositionVisitor::visit(LiteralBool &elem) {
    throw std::runtime_error("not implemented");
    return Visitor::visit(elem);
}

Circuit CircuitCompositionVisitor::visit(LiteralInt &elem) {
    throw std::runtime_error("not implemented");
    return Visitor::visit(elem);
}

Circuit CircuitCompositionVisitor::visit(LiteralString &elem) {
    throw std::runtime_error("not implemented");
    return Visitor::visit(elem);
}

Circuit CircuitCompositionVisitor::visit(LiteralFloat &elem) {
    throw std::runtime_error("not implemented");
    return Visitor::visit(elem);
}

Circuit CircuitCompositionVisitor::visit(LogicalExpr &elem) {
    throw std::runtime_error("not implemented");
    return Visitor::visit(elem);
}

Circuit CircuitCompositionVisitor::visit(UnaryExpr &elem) {
    throw std::runtime_error("not implemented");
    return Visitor::visit(elem);
}

Circuit CircuitCompositionVisitor::visit(BinaryExpr &elem) {
    throw std::runtime_error("not implemented");
    return Visitor::visit(elem);
}
