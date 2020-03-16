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
    Ast* ast = M::makeAST(f); // self eval happening here. Intermediate results will get calculated instantly, which is why we benchmark this too. This means unwanted overhead from the M class in the final result.
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double, std::milli> time_span = t2 - t1;
    double self_eval_time =  time_span.count();

    double time = Wool::W(*ast).benchmarkWith(l);
    return self_eval_time + time;
}


int M::analyse(std::function<void()> f){
    Ast* ast = M::makeAST(f);
    return Wool::W(*ast).getMultDepth();
}

long M::result(std::function<void()> f){
    Ast* ast = makeAST(f);
    return Wool::W(*ast).evaluateWith(Wool::Library::Plaintext); //TODO set correct library
}

void output(M value){
    M::output(value);
}

// known issue: this has no option to transfer info about the desired crypto library.
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
}

M::M(AbstractExpr *expr, bool plaintext, Wool::Library l){
    this->plaintext = plaintext;
    this->expr = expr;
    this->library = l;
}

M::M(const M &other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = other.library;
}

M::M(M &&other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = other.library;
}

M::M(long i) {
  this->plaintext = true;
  this->expr = new LiteralInt(
      i); //TODO: long to int conversion is not a good idea. change LiteralInt to support longs
  this->library = Wool::Library::Plaintext;
}

M::M(int i) {
  this->plaintext = true;
  this->expr = new LiteralInt(i);
  this->library = Wool::Library::Plaintext;
}

M::M(bool b) {
  this->plaintext = true;
  this->expr = new LiteralBool(b);
  this->library = Wool::Library::Plaintext;
}

M &M::operator=(const M &other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = resolveLibraries(library, other.library);
  return *this;
}

M &M::operator=(M &&other) {
  plaintext = other.plaintext;
  expr = other.expr;
  library = resolveLibraries(library, other.library);
  return *this;
}

M &M::operator=(long i) {
  this->plaintext = true;
  this->expr = new LiteralInt(i);
  if (!isWellSuited(this->library, i)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
  }
  return *this;
}

M &M::operator=(bool b) {
  this->plaintext = true;
  this->expr = new LiteralBool(b);
  if (!isWellSuited(this->library, b)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with bools.");
  }
  return *this;
}

M &M::operator=(int i) {
  this->plaintext = true;
  this->expr = new LiteralInt(i);
  if (!isWellSuited(this->library, i)) {
    throw std::runtime_error(
        "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
  }
  return *this;
}

M &M::operator+=(const M &rhs) {
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::ADDITION, rhs.expr);
  this->expr = exp;
  this->plaintext = this->plaintext && rhs.plaintext;
  this->library = resolveLibraries(this->library, rhs.library);
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
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::SUBTRACTION, rhs.expr);
  this->expr = exp;
  this->plaintext = this->plaintext && rhs.plaintext;
  this->library = resolveLibraries(this->library, rhs.library);
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
  auto exp = new ArithmeticExpr(this->expr, ArithmeticOp::MULTIPLICATION, rhs.expr);
  this->expr = exp;
  this->plaintext = this->plaintext && rhs.plaintext;
  this->library = resolveLibraries(this->library, rhs.library);
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
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::EQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l);
}

M operator!=(const M &lhs, const M &rhs) {
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::UNEQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l);
}

M operator>=(const M &lhs, const M &rhs) {
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::GREATER_EQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l);
}

M operator>(const M &lhs, const M &rhs) {
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::GREATER, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l);
}

M operator<=(const M &lhs, const M &rhs) {
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::SMALLER_EQUAL, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l);
}

M operator<(const M &lhs, const M &rhs) {
    auto exp = new LogicalExpr(lhs.expr, LogCompOp::SMALLER, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l);
}

M operator+(const M &lhs, const M &rhs) {
    auto exp = new ArithmeticExpr(lhs.expr, ArithmeticOp::ADDITION, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l);
}

M operator-(const M &lhs, const M &rhs) {
    auto exp = new ArithmeticExpr(lhs.expr, ArithmeticOp::SUBTRACTION, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l);
}

M operator*(const M &lhs, const M &rhs) {
    auto exp = new ArithmeticExpr(lhs.expr, ArithmeticOp::MULTIPLICATION, rhs.expr);
    bool pt = lhs.isPlaintext() && rhs.isPlaintext();
    Wool::Library l = M::resolveLibraries(lhs.library, rhs.library);
    return M(exp, pt, l);
}

M rotate(M m, int k){
    auto exp = new Rotate(m.getExpr(), new LiteralInt(k));
    return M(exp, m.isPlaintext(), m.getLib());
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

//TODO
// would be nice if we could name this encrypt as well
M batchEncrypt(vector<bool> v){
    Matrix<bool>* mat = new Matrix<bool>(vector<vector<bool>> {v});
    return M(new LiteralBool (mat), false);
}

M batchEncrypt(vector<int> v){
    Matrix<int>* mat = new Matrix<int>(vector<vector<int>> {v});
    return M(new LiteralInt (mat), false);
}

long decrypt(M m) {
  if (m.isPlaintext() && m.getLib() != Wool::Library::Plaintext) throw  std::runtime_error("Computation uses crypto library, but no value was ever encrypted.");
  if (m.isPlaintext()) return Wool::W(m.getExpr()).evaluateWith(Wool::Library::Plaintext); // Plaintext computations are performed always in plaintext
  return Wool::W(m.getExpr()).evaluateWith(m.getLib());
}

std::vector<long> decrypt(std::vector<M> mv) {
  vector<long> dec;
  for (auto x: mv) {
    dec.push_back(Wool::W(x.getExpr()).evaluateWith(x.getLib()));
  }
  return dec;
}

M::M(AbstractExpr *expr, bool plaintext) {
  this->expr = expr;
  this->plaintext = plaintext;
  this->library = Wool::Plaintext;
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


}
