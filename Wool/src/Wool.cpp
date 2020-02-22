#include <string>
#include <exception>
#include "Wool.hpp"
#include "AbstractExpr.h"
#include "circuit.hpp"
#include "context.hpp"
#include "context-clear.hpp"
#include "CircuitCompositionVisitor.hpp"

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

    W::W(AbstractExpr* ae){
        composeCircuit(ae);
    }


    long W::evaluateWith(Library l) {
        switch (l){
            case Wool::Plaintext:
                return get<0>(eval<ContextClear<int32_t>, int32_t>())[0]; // TODO: With the aid of Bithelpers, determine int32_t type accurately
            case  Wool::LP:
                throw std::runtime_error("Not yet implemented.");
            case  Wool::Palisade:
                throw std::runtime_error("Not yet implemented.");
            case  Wool::SEALBFV:
                throw std::runtime_error("Not yet implemented.");
            case  Wool::SEALCKKS:
                throw std::runtime_error("Not yet implemented.");
            case  Wool::TFHEBool:
                throw std::runtime_error("Not yet implemented.");
            case  Wool::TFHECommon:
                throw std::runtime_error("Not yet implemented.");
            case  Wool::TFHEInteger:
                throw std::runtime_error("Not yet implemented.");
        }
        throw std::runtime_error("No valid library at evaluation.");
    }

    //TODO: vector<long>? vector<T>?
    template<typename genericContext, typename intType_t>
    tuple<vector<long>, DurationContainer> W::eval() {
        typedef vector<vector<intType_t>> PtVec;

        genericContext ctx;  // paramset, bootstrappable
        DurationContainer dc;

        PtVec inputs;
        for (auto i: ptvec) {
            vector<intType_t> v = {(intType_t) i};
            inputs.push_back(v);
        }

        cout << "Inputs are: ";
        for (auto x : inputs) cout << to_string(x[0]) << " ";
        cout << endl;

        cout << "Evaluating..." << endl;
        PtVec ptv;
        try {
            ptv = ctx.eval_with_plaintexts(c, inputs, dc);
        }
        catch (const GateNotImplemented &e) {
            throw GateNotImplemented();
        }
        vector<long> iptv;
        for (auto x: ptv) {
            iptv.push_back((int) x[0]);
        }
        return make_tuple(iptv, dc);
    }

    void W::composeCircuit(AbstractExpr *ae) {
        CircuitCompositionVisitor ccv(ae);
        this->c = ccv.visit(ae);
        this->ptvec = ccv.getPtvec();
    }

}