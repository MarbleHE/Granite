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

using namespace std;

namespace Marble {

    // Initialize static member
    Ast M::output_ast;

    Ast M::make_AST(std::function<void()> f) {
        M::output_ast = Ast(); // clear Ast
        f(); // execute function to parse by self-evaluation with the help of the operators defined by M. The output(M m) function sets M::output_ast
        return M::output_ast;
    }

    void M::output(M m){
        M::output_ast = Ast();
        Function* func = dynamic_cast<Function*>(M::output_ast.setRootNode(new Function("f")));
        func->addStatement(new Return(m.expr));
    }

    M::M() {
        this->plaintext = true;
        this->expr = new Variable("_var_");
    }

    M::M(const M &other) {
        plaintext = other.plaintext;
        expr = other.expr;
    }

    M::M(M &&other) {
        plaintext = other.plaintext;
        expr = other.expr;
    }

    M::M(long i) {
        this->plaintext = true;
        this->expr = new LiteralInt(i); //TODO: long to int conversion is not a good idea. change LiteralInt to support longs
    }

    M &M::operator=(const M &other) {
        plaintext = other.plaintext;
        expr = other.expr;
        return *this;
    }

    M &M::operator=(M &&other) {
        plaintext = other.plaintext;
        expr = other.expr;
        return *this;
    }

    M &M::operator=(long i) {
        this->plaintext = true;
        this->expr = new LiteralInt(i);
        return *this;
    }

    M &M::operator=(bool b) {
        //*this = encrypt(SelectorType(), b, 1, false);
        this->plaintext = true;
        this->expr = new LiteralBool(b);
        return *this;
    }

    M &M::operator=(int i) {
        //*this = encrypt(SelectorType(), i, 32, true);
        this->plaintext = true;
        this->expr = new LiteralInt(i);
        return *this;
    }

    M &M::operator+=(const M &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::addition, rhs.expr);
        this->expr = exp;
        return *this;
    }

    M &M::operator+=(const long &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::addition, new LiteralInt(rhs));
        this->expr = exp;
        return *this;
    }

    M &M::operator-=(const M &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::subtraction, rhs);
        this->expr = exp;
        return *this;
    }

    //TODO: -= long &rhs missing?

    M &M::operator*=(const M &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::multiplication, rhs);
        this->expr = exp;
        return *this;
    }

    M &M::operator*=(long &rhs) {
        auto exp = new BinaryExpr(this->expr, OpSymb::BinaryOp::multiplication, new LiteralInt(rhs));
        this->expr = exp;
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


/*
    M encode(SelectorType batched, long value, int bitSize, bool twos_complement) {
        assert(bitSize > 0);
        return M(value, bitSize, twos_complement, true);

    }

    M encrypt(SelectorType batched, long value, int bitSize, bool twos_complement) {
        assert(bitSize > 0);
        return M(value, bitSize, twos_complement, false);
    }

    M encrypt(SelectorType batched, vector<int> values, int bitSize, bool twos_complement) {
        vector<long> v(values.size());
        for (int i = 0; i < values.size(); ++i) {
            v[i] = values[i];
        }
        return encrypt(batched, v, bitSize, twos_complement);
    }

    M encode(SelectorType batched, vector<long> values, int bitSize, bool twos_complement) {
        assert(bitSize > 0);
        return M(values, bitSize, twos_complement, true);
    }

    M encode(SelectorType batched, vector<int> values, int bitSize, bool twos_complement) {
        vector<long> v(values.size());
        for (int i = 0; i < values.size(); ++i) {
            v[i] = values[i];
        }
        return encode(batched, v, bitSize, twos_complement);
    }

    M encrypt(SelectorType batched, vector<long> values, int bitSize, bool twos_complement) {
        assert(bitSize > 0);
        return M(values, bitSize, twos_complement, false);
    }

    M encrypt(SelectorType batched, vector<bool> values, int bitSize, bool twos_complement) {
        vector<long> v(values.size());
        for (int i = 0; i < values.size(); ++i) {
            v[i] = values[i];
        }
        return encrypt(SelectorType(), v, bitSize, twos_complement);
    }

    M encrypt(long value, int bitSize, bool twos_complement) {
        return M(value, bitSize, twos_complement, false);
    }


    vector<M> encrypt(vector<long> values, int bitSize, bool twos_complement) {
        vector<M> vs;
        for (int i = 0; i < values.size(); ++i) {
            M t = M(values[i], bitSize, twos_complement, false);
            vs.emplace_back(t);
        }
        return vs;
    }

    vector<M> encrypt(vector<bool> values, int bitSize, bool twos_complement) {
        vector<long> v(values.size());
        for (int i = 0; i < values.size(); ++i) {
            v[i] = values[i];
        }
        return encrypt(v, bitSize, twos_complement);
    }
*/
    M::M(AbstractExpr& expr, bool plaintext) {
        this->expr = &expr;
        this->plaintext = plaintext;
    }

    M::~M(){
        if (!this->expr){
            delete this->expr;
        }
    }

    void M::enc_if_needed() {

    }
}
