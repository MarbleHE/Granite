//
// Created by mario on 08.03.20.
//

#ifndef GRANITE_BATCHINGVISITOR_HPP
#define GRANITE_BATCHINGVISITOR_HPP

#include "Visitor.h"
#include "AbstractMatrix.h"
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

    int sndMax = 0;
public:
    void visit(AbstractExpr &elem) override;

    void visit(AbstractMatrix &elem) override;

    void visit(LiteralBool &elem) override;

    void visit(LiteralInt &elem) override;

    void visit(LiteralString &elem) override;

    void visit(LiteralFloat &elem) override;

    void visit(LogicalExpr &elem) override;

    void visit(UnaryExpr &elem) override;

    void visit(ArithmeticExpr &elem) override;

    int getMaxSlots();

    int getSndMaxSlots();
};


#endif //GRANITE_BATCHINGVISITOR_HPP
