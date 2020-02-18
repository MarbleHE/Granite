//
// Created by mario on 17.02.20.
//

#ifndef GRANITE_WOOL_HPP
#define GRANITE_WOOL_HPP

#include <string>
#include "AbstractExpr.h"

namespace Wool {

    enum Library {Plaintext, HElib, LP, Palisade, SEALBFV, SEALCKKS, TFHEBool, TFHECommon, TFHEInteger};

    /// computes string from library enum
    /// \param l
    /// \return string to corresponding library enum
    std::string toString(Wool::Library l);

    long evaluate(AbstractExpr* ae, Library l);

};


#endif //GRANITE_WOOL_HPP
