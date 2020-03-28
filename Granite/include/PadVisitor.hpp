//
// Created by mario on 19.03.20.
//

#ifndef GRANITE_PADVISITOR_HPP
#define GRANITE_PADVISITOR_HPP

#include "Visitor.h"
#include "AbstractExpr.h"
#include "LiteralInt.h"

class PadVisitor: Visitor {
private:
    int targetSlotSize;
public:
    PadVisitor(int targetSlotSize);

    void visit(AbstractExpr &elem) override;

    void visit(LiteralInt &elem) override;

};
#endif //GRANITE_PADVISITOR_HPP
