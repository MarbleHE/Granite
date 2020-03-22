#include <chrono>
#include "M.hpp"
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

namespace Marble {

// Initialize static member
Ast *M::output_ast = new Ast();

Ast *M::makeAST(std::function<void()> f) {
  M::output_ast = new Ast(); // clear Ast
  f(); // execute function to parse by self-evaluation with the help of the operators defined by M. The output(M m) function sets M::output_ast
  return M::output_ast;
}

double M::evaluate(std::function<void ()> f, Wool::Library l) {
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    Ast* ast = M::makeAST(f); // self eval happening here. Intermediate results will get calculated instantly, which is why we benchmark this too. This means unwanted overhead from the M class and f() in the final result.
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double, std::milli> time_span = t2 - t1;
    double self_eval_time =  time_span.count();
    //TODO: with visitor: find maxSlots and pad all to next slot size (replicate 3 times), except it fits perfectly && sndMaxSlots == 1. If sndMaxSlots > 1, pad to 3 * maxSlots.

    //pad(ast,l); Not necessary. SHEEP pads itself.
    double time = Wool::W(*ast).benchmarkWith(l);
    return self_eval_time + time;
}


int M::analyse(std::function<void()> f){
    Ast* ast = M::makeAST(f);
    return Wool::W(*ast).getMultDepth();
}

long M::result(std::function<void()> f, Wool::Library l){
    Ast* ast = makeAST(f);
    //pad(ast,l); Not necessary. SHEEP pads itself.
    return Wool::W(*ast).evaluateWith(l);
}

void output(M value){
    M::output(value);
}

// known issue: this has no option to transfer info about the desired crypto library. Use evaluate with library instead.
void M::output(const M &m) {
  M::output_ast = new Ast();
  Function *func = dynamic_cast<Function *>(M::output_ast->setRootNode(new Function("f")));
  func->addStatement(new Return((m.expr)));
}

M::M() {
  this->plaintext = true;
  this->expr = new Variable("_var_");
  this->library = Wool::Library::Plaintext;
}

M::M(long value, bool plaintext, Wool::Library library) {
  this->plaintext = plaintext;
  this->expr = new LiteralInt((int) value);
  this->library = library;
  this->exprSize = 1;
}


M::M(const M &other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = other.library;
  exprSize = other.exprSize;
}

M::M(M &&other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = other.library;
  exprSize = other.exprSize;
}

M::M(long i) {
  this->plaintext = true;
  this->expr = new LiteralInt(
      i); //TODO: long to int conversion is not a good idea. change LiteralInt to support longs
  this->library = Wool::Library::Plaintext;
  this->exprSize = 1;
}

M::M(int i) {
  this->plaintext = true;
  this->expr = new LiteralInt(i);
  this->library = Wool::Library::Plaintext;
    this->exprSize = 1;
}

M::M(bool b) {
  this->plaintext = true;
  this->expr = new LiteralBool(b);
  this->library = Wool::Library::Plaintext;
    this->exprSize = 1;
}

M &M::operator=(const M &other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = resolveLibraries(library, other.library);
  exprSize = other.exprSize;
  return *this;
}

M &M::operator=(M &&other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = resolveLibraries(library, other.library);
    exprSize = other.exprSize;
    return *this;
}

M &M::operator=(long i) {
  this->plaintext = true;
  this->expr = new LiteralInt(i);
  if (!isWellSuited(this->library, i)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
  }
    this->exprSize = 1;
    return *this;
}

M &M::operator=(bool b) {
  this->plaintext = true;
  this->expr = new LiteralBool(b);
  if (!isWellSuited(this->library, b)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with bools.");
  }
    this->exprSize = 1;
    return *this;
}

M &M::operator=(int i) {
  this->plaintext = true;
  this->expr = new LiteralInt(i);
  if (!isWellSuited(this->library, i)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
  }
    this->exprSize = 1;
    return *this;
}

M &M::operator+=(const M &rhs) {
  this->plaintext = this->plaintext && rhs.plaintext;
  this->library = resolveLibraries(this->library, rhs.library);
    if (exprSize < rhs.exprSize || rhs.exprSize < exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::ADDITION, rhs.expr);
    this->expr = exp;
    return *this;
}

M &M::operator+=(const long &rhs) {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::ADDITION, new LiteralInt(rhs));
  this->expr = exp;
  if (!isWellSuited(this->library, rhs)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
  }
    return *this;
}

