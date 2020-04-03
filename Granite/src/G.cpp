#include <chrono>
#include "G.hpp"
#include "BitHelpers.h"
#include "Ast.h"
#include "Variable.h"
#include "LiteralInt.h"
#include "LiteralBool.h"
#include "Function.h"
#include "Return.h"
#include "AbstractBinaryExpr.h"
#include "ArithmeticExpr.h"
#include "Operator.h"
#include "UnaryExpr.h"
#include "Wool.hpp"
#include "LogicalExpr.h"
#include "AbstractMatrix.h"
#include "Rotate.h"
#include "PadVisitor.hpp"

using namespace std;

namespace Granite {

// Initialize static member
Ast *G::output_ast = new Ast();

Ast *G::makeAST(std::function<void()> f) {
    G::output_ast = new Ast(); // clear Ast
  f(); // execute function to parse by self-evaluation with the help of the operators defined by G. The output(G m) function sets G::output_ast
  return G::output_ast;
}

double G::evaluate(std::function<void ()> f, Wool::Library l) {
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    Ast* ast = G::makeAST(f); // self eval happening here. Intermediate results will get calculated instantly, which is why we benchmark this too. This means unwanted overhead from the G class and f() in the final result.
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double, std::milli> time_span = t2 - t1;
    double self_eval_time =  time_span.count();
    //TODO: with visitor: find maxSlots and pad all to next slot size (replicate 3 times), except it fits perfectly && sndMaxSlots == 1. If sndMaxSlots > 1, pad to 3 * maxSlots.

    //pad(ast,l); Not necessary. SHEEP pads itself.
    double time = Wool::W(*ast).benchmarkWith(l);
    return self_eval_time + time;
}


int G::analyse(std::function<void()> f){
    Ast* ast = G::makeAST(f);
    return Wool::W(*ast).getMultDepth();
}

long G::result(std::function<void()> f, Wool::Library l){
    Ast* ast = makeAST(f);
    //pad(ast,l); Not necessary. SHEEP pads itself.
    return Wool::W(*ast).evaluateWith(l);
}

void output(G value){
    G::output(value);
}

// known issue: this has no option to transfer info about the desired crypto library. Use evaluate with library instead.
void G::output(const G &m) {
        G::output_ast = new Ast();
  Function *func = dynamic_cast<Function *>(G::output_ast->setRootNode(new Function("f")));
  func->addStatement(new Return((m.expr)));
}

G::G() {
  this->plaintext = true;
  this->library = Wool::Library::Plaintext;
}

G::G(long value, bool plaintext, Wool::Library library) {
  this->plaintext = plaintext;
  this->expr = new LiteralInt((int) value);
  this->library = library;
  this->exprSize = 1;
}


G::G(const G &other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = other.library;
  exprSize = other.exprSize;
}

G::G(G &&other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = other.library;
  exprSize = other.exprSize;
}

G::G(long i) {
  this->plaintext = true;
  this->expr = new LiteralInt(
      i); //TODO: long to int conversion is not a good idea. change LiteralInt to support longs
  this->library = Wool::Library::Plaintext;
  this->exprSize = 1;
}

G::G(int i) {
  this->plaintext = true;
  this->expr = new LiteralInt(i);
  this->library = Wool::Library::Plaintext;
    this->exprSize = 1;
}

G::G(bool b) {
  this->plaintext = true;
  this->expr = new LiteralBool(b);
  this->library = Wool::Library::Plaintext;
    this->exprSize = 1;
}

G &G::operator=(const G &other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = resolveLibraries(library, other.library);
  exprSize = other.exprSize;
  return *this;
}

G &G::operator=(G &&other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = resolveLibraries(library, other.library);
    exprSize = other.exprSize;
    return *this;
}

G &G::operator=(long i) {
  this->plaintext = true;
  this->expr = new LiteralInt(i);
  if (!isWellSuited(this->library, i)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
  }
    this->exprSize = 1;
    return *this;
}

G &G::operator=(bool b) {
  this->plaintext = true;
  this->expr = new LiteralBool(b);
  if (!isWellSuited(this->library, b)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with bools.");
  }
    this->exprSize = 1;
    return *this;
}

G &G::operator=(int i) {
  this->plaintext = true;
  this->expr = new LiteralInt(i);
  if (!isWellSuited(this->library, i)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
  }
    this->exprSize = 1;
    return *this;
}

G &G::operator+=(const G &rhs) {
  this->plaintext = this->plaintext && rhs.plaintext;
  this->library = resolveLibraries(this->library, rhs.library);
    if (exprSize < rhs.exprSize || rhs.exprSize < exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::ADDITION, rhs.expr);
    this->expr = exp;
    return *this;
}

G &G::operator+=(const long &rhs) {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::ADDITION, new LiteralInt(rhs));
  this->expr = exp;
  if (!isWellSuited(this->library, rhs)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
  }
    return *this;
}

