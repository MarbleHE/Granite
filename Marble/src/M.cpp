#include "M.hpp"
#include "BitHelpers.h"

using namespace std;

namespace Marble {

    // Initialize static members
    int M::fid = 0;
    int M::vid = 0;
    Ast M::ast;
    Function* M::current_f;

    Ast M::make_AST(std::function<void()> f) {
        M::ast = Ast(); // clear Ast
        string f_name = "f_" + to_string(M::fid);
        M::fid++;
        M::current_f = dynamic_cast<Function*>(M::ast.setRootNode(new Function(f_name)));
        f(); // execute function to parse by self-evaluation with the help of the operators defined by M
        return M::ast;
    }

    M::M() {
        this->values = std::vector<long>(1, 0);
        this->bitSize = 1;
        this->twos_complement = false;
        this->plaintext = true;
    }

    M::M(const M &other) {
        bitSize = other.bitSize;
        plaintext = other.plaintext;
        values = other.values;
        twos_complement = other.twos_complement;
    }

    M::M(M &&other) {
        bitSize = other.bitSize;
        plaintext = other.plaintext;
        values = other.values;
        twos_complement = other.twos_complement;
    }

    M::M(long i) {
        this->values = std::vector<long>(1, i);
        this->bitSize = ceil_log2(i) + 1;
        this->twos_complement = (i < 0);
        this->plaintext = true;
    }

    M &M::operator=(const M &other) {
        bitSize = other.bitSize;
        plaintext = other.plaintext;
        values = other.values;
        twos_complement = other.twos_complement;
        if(uid == 0){
            M::current_f->addStatement(new VarDecl(toExpr(other)));
        }
        else
        {
            M::current_f->addStatement(new VarAssign(toExpr(other)));
            uid++;
        }
        return *this;
    }

    M &M::operator=(M &&other) {
        bitSize = other.bitSize;
        plaintext = other.plaintext;
        values = other.values;
        twos_complement = other.twos_complement;
        if(uid == 0){
            M::current_f->addStatement(new VarDecl(toExpr(other)));
            varid = M::vid++;
        }
        else
        {
            M::current_f->addStatement(new VarAssign(toExpr(other)));
            uid++;
        }
        return *this;
    }

    M &M::operator=(long i) {
        this->values = std::vector<long>(1, i);
        this->bitSize = ceil_log2(i) + 1;
        this->twos_complement = (i < 0);
        this->plaintext = true;
        this->varid = M::vid++;
        return *this;
    }

    M &M::operator=(bool b) {
        *this = encrypt(SelectorType(), b, 1, false);
        if(uid == 0){
            M::current_f->addStatement(new VarDecl(to_string(M::vid), b));
            this->varid = M::vid++;
        }
        else
        {
            M::current_f->addStatement(new VarAssign(to_string(this.varid), b));
            uid++;
        }
        return *this;
    }

    M &M::operator=(int i) {
        *this = encrypt(SelectorType(), i, 32, true);
        if(uid == 0){
            M::current_f->addStatement(new VarDecl(to_string(M::vid), i));
            this->varid = M::vid++;
        }
        else
        {
            M::current_f->addStatement(new VarAssign(to_string(this.varid), i));
            uid++;
        }
        return *this;
    }
/*
    M &M::operator+=(const M &rhs) {
        return <#initializer#>;
    }

    M &M::operator+=(const std::vector<long> &rhs) {
        return <#initializer#>;
    }

    M &M::operator-=(const M &rhs) {
        return <#initializer#>;
    }

    M &M::operator*=(const M &rhs) {
        return <#initializer#>;
    }

    M &M::operator*=(std::vector<long> &rhs) {
        return <#initializer#>;
    }

    M &M::operator++() {
        return <#initializer#>;
    }

    M &M::operator--() {
        return <#initializer#>;
    }

    M &M::operator!() {
        return <#initializer#>;
    }
*/


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

    M::M(long value, int bitSize, bool twos_complement, bool plaintext) {
        this->values = std::vector<long>(1, value);
        this->bitSize = bitSize;
        this->twos_complement = twos_complement;
        this->plaintext = plaintext;
    }

    M::M(std::vector<long> values, int bitSize, bool twos_complement, bool plaintext) {
        this->values = values;
        this->bitSize = bitSize;
        this->twos_complement = twos_complement;
        this->plaintext = plaintext;
    }

    void M::enc_if_needed() {

    }
}