M &M::operator+=(const int &rhs) {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::ADDITION, new LiteralInt(rhs));
  this->expr = exp;
  if (!isWellSuited(this->library, rhs)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
  }
    return *this;
}

M &M::operator-=(const M &rhs) {
  this->plaintext = this->plaintext && rhs.plaintext;
  this->library = resolveLibraries(this->library, rhs.library);
    if (exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::SUBTRACTION, rhs.expr);
    this->expr = exp;
    return *this;
}


M &M::operator-=(const long &rhs) {
    auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::SUBTRACTION, new LiteralInt(rhs));
    this->expr = exp;
    if (!isWellSuited(this->library, rhs)) {
        throw std::runtime_error(
                "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
    }
    return *this;
}

M &M::operator-=(const int &rhs) {
    auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::SUBTRACTION, new LiteralInt(rhs));
    this->expr = exp;
    if (!isWellSuited(this->library, rhs)) {
        throw std::runtime_error(
                "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
    }
    return *this;
}

M &M::operator*=(const M &rhs) {
  this->plaintext = this->plaintext && rhs.plaintext;
  this->library = resolveLibraries(this->library, rhs.library);
    if (exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::MULTIPLICATION, rhs.expr);
    this->expr = exp;
    return *this;
}

M &M::operator*=(long &rhs) {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::MULTIPLICATION, new LiteralInt(rhs));
  this->expr = exp;
  if (!isWellSuited(this->library, rhs)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
  }
    return *this;
}

M &M::operator*=(int &rhs) {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::MULTIPLICATION, new LiteralInt(rhs));
  this->expr = exp;
  if (!isWellSuited(this->library, rhs)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
  }
    return *this;
}

M &M::operator++() {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::ADDITION, new LiteralInt(1));
  this->expr = exp;
    return *this;
}

M &M::operator--() {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::SUBTRACTION, new LiteralInt(1));
  this->expr = exp;
    return *this;
}

M &M::operator!() {
  auto exp = new UnaryExpr(UnaryOp::NEGATION, this->expr);
  this->expr = exp;
    return *this;
}

M operator==(const M &lhs, const M &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::EQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l, lhs.exprSize);
}

M operator!=(const M &lhs, const M &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::UNEQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l, rhs.exprSize);
}

M operator>=(const M &lhs, const M &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::GREATER_EQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l, rhs.exprSize);
}

M operator>(const M &lhs, const M &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::GREATER, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l, rhs.exprSize);
}

M operator<=(const M &lhs, const M &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::SMALLER_EQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l, rhs.exprSize);
}

M operator<(const M &lhs, const M &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::SMALLER, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l, rhs.exprSize);
}

M operator+(const M &lhs, const M &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(lhs.expr, ArithmeticOp::ADDITION, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l, rhs.exprSize);
}

M operator-(const M &lhs, const M &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(lhs.expr, ArithmeticOp::SUBTRACTION, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l, rhs.exprSize);
}

M operator*(const M &lhs, const M &rhs) {
    if (lhs.exprSize < rhs.exprSize){
        throw std::runtime_error("Expression size mismatch.");
    }
    auto exp = new ArithmeticExpr(lhs.expr, ArithmeticOp::MULTIPLICATION, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l, rhs.exprSize);
}