G &G::operator+=(const int &rhs) {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::ADDITION, new LiteralInt(rhs));
  this->expr = exp;
  if (!isWellSuited(this->library, rhs)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
  }
    return *this;
}

G &G::operator-=(const G &rhs) {
  this->plaintext = this->plaintext && rhs.plaintext;
  this->library = resolveLibraries(this->library, rhs.library);
    if (exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::SUBTRACTION, rhs.expr);
    this->expr = exp;
    return *this;
}


G &G::operator-=(const long &rhs) {
    auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::SUBTRACTION, new LiteralInt(rhs));
    this->expr = exp;
    if (!isWellSuited(this->library, rhs)) {
        throw std::runtime_error(
                "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
    }
    return *this;
}

G &G::operator-=(const int &rhs) {
    auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::SUBTRACTION, new LiteralInt(rhs));
    this->expr = exp;
    if (!isWellSuited(this->library, rhs)) {
        throw std::runtime_error(
                "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
    }
    return *this;
}

G &G::operator*=(const G &rhs) {
  this->plaintext = this->plaintext && rhs.plaintext;
  this->library = resolveLibraries(this->library, rhs.library);
    if (exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::MULTIPLICATION, rhs.expr);
    this->expr = exp;
    return *this;
}

G &G::operator*=(long &rhs) {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::MULTIPLICATION, new LiteralInt(rhs));
  this->expr = exp;
  if (!isWellSuited(this->library, rhs)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
  }
    return *this;
}

G &G::operator*=(int &rhs) {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::MULTIPLICATION, new LiteralInt(rhs));
  this->expr = exp;
  if (!isWellSuited(this->library, rhs)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
  }
    return *this;
}

G &G::operator++() {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::ADDITION, new LiteralInt(1));
  this->expr = exp;
    return *this;
}

G &G::operator--() {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::SUBTRACTION, new LiteralInt(1));
  this->expr = exp;
    return *this;
}

G &G::operator!() {
  auto exp = new UnaryExpr(UnaryOp::NEGATION, this->expr);
  this->expr = exp;
    return *this;
}

G operator==(const G &lhs, const G &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::EQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = G::resolveLibraries(lhs.library, rhs.library);
    return G(exp, pt, l, lhs.exprSize);
}

G operator!=(const G &lhs, const G &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::UNEQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = G::resolveLibraries(lhs.library, rhs.library);
    return G(exp, pt, l, rhs.exprSize);
}

G operator>=(const G &lhs, const G &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::GREATER_EQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = G::resolveLibraries(lhs.library, rhs.library);
    return G(exp, pt, l, rhs.exprSize);
}

G operator>(const G &lhs, const G &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::GREATER, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = G::resolveLibraries(lhs.library, rhs.library);
    return G(exp, pt, l, rhs.exprSize);
}

G operator<=(const G &lhs, const G &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::SMALLER_EQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = G::resolveLibraries(lhs.library, rhs.library);
    return G(exp, pt, l, rhs.exprSize);
}

G operator<(const G &lhs, const G &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::SMALLER, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = G::resolveLibraries(lhs.library, rhs.library);
    return G(exp, pt, l, rhs.exprSize);
}

