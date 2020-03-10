//
// Created by mario on 08.03.20.
//

#ifndef GRANITE_BATCHINGVISITOR_HPP
#define GRANITE_BATCHINGVISITOR_HPP

#include "Visitor.h"
#include "LiteralVector.hpp"

class BatchingVisitor: Visitor {
private:
    int maxSlots;
public:
    void visit(LiteralVector &elem);
};


#endif //GRANITE_BATCHINGVISITOR_HPP
