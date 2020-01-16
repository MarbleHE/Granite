#include "M.hpp"
#include "BitHelpers.h"

using namespace std;

namespace Marble {

    // Initialize static members

    Ast M::make_AST(std::function<void()> f) {
        M::ast = Ast();
        ast.add_function(f);
        f();
        return Ast();
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
        return *this;
    }

    M &M::operator=(M &&other) {
        bitSize = other.bitSize;
        plaintext = other.plaintext;
        values = other.values;
        twos_complement = other.twos_complement;
        return *this;
    }

    M &M::operator=(long i) {
        this->values = std::vector<long>(1, i);
        this->bitSize = ceil_log2(i) + 1;
        this->twos_complement = (i < 0);
        this->plaintext = true;
        return *this;
    }

    M &M::operator=(bool b) {
        *this = encrypt(SelectorType(), b, 1, false);
        return *this;
    }

    M &M::operator=(int i) {
        *this = encrypt(SelectorType(), i, 32, true);
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
