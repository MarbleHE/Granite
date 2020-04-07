//
// Created by Mario Stöckli on 06.04.20.
//

#ifndef GRANITE_PARAMETERVISITOR_HPP
#define GRANITE_PARAMETERVISITOR_HPP

#include "Visitor.h"
#include "Dimension.h"
#include "LiteralInt.h"
#include "Matrix.h"
#include "LogicalExpr.h"
#include "UnaryExpr.h"
#include "AbstractBinaryExpr.h"
#include "ArithmeticExpr.h"
#include "AbstractExpr.h"
#include "OpSymbEnum.h"

class ParameterVisitor: Visitor {
private:
    int maxSlots = 1;

    int sndMax = 0;

    int opcount = 0;

    int multCount = 0;
public:
    void visit(AbstractExpr &elem) override;

    void visit(LogicalExpr &elem) override;

    void visit(UnaryExpr &elem) override;

    void visit(ArithmeticExpr &elem) override;

    void visit(LiteralInt &elem) override ;

    int getMaxSlots();

    int getSndMaxSlots();

    int getMultCount();

    int getOpCount();

};

#endif //GRANITE_PARAMETERVISITOR_HPP
