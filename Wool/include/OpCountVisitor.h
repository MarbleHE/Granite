//
// Created by Mario Stöckli on 03.04.20.
//

#ifndef GRANITE_OPCOUNTVISITOR_H
#define GRANITE_OPCOUNTVISITOR_H

#include "Visitor.h"

#include "LogicalExpr.h"
#include "UnaryExpr.h"
#include "AbstractBinaryExpr.h"
#include "ArithmeticExpr.h"

class OpCountVisitor : Visitor {
private:
    int opcount;
public:
    int getOpCount();

    void visit(AbstractExpr &elem) override;

    void visit(LogicalExpr &elem) override;

    void visit(UnaryExpr &elem) override;

    void visit(ArithmeticExpr &elem) override;


};


#endif //GRANITE_OPCOUNTVISITOR_H
