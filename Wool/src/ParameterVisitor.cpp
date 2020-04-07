//
// Created by Mario Stöckli on 06.04.20.
//


#include <cmath>
#include "ParameterVisitor.hpp"
#include "Dimension.h"
#include "LiteralInt.h"
#include "Matrix.h"
#include "LogicalExpr.h"
#include "UnaryExpr.h"
#include "ArithmeticExpr.h"
#include "AbstractExpr.h"
#include "OpSymbEnum.h"


int ParameterVisitor::getMultCount() {
    return this->multCount;
}

void ParameterVisitor::visit(AbstractExpr &elem) {
    elem.accept(*this);
}

void ParameterVisitor::visit(ArithmeticExpr &elem) {
    opcount++;
    if (std::get<ArithmeticOp>(elem.getOperator()->getOperatorSymbol()) == ArithmeticOp::MULTIPLICATION){
        multCount++;
    }
    elem.getLeft()->accept(*this);
    elem.getRight()->accept(*this);
}


int ParameterVisitor::getOpCount() {
    return this->opcount;
}

void ParameterVisitor::visit(LogicalExpr &elem) {
    opcount++;
    elem.getLeft()->accept(*this);
    elem.getRight()->accept(*this);
}

void ParameterVisitor::visit(UnaryExpr &elem) {
    opcount++;
    elem.getRight()->accept(*this);
}

void ParameterVisitor::visit(LiteralInt &elem) {
    auto m = elem.getMatrix();
    if (auto mat = dynamic_cast<Matrix<int>*> (m)){
        if (mat->getDimensions().numColumns >= maxSlots) {
            maxSlots = mat->getDimensions().numColumns;
        } else {
            sndMax = std::max(mat->getDimensions().numColumns,sndMax);
        }
    } else if (auto mat = dynamic_cast<Matrix<AbstractExpr*>*> (m)){
        for (size_t i = 0; i < mat->getDimensions().numColumns; i++){
            mat->getElementAt(0,i)->accept(*this);
        }
    } else {
        throw std::runtime_error("LiteralInt has something in Matrix it should not.");
    }
}

int ParameterVisitor::getMaxSlots() {
    return this->maxSlots;
}

int ParameterVisitor::getSndMaxSlots(){
    return this->sndMax;
}