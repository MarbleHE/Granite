//
// Created by Mario Stöckli on 04.04.20.
//

#ifndef GRANITE_MULTCOUNTVISITOR_H
#define GRANITE_MULTCOUNTVISITOR_H


#include "Visitor.h"

#include "LogicalExpr.h"
#include "UnaryExpr.h"
#include "AbstractBinaryExpr.h"
#include "ArithmeticExpr.h"

class MultCountVisitor : Visitor {
private:
    int multCount = 0;
public:
    int getMultCount();

    void visit(AbstractExpr &elem) override;

    void visit(ArithmeticExpr &elem) override;


};

#endif //GRANITE_MULTCOUNTVISITOR_H
