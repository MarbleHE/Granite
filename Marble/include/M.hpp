//
// Created by mario on 08.01.20.
//
#ifndef M_HPP
#define M_HPP

#include <vector>
#include <string>
#include <functional>

#include "AbstractExpr.h"

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

    M encrypt(SelectorType batched, std::vector<long> value, int bitSize = 32, bool twos_complement = true);

    M encrypt(SelectorType batched, std::vector<bool> value, int bitSize = 1, bool twos_complement = false);

    M encrypt(SelectorType batched, std::vector<int> value, int bitSize = 32, bool twos_complement = true);

    M encrypt(long value, int bitSize, bool twos_complement);

    std::vector<M> encrypt(std::vector<long> value, int bitSize = 32, bool twos_complement = true);

    std::vector<M> encrypt(std::vector<bool> value, int bitSize = 1, bool twos_complement = false);

    M encode(SelectorType batched, long value, int bitSize = 32, bool twos_complement = true);

    M encode(SelectorType batched, std::vector<long> value, int bitSize = 32, bool twos_complement = true);

    M encode(SelectorType batched, std::vector<int> value, int bitSize = 32, bool twos_complement = true);


    // Output

    void output(M value);

    class M {
    public:
        /// a pointer to the AST responsible for its construction
        AbstractExpr* expr;

        /// The method generating an AST from some function written with M classes.
        static Ast make_AST(std::function<void()> f);

        /// Method for multdepth analysis will probably be within Wool. (Wrapper for Wool)
        /// @return Maximum multiplicative depth of circuit composed from AST.
        static int analyse(std::function<void()> f);

        /// Method for evaluation/benchmarking will also be in Wool. (Wrapper for Wool, which wraps SHEEP)
        /// TODO: with which library will evaluation happen? Can we select?
        /// @return duration elapsed in milleseconds
        static double evaluate(std::function<void()> f);

        /// Return statement imitation
        static void output(M m);

        /// Destructor;
        ~M();

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
/*
        /// Addition
        M &operator+=(const M &rhs);

        /// Addition
        M &operator+=(const std::vector<long> &rhs);

        /// Subtraction
        M &operator-=(const M &rhs);

        /// Multiplication
        M &operator*=(const M &rhs);

        /// Multiplication
        M &operator*=(std::vector<long> &rhs);

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
*/
        ///
    private:
        /// This is the (temporary) Ast used to build the finally returned Ast by make_AST
        static Ast output_ast;

        bool plaintext;

        /// Direct init with AST
        M(AbstractExpr& expr, bool plaintext);

        
/*
        friend M encrypt(SelectorType batched, long value, int bitSize, bool twos_complement);

        friend M encode(SelectorType batched, long value, int bitSize, bool twos_complement);


        friend M encrypt(SelectorType batched, std::vector<long> value, int bitSize, bool twos_complement);

        friend M encode(SelectorType batched, std::vector<long> value, int bitSize, bool twos_complement);

        friend std::vector<M> encrypt(std::vector<long> values, int bitSize, bool twos_complement);

        friend std::vector<M> encrypt(std::vector<bool> values, int bitSize, bool twos_complement);

        friend M encrypt(long value, int bitSize, bool twos_complement);
*/
        friend Ast make_AST(std::function<void()> f);

        void enc_if_needed();

    };

};

#endif //M_HPP
