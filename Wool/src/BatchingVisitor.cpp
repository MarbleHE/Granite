//
// Created by mario on 08.03.20.
//

#include "BatchingVisitor.hpp"
#include "LiteralVector.hpp"


void BatchingVisitor::visit(LiteralVector &elem){
    if (elem.size() > maxSlots){
        maxSlots = elem.size();
    }
}