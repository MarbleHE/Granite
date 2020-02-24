//
// Created by mario on 21.02.20.
//

#ifndef GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP
#define GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP

#include <vector>
#include <stack>
#include <variant>
#include <map>
#include <context.hpp>
#include "Visitor.h"
#include "circuit.hpp"
#include "OpSymbEnum.h"
#include "simple-circuits.hpp"
#include "circuit-util.hpp"
#include "AbstractExpr.h"

class CircuitCompositionVisitor : Visitor {
private:
    /// plaintext vector
    std::vector<long> ptvec;

    /// plaintext vector of constants
    std::vector<long> cptvec;

    /// Stack of circuits, which gets gradually built until only the final Circuit is left.
    std::stack<Circuit> cs;

    /// converts an OpSymb to the corresponding SHEEP gate
    Circuit toGateCircuit(const std::variant<OpSymb::BinaryOp, OpSymb::LogCompOp, OpSymb::UnaryOp> &variant);

    /// Mapping between OpSymb::BinaryOp from ast_lib and Gate from sheep
    const std::map<OpSymb::BinaryOp, Circuit> binopCircuitMap = {
            {OpSymb::BinaryOp::addition,       single_binary_gate_circuit(Gate::Add)},
            {OpSymb::BinaryOp::subtraction,    single_binary_gate_circuit((Gate::Subtract))},
            {OpSymb::BinaryOp::multiplication, single_binary_gate_circuit(Gate::Multiply)}
    };
public:
    std::vector<long> getPtvec();

    std::vector<long> getCptvec();

    void visit(AbstractExpr &elem) override;

    void visit(LiteralBool &elem) override;

    void visit(LiteralInt &elem) override;

    void visit(LiteralString &elem) override;

    void visit(LiteralFloat &elem) override;

    void visit(LogicalExpr &elem) override;

    void visit(UnaryExpr &elem) override;

    void visit(BinaryExpr &elem) override;

    Circuit getCircuit();
};


#endif //GRANITE_CIRCUITCOMPOSITIONVISITOR_HPP
