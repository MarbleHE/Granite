//
// Created by mario on 08.03.20.
//
#include <cmath>
#include "AbstractExpr.h"
#include "BatchingVisitor.hpp"
#include "AbstractMatrix.h"
#include "LiteralBool.h"
#include "LiteralInt.h"
#include "LiteralString.h"
#include "LiteralFloat.h"
#include "LogicalExpr.h"
#include "UnaryExpr.h"
#include "ArithmeticExpr.h"
#include "Dimension.h"

void BatchingVisitor::visit(AbstractExpr &elem) {
    elem.accept(*this);
}

void BatchingVisitor::visit(AbstractMatrix &elem) {
    if (elem.getDimensions().numColumns >= maxSlots) {
        maxSlots = elem.getDimensions().numColumns;
    } else {
        sndMax = std::max(elem.getDimensions().numColumns,sndMax);
    }
}

void BatchingVisitor::visit(LiteralBool &elem) {
    elem.getMatrix()->accept(*this);
}

void BatchingVisitor::visit(LiteralInt &elem) {
    elem.getMatrix()->accept(*this);
}

void BatchingVisitor::visit(LiteralString &elem) {
    elem.getMatrix()->accept(*this);
}

void BatchingVisitor::visit(LiteralFloat &elem) {
    elem.getMatrix()->accept(*this);
}

void BatchingVisitor::visit(LogicalExpr &elem) {
    elem.getLeft()->accept(*this);
    elem.getRight()->accept(*this);
}

void BatchingVisitor::visit(UnaryExpr &elem) {
    elem.getRight()->accept(*this);
}

void BatchingVisitor::visit(ArithmeticExpr &elem) {
    elem.getLeft()->accept(*this);
    elem.getRight()->accept(*this);
}

int BatchingVisitor::getMaxSlots() {
    return this->maxSlots;
}

int BatchingVisitor::getSndMaxSlots(){
    return this->sndMax;
}