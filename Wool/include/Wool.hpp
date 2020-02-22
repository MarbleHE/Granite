//
// Created by mario on 17.02.20.
//

#ifndef GRANITE_WOOL_HPP
#define GRANITE_WOOL_HPP

#include <string>
#include "AbstractExpr.h"
#include "circuit.hpp"
#include "context.hpp"

namespace Wool {
    class W;

    /// Libraries supported by SHEEP
    enum Library {Plaintext, HElib, LP, Palisade, SEALBFV, SEALCKKS, TFHEBool, TFHECommon, TFHEInteger};

    /// computes string from library enum
    /// \param l
    /// \return string to corresponding library enum
    std::string toString(Wool::Library l);


    class W{
    public:
        /// Basic constructor initializes Circuit c
        explicit W(AbstractExpr* ae);

        //TODO: W(Ast a);

        /// Evaluates Circuit c of instance with library l.
        /// \param l Library with which we want to evaluate.
        /// \return result of evaluation
        long evaluateWith(Library l);

        //TODO getCircuit();
    private:
        /// Circuit which was derived from AbstractExpr or AST
        Circuit c;

        /// input to feed into Circuit
        std::vector<long> ptvec;

        /// Composes Circuit of AbstractExpr and saves it in c
        /// \param ae AbstractExpr to compose circuit from
        /// \return Circuit
        void composeCircuit(AbstractExpr* ae);

        //TODO: composeCircuit(Ast a);

        /// Evaluates the Circuit c given a SHEEP HE context and integer type.
        /// \tparam genericContext
        /// \tparam intType_t
        /// \return first element is the result and second element is timing information
        template<typename genericContext, typename intType_t> std::tuple<std::vector<long>, DurationContainer> eval();

        /// Recursively builds a Circuit from an Expression and all its subexpression
        /// \param ae
        /// \return Circuit built from ae
        Circuit generateCircuit(AbstractExpr *ae);
    };

};


#endif //GRANITE_WOOL_HPP
