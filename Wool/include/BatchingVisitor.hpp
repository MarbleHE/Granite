//
// Created by mario on 08.03.20.
//

#ifndef GRANITE_BATCHINGVISITOR_HPP
#define GRANITE_BATCHINGVISITOR_HPP

#include "Visitor.h"
#include "LiteralVector.hpp"
#include "LiteralBool.h"
#include "LiteralInt.h"
#include "LiteralString.h"
#include "LiteralFloat.h"
#include "LogicalExpr.h"
#include "UnaryExpr.h"
#include "AbstractBinaryExpr.h"
#include "ArithmeticExpr.h"

class BatchingVisitor : Visitor {
private:
    int maxSlots = 1;
public:
    void visit(LiteralVector &elem);

    void visit(AbstractExpr &elem);

    void visit(LiteralBool &elem);

    void visit(LiteralInt &elem);

    void visit(LiteralString &elem);

    void visit(LiteralFloat &elem);

    void visit(LogicalExpr &elem);

    void visit(UnaryExpr &elem);

    void visit(ArithmeticExpr &elem);

    int getMaxSlots();
};


#endif //GRANITE_BATCHINGVISITOR_HPP