void M::rotate(int k){
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

M encrypt(long value, Wool::Library library) {
  return M(value, false, library);
}

M encrypt(long value) {
  return M(value, false, Wool::Library::Plaintext);
}

vector<M> encrypt(vector<bool> v){
    vector<M> mv;
    for (auto x: v){
        mv.emplace_back(encrypt(x));
    }
    return mv;
}

vector<M> encrypt(vector<int> v){
    vector<M> mv;
    for (auto x: v){
        mv.emplace_back(encrypt(x));
    }
    return mv;
}

vector<M> encrypt(vector<long> v){
    vector<M> mv;
    for (auto x: v){
        mv.emplace_back(encrypt(x));
    }
    return mv;
}


M &M::fold(std::function<M(M, M)> f) {
    // rotate down by half, then apply f to combine values
    while(this->getExprSize() > 1) {
        M t = *this;
        t.rotate(t.getExprSize() / 2);
        t.exprSize = ceil((float) t.exprSize / (float) 2);
        this->exprSize = t.getExprSize();
        *this = f(*this,t);
    }

    return *this;
}

M M::sum(M ml,M mr){
    return (ml + mr);
}


M::M(AbstractExpr *ae, bool plaintext, Wool::Library library, int expressionSize){
    this->expr = ae;
    this->plaintext = plaintext;
    this->library = library;
    this->exprSize = expressionSize;
}

M batchEncrypt(vector<bool> v){
    Matrix<bool>* mat = new Matrix<bool>(vector<vector<bool>> {v});
    return M(new LiteralBool (mat), false, Wool::Library::HElib,v.size()); //TODO or is TFHE Bool better?
}

M batchEncrypt(vector<int> v){
    Matrix<int>* mat = new Matrix<int>(vector<vector<int>> {v});
    return M(new LiteralInt (mat), false, Wool::Library::SEALBFV, v.size());
}

long decrypt(M m) {
  if (m.isPlaintext() && m.getLib() != Wool::Library::Plaintext) throw  std::runtime_error("Computation uses crypto library, but no value was ever encrypted.");
  if (m.isPlaintext()){
      //pad(m.getExpr(),Wool::Library::Plaintext); Not necessary. SHEEP pads itself.
      return Wool::W(m.getExpr()).evaluateWith(Wool::Library::Plaintext); // Plaintext computations are performed always in plaintext
  }
  //pad(m.getExpr(), m.getLib()); Not necessary. SHEEP pads itself.
  return Wool::W(m.getExpr()).evaluateWith(m.getLib());
}

std::vector<long> decrypt(std::vector<M> mv) {
  vector<long> dec;
  for (auto x: mv) {
    dec.push_back(Wool::W(x.getExpr()).evaluateWith(x.getLib()));
  }
  return dec;
}


M::~M() {
    //TODO: fix memory leak. Possible solution: refactor M from expression pointers to Ast copies.
}

Wool::Library M::resolveLibraries(Wool::Library l, Wool::Library r) {
  if (l==Wool::Library::Plaintext && r==Wool::Library::Plaintext) return Wool::Library::Plaintext;
  if (l==Wool::Library::Plaintext) return r;
  if (r==Wool::Library::Plaintext) return l;
  if (l!=r)
    throw std::runtime_error(
        "Cannot mix different HE libraries " + Wool::toString(l) + "and " + Wool::toString(r) + ".");
  return l;
}

AbstractExpr *M::getExpr() {
  return this->expr;
}

Wool::Library M::getLib() const {
  return this->library;
}

int M::getExprSize(){
    return this->exprSize;
}

//TODO: validate for correctness
bool M::isWellSuited(Wool::Library l, long x) {
  return !(l==Wool::Library::LP or l==Wool::Library::TFHEBool);
}

bool M::isWellSuited(Wool::Library l, int i) {
  return !(l==Wool::Library::TFHEBool);
}

bool M::isWellSuited(Wool::Library l, bool i) {
  return !(l==Wool::Library::SEALCKKS);
}

bool M::isPlaintext() const {
  return this->plaintext;
}

void M::setLib(Wool::Library l) {
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
