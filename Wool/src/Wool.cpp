#include <string>
#include <exception>
#include "Wool.hpp"
#include "AbstractExpr.h"

using namespace std;

namespace Wool{

    string toString(Wool::Library l) {
        switch (l) {
            case Wool::Plaintext:
                return "Plaintext";
            case  Wool::LP:
                return "LP";
            case  Wool::Palisade:
                return "Palisade";
            case  Wool::SEALBFV:
                return "SEALBFV";
            case  Wool::SEALCKKS:
                return "SEALCKKS";
            case  Wool::TFHEBool:
                return "TFHEBool";
            case  Wool::TFHECommon:
                return "TFHECommon";
            case  Wool::TFHEInteger:
                return "TFHEInteger";
        }
    }

    long evaluate(AbstractExpr *ae, Library l) {
        throw std::runtime_error("Not yet implemented.");
    }

}