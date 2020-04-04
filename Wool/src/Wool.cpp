#include <algorithm>
#include <cmath>
#include <string>
#include "Wool.hpp"
#include "AbstractExpr.h"
#include "circuit.hpp"
#include "CircuitCompositionVisitor.hpp"
#include "Return.h"
#include "Function.h"
#include "MultDepthVisitor.h"
#include "BatchingVisitor.hpp"
#include "OpCountVisitor.hpp"
#include "MultCountVisitor.hpp"
#include "context.hpp"
#include "context-clear.hpp"
#ifdef HAVE_LP
#include "context-lp.hpp"
#endif
#ifdef HAVE_PALISADE
#include "context-palisade.hpp"
#endif
#ifdef HAVE_SEAL_BFV
#include "context-seal-bfv.hpp"
#endif
#ifdef HAVE_SEAL_CKKS
#include "context-seal-ckks.hpp"
#endif
#ifdef HAVE_TFHE
#include "context-tfhe.hpp"
#endif
#ifdef HAVE_HElib
#include "context-helib.hpp"
#endif

using namespace std;

namespace Wool {

string toString(Wool::Library l) {
  switch (l) {
    case Wool::Plaintext:return "Plaintext";
    case Wool::LP:return "LP";
    case Wool::Palisade:return "Palisade";
    case Wool::SEALBFV:return "SEALBFV";
    case Wool::SEALCKKS:return "SEALCKKS";
    case Wool::TFHEBool:return "TFHEBool";
    case Wool::TFHEInteger:return "TFHEInteger";
  }
}

W::W(AbstractExpr *ae) {
  Ast ast = Ast();
  Function *func = dynamic_cast<Function *>(ast.setRootNode(new Function("f")));
  func->addStatement(new Return(ae));
  calculateParams(ast);
  composeCircuit(ae);
}

//TODO: make less vulnerable to any changes
W::W(Ast a) {
  Function* f = (Function *) a.getRootNode();
  Return* r = (Return *) f->getBodyStatements()[0];
  AbstractExpr * ae = r->getReturnExpressions()[0];
  calculateParams(a);
  composeCircuit(ae);
}

long W::evaluateWith(Library l) {
    int pt_size = estimatePlaintextSize();
    long r;
    if (pt_size < 8){
        auto ctx = generateContext<int8_t>(l);
        r = get<0>(eval<int8_t >(ctx))[0];
    }
    else if (pt_size < 16){
        auto ctx = generateContext<int16_t>(l);
        r =  get<0>(eval<int16_t >(ctx))[0];
    }
    else if (pt_size < 32){
        auto ctx = generateContext<int32_t>(l);
        r =  get<0>(eval<int32_t >(ctx))[0];
    }
    else {
        cout << "Warning: Result might not fit into plaintext modulus. ";
        auto ctx = generateContext<int64_t> (l);
        r =  get<0>(eval<int64_t >(ctx))[0];
    }
  return r;
}

double W::benchmarkWith(Library l) {
    int pt_size = estimatePlaintextSize();
    DurationContainer dc;
    if (pt_size < 8){
        auto ctx = generateContext<int8_t>(l);
        dc = get<1>(eval<int8_t >(ctx));
    }
    else if (pt_size < 16){
        auto ctx = generateContext<int16_t>(l);
        dc =  get<1>(eval<int16_t >(ctx));
    }
    else if (pt_size < 32){
        auto ctx = generateContext<int32_t>(l);
        dc =  get<1>(eval<int32_t >(ctx));
    }
    else {
        cout << "Warning: Result might not fit into plaintext modulus. ";
        auto ctx = generateContext<int64_t> (l);
        dc =  get<1>(eval<int64_t >(ctx));
    }
    return dc.first[0].count() / 1000 + dc.first[1].count() / 1000 + dc.first[2].count() / 1000; // enc, eval and dec times all added
}

template <typename intType>
BaseContext<intType>* W::generateContext(Library l){
    //TODO: select parameters
    int N = estimateN(l);
    int slotIndex = 0;
    switch (l) {
        case Wool::Plaintext:
            cout << "Evaluating... in Plaintext" << endl;
            return new SHEEP::ContextClear<intType>(getSlotSize(Plaintext));
    //TODO: implement all libraries
#ifdef HAVE_LP
            case Wool::LP:
                cout << "Evaluating... with LP" << endl;
            return new SHEEP::ContextLP<intType>();
#endif
#ifdef HAVE_PALISADE
            case Wool::Palisade:
                cout << "Evaluating... with Palisade" << endl;
            return new SHEEP::ContextPalisade<intType>(estimatePlaintextSize(),maxSlots,nMults,multDepth);
#endif
#ifdef HAVE_HElib
        case Wool::HElib:
            cout << "Evaluating... with HElib" << endl;
            slotIndex = 0;
            for (auto x : slotsBGV){
                if (x == maxSlots && sndMaxSlots * 3 < maxSlots){
                    return new SHEEP::ContextHElib_F2<intType>(slotIndex , 1, 128);
                }
                slotIndex++;
            }
            slotIndex = 0;
            for (auto x : slotsBGV){
                if (3 * maxSlots < x){
                    return new SHEEP::ContextHElib_F2<intType>(slotIndex , 1, 128);
                }
                slotIndex++;
            }
            return new SHEEP::ContextHElib_F2<intType>(slotIndex , 1, 128);
            //TODO: implement HElib with bools
#endif
#ifdef HAVE_SEAL_BFV
        case Wool::SEALBFV:
            cout << "Evaluating... with SEALBFV" << endl;
            return new SHEEP::ContextSealBFV<intType>(estimatePlaintextSize(),128,N, false);
#endif
#ifdef HAVE_SEAL_CKKS
        case Wool::SEALCKKS:
            throw std::runtime_error("Wrong template. Dont' try to use CKKS with integer types...");
#endif
#ifdef HAVE_TFHE
        case Wool::TFHEInteger:
             cout << "Evaluating... with TFHE" << endl;
             return new SHEEP::ContextTFHE<intType>(128);
#endif
    }

    throw std::runtime_error("Selected Library not correctly installed.");
}

#ifdef HAVE_SEAL_CKKS
//Only one library handling doubles so far...
template <>
BaseContext<double>* W::generateContext(Library l){
    int N = estimateN(l);
    cout << "Evaluating... with SEALCKKS" << endl;
    return new SHEEP::ContextSealCKKS<double>(40961, N, 30);
}
#endif

#ifdef HAVE_TFHE
//Only one library handling bools so far...
template <>
BaseContext<bool>* W::generateContext(Library l){
    switch (l) {
        case TFHEBool:
            cout << "Evaluating... with TFHE<bool>" << endl;
            return new SHEEP::ContextTFHE<bool>();
        case Plaintext:
            return new SHEEP::ContextClear<bool>();
            //TODO add HElib with bool...
    }

}
#endif

template <typename intType_t>
tuple<vector<long>, DurationContainer> W::eval(BaseContext<intType_t> *ctx){
    typedef vector<vector<intType_t>> PtVec;

    DurationContainer dc;

    PtVec inputs;
    for (auto i: ptvec) {
        vector<intType_t> in;
        for (auto j: i){
            in.push_back((intType_t) j);
        }
        inputs.push_back(in);
    }

    PtVec ptv;
    try {
        /*TODO: find out why TBB evaluation gets keyerror, when it tries to lookup outputs in the eval_map at the end
#ifdef HAVE_TBB
    cout << "in parallel..." << endl;
    ptv = ctx->eval_with_plaintexts(c, inputs, cptvec, dc, EvaluationStrategy::parallel);
#else*/
    ptv = ctx->eval_with_plaintexts(c, inputs, cptvec, dc);
//#endif
    }
    catch (const GateNotImplemented &e) {
        throw runtime_error(e.what());
    }
    vector<long> iptv;
    for (auto x: ptv) {
        iptv.push_back((long) x[0]);
    }
    return make_tuple(iptv, dc);
}

//TODO: vector<long>? vector<T>?
template<typename genericContext, typename intType_t>
tuple<vector<long>, DurationContainer> W::eval() {
  typedef vector<vector<intType_t>> PtVec;

  genericContext ctx;  // paramset, bootstrappable
  DurationContainer dc;

  PtVec inputs;
  for (auto i: ptvec) {
    vector<intType_t> in;
    for (auto j: i){
        in.push_back((intType_t) j);
    }
    inputs.push_back(in);
  }

  cout << "Evaluating..." << endl;
  PtVec ptv;
  try {
#ifdef HAVE_TBB
  cout << "in parallel..." << endl;
  ptv = ctx.eval_with_plaintexts(c, inputs, cptvec, dc, EvaluationStrategy::parallel);
#else
  ptv = ctx.eval_with_plaintexts(c, inputs, cptvec, dc);
#endif
  }
  catch (const GateNotImplemented &e) {
    throw GateNotImplemented();
  }
  vector<long> iptv;
  for (auto x: ptv) {
    iptv.push_back((long) x[0]);
  }
  return make_tuple(iptv, dc);
}

void W::composeCircuit(AbstractExpr *ae) {
  CircuitCompositionVisitor ccv = CircuitCompositionVisitor();
  ccv.visit(*ae);
  this->c = ccv.getCircuit();
  this->ptvec = ccv.getPtvec();
  this->cptvec = ccv.getCptvec();
  //TODO: set recommended library with visitor...
}

int W::getMultDepth(){
    return this->multDepth;
}

void W::printCircuit(){
    cout << c << endl;
}

int W::estimatePlaintextSize() {
    // The maximum possible number to achieve in a circuit is by multiplying the largest number multDepth times itself
    auto ptv = ptvec;
    auto cptv = cptvec;
    if (ptv.empty()){
        ptv.emplace_back(1);
    }
    if (cptv.empty()){
        cptv.emplace_back(1);
    }
    vector<long> ptmaxv;
    for (auto x : ptvec){
        if (!x.empty()){
            auto me = max_element(x.begin(),x.end());
            ptmaxv.push_back(*me);
        }
    }
    auto cptmax = max_element(cptv.begin(),cptv.end());
    auto ptmax = max_element(ptmaxv.begin(),ptmaxv.end());
    if (*ptmax == 0 == *cptmax){
        *ptmax = 1; // handle 0 vectors... no log problems.
    }
    int max_val = max(*ptmax,*cptmax);
    int other_ops = opcount - multDepth; //any other operations can at most multiply the result. (e.g. with +)
    max_val = other_ops*max_val;
    int max_pt_sz = (multDepth + 1) * ceil(log2(max_val)) + 1;
    return max_pt_sz;
}

int W::estimateN(Library l){
    int qN = estimateNViaQ(l);
    vector<int> slots;
    switch (l){
        case SEALCKKS:
            slots = slotsCKKS;
            break;
        case SEALBFV:
            slots = slotsBFV;
            break;
        case HElib:
            return 0; //No N for HElib
        case LP:
            return 0; //No N for LP
        case Palisade: //TODO find out how these schemes handle slots
        case TFHEBool:
        case TFHEInteger:
            return 0;
        case Plaintext:
            return 0;  //No N for Plaintext
    }
    int i = 0;
    for (auto x : slots){
        if (x == maxSlots && sndMaxSlots * 3 < maxSlots){
            return max(N[i],qN);
        }
        i++;
    }
    i = 0;
    for (auto x : slots){
        if (3 * maxSlots < x){
            return max(N[i],qN);
        }
        i++;
    }
    throw runtime_error("The maximum number of slots (" + to_string(maxSlots) +") in your function exceeds a sane value. ");
}

int W::estimateNViaQ(Library l){
    int Q = (multDepth + 1) * 60; // 60 bits each q_i
    int j = 0;
    for (size_t i = 0; i < Q128bit.size(); i++){
        if (Q128bit[i] > Q){
            break;
        }
        j++;
    }
    if (j >= Q128bit.size()){ // we have not found a good Q
        throw std::runtime_error("No suitable Q found. MultDepth (" + to_string(multDepth) + ") might exceed limits.");
    }
    return N[j];
}

int W::getSlotSize(Library l){
    if (maxSlots == 1) return 1;
    int i = 0;
    switch (l){
        case HElib:
            throw runtime_error("not implemented"); //TODO
        case SEALBFV:
            return estimateN(l)/2;
        case SEALCKKS:
            return estimateN(l)/2;
        case Plaintext:
            while (pow(2, i) <= maxSlots){
                if (pow(2, i) == maxSlots && sndMaxSlots * 3 < maxSlots){
                    return pow(2, i);
                }
                i++;
            }
            i = 0;
            while (pow(2, i) < 3 * maxSlots ){
                i++;
            }
            return pow(2,i);
        default:
            if (maxSlots > 1) throw std::runtime_error("Library has maxSLots > 1, but does not support batching"); //TODO: move this throw to proper place
            return 1; //TODO: other libraries with slots?
    }

}

void W::calculateParams(Ast ast) {
    Function* f = (Function *) ast.getRootNode();
    Return* r = (Return *) f->getBodyStatements()[0];
    AbstractExpr * ae = r->getReturnExpressions()[0];
    MultDepthVisitor mdv = MultDepthVisitor();
    mdv.visit(ast);
    this->multDepth = mdv.getMaxDepth();
    OpCountVisitor opv = OpCountVisitor();
    opv.visit(*ae);
    this->opcount = opv.getOpCount();
    BatchingVisitor bv = BatchingVisitor();
    bv.visit(*ae);
    this->maxSlots = bv.getMaxSlots();
    this->sndMaxSlots = bv.getSndMaxSlots();
    MultCountVisitor mcv = MultCountVisitor();
    mcv.visit(*ae);
    this->nMults = mcv.getMultCount();
}


}