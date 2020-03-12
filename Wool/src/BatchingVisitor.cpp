//
// Created by mario on 08.03.20.
//

#include "BatchingVisitor.hpp"
#include "LiteralVector.hpp"
#include "LiteralBool.h"
#include "LiteralInt.h"
#include "LiteralString.h"
#include "LiteralFloat.h"
#include "LogicalExpr.h"
#include "UnaryExpr.h"
#include "AbstractBinaryExpr.h"
#include "ArithmeticExpr.h"


void BatchingVisitor::visit(LiteralVector &elem) {
    if (elem.size() > maxSlots) {
        maxSlots = elem.size();
    }
}


void BatchingVisitor::visit(AbstractExpr &elem) {
}

void BatchingVisitor::visit(LiteralBool &elem) {
}

void BatchingVisitor::visit(LiteralInt &elem) {
}

void BatchingVisitor::visit(LiteralString &elem) {
}

void BatchingVisitor::visit(LiteralFloat &elem) {
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