G operator+(const G &lhs, const G &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(lhs.expr, ArithmeticOp::ADDITION, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = G::resolveLibraries(lhs.library, rhs.library);
    return G(exp, pt, l, rhs.exprSize);
}

G operator-(const G &lhs, const G &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(lhs.expr, ArithmeticOp::SUBTRACTION, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = G::resolveLibraries(lhs.library, rhs.library);
    return G(exp, pt, l, rhs.exprSize);
}

G operator*(const G &lhs, const G &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(lhs.expr, ArithmeticOp::MULTIPLICATION, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = G::resolveLibraries(lhs.library, rhs.library);
    return G(exp, pt, l, rhs.exprSize);
}

void G::rotate(int k){
    //TODO: handle all Literals and expressions
    if (!dynamic_cast<AbstractLiteral*>( this->getExpr())){
        auto exp = new Rotate(new LiteralInt (new Matrix<AbstractExpr*> (this->getExpr())), k); //TODO: Bool, string, float support
        this->expr = exp;
    }
    else {
        auto exp = new Rotate(this->getExpr(), k);
        this->expr = exp;
    }
}

G encrypt(long value, Wool::Library library) {
  return G(value, false, library);
}

G encrypt(long value) {
  return G(value, false, Wool::Library::Plaintext);
}

vector<G> encrypt(vector<bool> v){
    vector<G> mv;
    for (auto x: v){
        mv.emplace_back(encrypt(x));
    }
    return mv;
}

vector<G> encrypt(vector<int> v){
    vector<G> mv;
    for (auto x: v){
        mv.emplace_back(encrypt(x));
    }
    return mv;
}

vector<G> encrypt(vector<long> v){
    vector<G> mv;
    for (auto x: v){
        mv.emplace_back(encrypt(x));
    }
    return mv;
}


G &G::fold(std::function<G(G, G)> f) {
    // rotate down by half, then apply f to combine values
    while(this->getExprSize() > 1) {
        G t = *this;
        int rotationF = floor((float) t.getExprSize() / (float) 2);
        t.rotate(rotationF);
        t.exprSize = t.exprSize - rotationF;
        this->exprSize = t.getExprSize();
        *this = f(*this,t);
    }

    return *this;
}

G G::sum(G ml, G mr){
    return (ml + mr);
}


G::G(AbstractExpr *ae, bool plaintext, Wool::Library library, int expressionSize){
    this->expr = ae;
    this->plaintext = plaintext;
    this->library = library;
    this->exprSize = expressionSize;
}

G batchEncrypt(vector<bool> v){
    Matrix<bool>* mat = new Matrix<bool>(vector<vector<bool>> {v});
    return G(new LiteralBool (mat), false, Wool::Library::HElib, v.size()); //TODO or is TFHE Bool better?
}

G batchEncrypt(vector<int> v){
    Matrix<int>* mat = new Matrix<int>(vector<vector<int>> {v});
    return G(new LiteralInt (mat), false, Wool::Library::SEALBFV, v.size());
}

long decrypt(G m) {
  if (m.isPlaintext() && m.getLib() != Wool::Library::Plaintext) throw  std::runtime_error("Computation uses crypto library, but no value was ever encrypted.");
  if (m.isPlaintext()){
      //pad(m.getExpr(),Wool::Library::Plaintext); Not necessary. SHEEP pads itself.
      return Wool::W(m.getExpr()).evaluateWith(Wool::Library::Plaintext); // Plaintext computations are performed always in plaintext
  }
  //pad(m.getExpr(), m.getLib()); Not necessary. SHEEP pads itself.
  return Wool::W(m.getExpr()).evaluateWith(m.getLib());
}

std::vector<long> decrypt(std::vector<G> mv) {
  vector<long> dec;
  for (auto x: mv) {
    dec.push_back(Wool::W(x.getExpr()).evaluateWith(x.getLib()));
  }
  return dec;
}


G::~G() {
    //TODO: fix memory leak. Possible solution: refactor G from expression pointers to Ast copies.
}

Wool::Library G::resolveLibraries(Wool::Library l, Wool::Library r) {
  if (l==Wool::Library::Plaintext && r==Wool::Library::Plaintext) return Wool::Library::Plaintext;
  if (l==Wool::Library::Plaintext) return r;
  if (r==Wool::Library::Plaintext) return l;
  if (l!=r)
    throw std::runtime_error(
        "Cannot mix different HE libraries " + Wool::toString(l) + "and " + Wool::toString(r) + ".");
  return l;
}

AbstractExpr *G::getExpr() {
  return this->expr;
}

Wool::Library G::getLib() const {
  return this->library;
}

int G::getExprSize(){
    return this->exprSize;
}

//TODO: validate for correctness
bool G::isWellSuited(Wool::Library l, long x) {
  return !(l==Wool::Library::LP or l==Wool::Library::TFHEBool);
}

bool G::isWellSuited(Wool::Library l, int i) {
  return !(l==Wool::Library::TFHEBool);
}

bool G::isWellSuited(Wool::Library l, bool i) {
  return !(l==Wool::Library::SEALCKKS);
}

bool G::isPlaintext() const {
  return this->plaintext;
}

void G::setLib(Wool::Library l) {
    this->library = l;
    //TODO: check if l is a sane choice for its expr.
}


void pad(Ast* ast, Wool::Library l){
    Function * f = (Function *) ast->getRootNode();
    auto exp = ((Return *) f->getBodyStatements()[0])->getReturnExpressions()[0];
    pad(exp, l);
}

void pad(AbstractExpr* ae, Wool::Library l){
    int nslots = Wool::W(ae).getSlotSize(l);
    PadVisitor pv = PadVisitor(nslots);
    pv.visit(*ae);
}

}
