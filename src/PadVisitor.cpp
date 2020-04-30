//
// Created by mario on 19.03.20.
//
#include <vector>
#include "PadVisitor.hpp"
#include "ast_opt/ast/LiteralInt.h"
#include "ast_opt/ast/Matrix.h"

PadVisitor::PadVisitor(int targetSlotSize) {
    this->targetSlotSize = targetSlotSize;
}

void PadVisitor::visit(AbstractExpr &elem) {
    elem.accept(*this);
}


void PadVisitor::visit(LiteralInt &elem) {
    AbstractMatrix* m = elem.getMatrix();
    if (auto mat = dynamic_cast<Matrix<int>*> (m)){
        int padAmount = this->targetSlotSize - m->getDimensions().numColumns * 3;
        if (padAmount > 0){
            std::vector<int> v;
            for (size_t i = 0; i < m->getDimensions().numColumns; i++) {
                auto x = (LiteralInt *) mat->getElementAt(0, i);
                v.push_back(x->getValue());
            }
            auto t = v;
            t.insert(t.end(), t.begin(), t.end());
            for (size_t i = 0; i < padAmount; i++){
                v.push_back(v[0]); //just push anything in the space that is not used
            }
            v.insert(v.end(), t.begin(), t.end());
            Matrix<int>* paddedM = new Matrix<int>({v});
            elem.setMatrix(paddedM);
        }
    } else {
        elem.getMatrix()->getElementAt(0,0)->accept(*this); //TODO make general solution
    }
}
