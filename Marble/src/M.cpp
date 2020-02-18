#include "M.hpp"
#include "BitHelpers.h"
#include "Ast.h"
#include "Variable.h"
#include "LiteralInt.h"
#include "LiteralBool.h"
#include "Function.h"
#include "Return.h"
#include "BinaryExpr.h"
#include "Operator.h"
#include "UnaryExpr.h"
#include "Wool.hpp"

using namespace std;

namespace Marble {

    // Initialize static member
    Ast *M::output_ast = new Ast();


    Ast *M::make_AST(std::function<void()> f) {
        M::output_ast = new Ast(); // clear Ast
        f(); // execute function to parse by self-evaluation with the help of the operators defined by M. The output(M m) function sets M::output_ast
        //TODO: copy problem?
        return M::output_ast;
    }

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

    M::M(long value,Wool::Library library,bool plaintext){
        this->plaintext = plaintext;
        this->expr = new LiteralInt((int) value);
        this->library = library;
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
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::addition, rhs.expr);
        this->expr = exp;
        this->plaintext = this->plaintext && rhs.plaintext;
        this->library = resolveLibraries(this->library, rhs.library);
        return *this;
    }

    M &M::operator+=(const long &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::addition, new LiteralInt(rhs));
        this->expr = exp;
        if (!isWellSuited(this->library, rhs)) {
            throw std::runtime_error(
                    "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
        }
        return *this;
    }

    M &M::operator+=(const int &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::addition, new LiteralInt(rhs));
        this->expr = exp;
        if (!isWellSuited(this->library, rhs)) {
            throw std::runtime_error(
                    "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
        }
        return *this;
    }

    M &M::operator-=(const M &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::subtraction, rhs.expr);
        this->expr = exp;
        this->plaintext = this->plaintext && rhs.plaintext;
        this->library = resolveLibraries(this->library, rhs.library);
        return *this;
    }

    //TODO: -= long &rhs missing?

    M &M::operator*=(const M &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::multiplication, rhs.expr);
        this->expr = exp;
        this->plaintext = this->plaintext && rhs.plaintext;
        this->library = resolveLibraries(this->library, rhs.library);
        return *this;
    }

    M &M::operator*=(long &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::multiplication, new LiteralInt(rhs));
        this->expr = exp;
        if (!isWellSuited(this->library, rhs)) {
            throw std::runtime_error(
                    "The library " + Wool::toString(this->library) + "is not well suited to work with longs.");
        }
        return *this;
    }

    M &M::operator*=(int &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::multiplication, new LiteralInt(rhs));
        this->expr = exp;
        if (!isWellSuited(this->library, rhs)) {
            throw std::runtime_error(
                    "The library " + Wool::toString(this->library) + "is not well suited to work with ints.");
        }
        return *this;
    }

    M &M::operator++() {
        auto exp = new UnaryExpr(OpSymb::UnaryOp::increment, this->expr);
        this->expr = exp;
        return *this;
    }

    M &M::operator--() {
        auto exp = new UnaryExpr(OpSymb::UnaryOp::decrement, this->expr);
        this->expr = exp;
        return *this;
    }

    M &M::operator!() {
        auto exp = new UnaryExpr(OpSymb::UnaryOp::negation, this->expr);
        this->expr = exp;
        return *this;
    }

    M encrypt(long value, Wool::Library library) {
        return M(value, library, false);
    }

    M encrypt(long value){
        return M(value, Wool::Library::Plaintext, false);
    }

    long decrypt(M m) {
        return Wool::evaluate(m.getExpr(), m.getLib());
    }

    std::vector<long> decrypt(std::vector<M> mv) {
        vector<long> dec;
        for(auto x: mv){
            dec.push_back(Wool::evaluate(x.getExpr(), x.getLib()));
        }
        return dec;
    }


    M::M(AbstractExpr &expr, bool plaintext) {
        this->expr = &expr;
        this->plaintext = plaintext;
        this->library = Wool::Plaintext;
    }

    M::~M() {
    }

    Wool::Library M::resolveLibraries(Wool::Library l, Wool::Library r) {
        if (l == Wool::Library::Plaintext && r == Wool::Library::Plaintext) return Wool::Library::Plaintext;
        if (l == Wool::Library::Plaintext) return r;
        if (r == Wool::Library::Plaintext) return l;
        if (l != r)
            throw std::runtime_error("Cannot mix different HE libraries " + Wool::toString(l) + "and " + Wool::toString(r) + ".");
        return l;
    }

    AbstractExpr *M::getExpr() {
        return this->expr;
    }

    Wool::Library M::getLib() {
        return this->library;
    }

    //TODO: validate for correctness
    bool M::isWellSuited(Wool::Library l, long x) {
        return !(l == Wool::Library::LP or l == Wool::Library::TFHEBool);
    }

    bool M::isWellSuited(Wool::Library l, int i) {
        return !(l == Wool::Library::TFHEBool);
    }

    bool M::isWellSuited(Wool::Library l, bool i) {
        return !(l == Wool::Library::SEALCKKS);
    }

    bool M::isPlaintext() {
        return this->plaintext;
    }

}
