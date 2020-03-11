#include <string>
#include "Wool.hpp"
#include "AbstractExpr.h"
#include "circuit.hpp"
#include "CircuitCompositionVisitor.hpp"
#include "Return.h"
#include "Function.h"
#include "MultDepthVisitor.h"
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
#ifdef HAVE_TFHE_BOOL
#include "context-tfhe-bool.hpp"
#endif
#ifdef HAVE_TFHE_INTEGER
#include "context-tfhe-integer.hpp"
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
  composeCircuit(ae);
  Ast ast = Ast();
  Function *func = dynamic_cast<Function *>(ast.setRootNode(new Function("f")));
  func->addStatement(new Return(ae));
  MultDepthVisitor mdv = MultDepthVisitor();
  mdv.visit(ast);
  this->multDepth = mdv.getMaxDepth();
}

//TODO: make less vulnerable to any changes
W::W(Ast a) {
  Function* f = (Function *) a.getRootNode();
  Return* r = (Return *) f->getBodyStatements()[0];
  AbstractExpr * ae = r->getReturnExpressions()[0];
  composeCircuit(ae);
  MultDepthVisitor mdv = MultDepthVisitor();
  mdv.visit(a);
  this->multDepth = mdv.getMaxDepth();
}

long W::evaluateWith(Library l) {
    BaseContext<int32_t>* ctx = nullptr;
    //TODO: select parameters

  switch (l) {
    case Wool::Plaintext:
       // return get<0>(eval(SHEEP::ContextClear<int32_t>()))[0];
      return get<0>(eval<SHEEP::ContextClear<int64_t >,
                         int64_t >())[0]; // TODO: With the aid of Bithelpers, determine type accurately
#ifdef HAVE_LP
    case Wool::LP:throw std::runtime_error("Not yet implemented.");
#endif
#ifdef HAVE_PALISADE
    case Wool::Palisade:throw std::runtime_error("Not yet implemented.");
#endif
#ifdef HAVE_SEAL_BFV
    case Wool::SEALBFV:
        cout << "Evaluating... with SEALBFV" << endl;
        ctx = reinterpret_cast<BaseContext<int32_t> *>(new SHEEP::ContextSealBFV<int32_t>());
        break;
#endif
#ifdef HAVE_SEAL_CKKS
    case Wool::SEALCKKS:
        cout << "Evaluating... with SEALCKKS" << endl;
        ctx = reinterpret_cast<BaseContext<int32_t> *>(new SHEEP::ContextSealCKKS<double>());
        break;
#endif
#ifdef HAVE_TFHE_BOOL
    case Wool::TFHEBool:
        ctx = reinterpret_cast<BaseContext<int32_t> *>(new SHEEP::ContextTFHE<bool>());
        break;
#endif
#ifdef HAVE_TFHE_INTEGER
    case Wool::TFHEInteger:throw std::runtime_error("Not yet implemented.");
#endif
  }
  if (!ctx){
      cout << "Warning. No valid library at evaluation. Clear context selected." << endl;
      ctx = reinterpret_cast<BaseContext<int32_t> *>(new SHEEP::ContextClear<int64_t >());
  }
  auto r = get<0>(eval<int32_t>(ctx))[0];
  delete ctx;
    return r;
}


double W::benchmarkWith(Library l){
    DurationContainer dc;
    switch (l) {
        case Wool::Plaintext:
            dc =  get<1>(eval<SHEEP::ContextClear<int64_t >, int64_t>()); // TODO: With the aid of Bithelpers, determine int64_t type accurately
            return dc.first[0].count() / 1000 + dc.first[1].count() / 1000 + dc.first[2].count() / 1000; // enc, eval and dec times all added
        case Wool::LP:throw std::runtime_error("Not yet implemented.");
        case Wool::Palisade:throw std::runtime_error("Not yet implemented.");
        case Wool::SEALBFV:
            dc =  get<1>(eval<SHEEP::ContextSealBFV<int64_t>, int64_t >()); // TODO: With the aid of Bithelpers, determine int64_t type accurately
            return dc.first[0].count() / 1000 + dc.first[1].count() / 1000 + dc.first[2].count() / 1000; // enc, eval and dec times all added
        case Wool::SEALCKKS:throw std::runtime_error("Not yet implemented.");
        case Wool::TFHEBool:throw std::runtime_error("Not yet implemented.");
        case Wool::TFHEInteger:throw std::runtime_error("Not yet implemented.");
    }
    throw std::runtime_error("No valid library at evaluation.");
}

//TODO: template with separate plaintext and ciphertext types
template <typename intType_t>
tuple<vector<long>, DurationContainer> W::eval(BaseContext<intType_t> *ctx){
    typedef vector<vector<intType_t>> PtVec;

    DurationContainer dc;

    PtVec inputs;
    for (auto i: ptvec) {
        vector<intType_t> v = {(intType_t) i};
        inputs.push_back(v);
    }

    cout << "Inputs are: ";
    for (auto x : inputs) cout << to_string(x[0]) << " ";
    cout << endl;
    cout << "Const Inputs are: ";
    for (auto x : cptvec) cout << to_string(x) << " ";
    cout << endl;

    PtVec ptv;
    try {
        ptv = ctx->eval_with_plaintexts(c, inputs, cptvec, dc);
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
  cout << "Const Inputs are: ";
  for (auto x : cptvec) cout << to_string(x) << " ";
  cout << endl;

  cout << "Evaluating..." << endl;
  PtVec ptv;
  try {
    ptv = ctx.eval_with_plaintexts(c, inputs, cptvec, dc);
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
  CircuitCompositionVisitor ccv = CircuitCompositionVisitor();
  ccv.visit(*ae);
  this->c = ccv.getCircuit();
  this->ptvec = ccv.getPtvec();
  this->cptvec = ccv.getCptvec();
  //TODO: set recommended library
}

int W::getMultDepth(){
    return this->multDepth;
}

void W::printCircuit(){
    cout << c << endl;
}


}