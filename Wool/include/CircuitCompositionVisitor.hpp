//
// Created by mario on 21.02.20.
//

#ifndef GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP
#define GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP

#include "Visitor.h"

class CircuitCompositionVisitor: Visitor {
private:
    std::vector<long> ptvec;
public:
    std::vector<long> getPtvec();

    Circuit visit(LiteralBool &elem) override;
    Circuit visit(LiteralInt &elem) override;
    Circuit visit(LiteralString &elem) override;
    Circuit visit(LiteralFloat &elem) override;
    Circuit visit(LogicalExpr &elem) override;
    Circuit visit(UnaryExpr &elem) override;
    Circuit visit(BinaryExpr &elem) override;
};


#endif //GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP
