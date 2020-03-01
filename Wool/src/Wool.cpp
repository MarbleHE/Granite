#include <string>
#include "Wool.hpp"
#include "AbstractExpr.h"
#include "circuit.hpp"
#include "context.hpp"
#include "context-clear.hpp"
#include "CircuitCompositionVisitor.hpp"
#include "Return.h"
#include "Function.h"

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
    case Wool::TFHECommon:return "TFHECommon";
    case Wool::TFHEInteger:return "TFHEInteger";
  }
}

W::W(AbstractExpr *ae) {
  composeCircuit(ae);
}

//TODO: this is extremely vulnerable to any changes
W::W(Ast a) {
   Function* f = (Function *) a.getRootNode();
   Return* r = (Return *) f->getBodyStatements()[0];
   AbstractExpr * ae = r->getReturnExpressions()[0];
  composeCircuit(ae);
}

long W::evaluateWith(Library l) {
  switch (l) {
    case Wool::Plaintext:
      return get<0>(eval<SHEEP::ContextClear<int32_t>,
                         int32_t>())[0]; // TODO: With the aid of Bithelpers, determine int32_t type accurately
    case Wool::LP:throw std::runtime_error("Not yet implemented.");
    case Wool::Palisade:throw std::runtime_error("Not yet implemented.");
    case Wool::SEALBFV:throw std::runtime_error("Not yet implemented.");
    case Wool::SEALCKKS:throw std::runtime_error("Not yet implemented.");
    case Wool::TFHEBool:throw std::runtime_error("Not yet implemented.");
    case Wool::TFHECommon:throw std::runtime_error("Not yet implemented.");
    case Wool::TFHEInteger:throw std::runtime_error("Not yet implemented.");
  }
  throw std::runtime_error("No valid library at evaluation.");
}


double W::benchmarkWith(Library l){
    DurationContainer dc;
    switch (l) {
        case Wool::Plaintext:
            dc =  get<1>(eval<SHEEP::ContextClear<int32_t>, int32_t>()); // TODO: With the aid of Bithelpers, determine int32_t type accurately
            return dc.first[0].count() / 1000 + dc.first[1].count() / 1000 + dc.first[2].count() / 1000; // enc, eval and dec times all added
        case Wool::LP:throw std::runtime_error("Not yet implemented.");
        case Wool::Palisade:throw std::runtime_error("Not yet implemented.");
        case Wool::SEALBFV:throw std::runtime_error("Not yet implemented.");
        case Wool::SEALCKKS:throw std::runtime_error("Not yet implemented.");
        case Wool::TFHEBool:throw std::runtime_error("Not yet implemented.");
        case Wool::TFHECommon:throw std::runtime_error("Not yet implemented.");
        case Wool::TFHEInteger:throw std::runtime_error("Not yet implemented.");
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


}