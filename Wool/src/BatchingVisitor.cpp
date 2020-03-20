//
// Created by mario on 08.03.20.
//
#include <cmath>
#include "AbstractExpr.h"
#include "BatchingVisitor.hpp"
#include "Dimension.h"
#include "LiteralInt.h"
#include "Matrix.h"

void BatchingVisitor::visit(AbstractExpr &elem) {
    elem.accept(*this);
}


void BatchingVisitor::visit(LiteralInt &elem) {
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

int BatchingVisitor::getMaxSlots() {
    return this->maxSlots;
}

int BatchingVisitor::getSndMaxSlots(){
    return this->sndMax;
}