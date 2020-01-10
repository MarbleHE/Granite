//
// Created by mario on 08.01.20.
//
#ifndef M_HPP
#define M_HPP

#include <vector>
#include <string>
#include <functional>

#include "Ast.h"

namespace Marble {
    class M;

    struct SelectorType {
        int x = -1;
    };

    /// Optimizations/shortcuts for fold functions
    extern struct SelectorType sum;
    extern struct SelectorType min;
    extern struct SelectorType min_with_index;
    extern struct SelectorType batched;

    // Convenience functions with meaningful names (instead of just constructors)

    M encrypt(SelectorType batched, long value, int bitSize, bool twos_complement);

    M encrypt(SelectorType batched, vector<long> value, int bitSize = 32, bool twos_complement = true);

    M encrypt(SelectorType batched, vector<bool> value, int bitSize = 1, bool twos_complement = false);

    M encrypt(SelectorType batched, vector<int> value, int bitSize = 32, bool twos_complement = true);

    M encrypt(long value, int bitSize, bool twos_complement);

    vector<M> encrypt(vector<long> value, int bitSize = 32, bool twos_complement = true);

    vector<M> encrypt(vector<bool> value, int bitSize = 1, bool twos_complement = false);

    M encode(SelectorType batched, long value, int bitSize = 32, bool twos_complement = true);

    M encode(SelectorType batched, vector<long> value, int bitSize = 32, bool twos_complement = true);

    M encode(SelectorType batched, vector<int> value, int bitSize = 32, bool twos_complement = true);


    // Output

    void output(M value, string msg = "", int slot = -1);

    class M{
    public:
        /// The method generating an AST from some function written with M classes.
        static Ast make_AST(std::function<void()> f);

        /// TODO Method for multdepth analysis will probably be within Wool.
        /// TODO Method for evaluation/benchmarking will also be in Wool.

        /// Dummy contructor
        M();

        /// Copy constructor
        M(const M &other);

        /// Move constructor
        M(M &&other);

        /// Plaintext constructor
        M(long i);

        /// Copy assignment
        M &operator=(const M &other);

        /// Move assignment
        M &operator=(M &&other);

        /// Plaintext encode assignment
        M &operator=(long i);

        /// Encrypt bool
        M &operator=(bool b);

        /// Encrypt int
        M &operator=(int i);

        /// Addition
        M &operator+=(const M &rhs);

        /// Addition
        M &operator+=(const vector<long> &rhs);

        /// Subtraction
        M &operator-=(const M &rhs);

        /// Multiplication
        M &operator*=(const M &rhs);

        /// Multiplication
        M &operator*=(vector<long> &rhs);

        /// Increment operator
        M &operator++();

        /// Decrement operator
        M &operator--();

        /// Increment operator
        M operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        /// Decrement operator
        M operator--(int) {
            auto tmp = *this;
            operator--();
            return tmp;
        }

        /// Logical negation operator
        M &operator!();

        /// Equality
        friend M operator==(const M &lhs, const M &rhs);

        /// Inequality
        friend M operator!=(const M &lhs, const M &rhs);

        /// Greater-than-or-equal
        friend M operator>=(const M &lhs, const M &rhs);

        /// Greater-than
        friend M operator>(const M &lhs, const M &rhs);

        /// Smaller-than-or-equal
        friend M operator<=(const M &lhs, const M &rhs);

        /// Smaller-than
        friend M operator<(const M &lhs, const M &rhs);

        /// Addition
        friend M operator+(const M &lhs, const M &rhs);

        /// Subtraction
        friend M operator-(const M &lhs, const M &rhs);

        /// Multiplication
        friend M operator*(const M &lhs, const M &rhs);

        ///
    private:
        /// This is the (temporary) Ast used to build the finally returned Ast by make_AST
        static Ast ast;

        vector<long> values;
        int bitSize = -1;
        bool twos_complement;
        bool plaintext;

        /// Single value, replicated across all slots
        M(long value, int bitSize, bool twos_complement, bool plaintext);

        /// Different values in different slots
        M(vector<long> values, int bitSize, bool twos_complement, bool plaintext);


        friend M encrypt(SelectorType batched, long value, int bitSize, bool twos_complement);

        friend M encode(SelectorType batched, long value, int bitSize, bool twos_complement);


        friend M encrypt(SelectorType batched, vector<long> value, int bitSize, bool twos_complement);

        friend M encode(SelectorType batched, vector<long> value, int bitSize, bool twos_complement);

        friend vector<M> encrypt(vector<long> values, int bitSize, bool twos_complement);

        friend vector<M> encrypt(vector<bool> values, int bitSize, bool twos_complement);

        friend M encrypt(long value, int bitSize, bool twos_complement);

        friend void output(M value, string msg, int slot);

        friend Ast make_AST(std::function<void()> f);

        void enc_if_needed();

    };

};

#endif //M_HPP
