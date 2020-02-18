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

    enum Library {Plaintext, HElib, LP, Palisade, SEALBFV, SEALCKKS, TFHEBool, TFHECommon, TFHEInteger};

    // Enc and Dec
    M encrypt(long value, Library library);

    // Output

    void output(M value);

    class M {
    public:
        /// The method generating an AST from some function written with M classes.
        static Ast* make_AST(std::function<void()> f);

        /// Method for multdepth analysis will probably be within Wool. (Wrapper for Wool)
        /// \return Maximum multiplicative depth of circuit composed from AST.
        static int analyse(std::function<void()> f);

        /// Method for evaluation/benchmarking will also be in Wool. (Wrapper for Wool, which wraps SHEEP)
        /// TODO: with which library will evaluation happen? Can we select?
        /// \return duration elapsed in milleseconds
        static double evaluate(std::function<void()> f);

        /// Return statement imitation
        static void output(const M &m);

        /// Destructor;
        ~M();

        /// Dummy contructor
        M();

        /// full constructor
        M(long value, Library library, bool plaintext);

        /// Copy constructor
        M(const M &other);

        /// Move constructor
        M(M &&other);

        /// Plaintext constructor
        M(long i);

        /// Plaintext constructor
        M(int i);

        /// Plaintext constructor
        M(bool b);

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
        M &operator+=(const long &rhs);

        /// Addition
        M &operator+=(const int &rhs);

        /// Subtraction
        M &operator-=(const M &rhs);

        /// Multiplication
        M &operator*=(const M &rhs);

        /// Multiplication
        M &operator*=(long &rhs);

        /// Multiplication
        M &operator*=(int &rhs);

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

        /// \return plaintext bool, true if the value is plaintext
        bool isPlaintext();

        /// \return library to be used at evaluation time
        Library getLib();

        /// set Library to be used at evaluation time
        void setLib();

        /// \return the pointer to the Expression responsible for its construction
        AbstractExpr* getExpr();

    private:
        /// This is the (temporary) Ast used by output(M m) to return the final AST to make_AST
        static Ast *output_ast;

        /// a pointer to the AST responsible for its construction
        AbstractExpr *expr;

        /// Library to use when evaluating
        Library library;

        /// true, if value is a plaintext
        bool plaintext;

        /// Direct init with AST
        M(AbstractExpr &expr, bool plaintext);

        /// \return true, if the library is well suited for int
        bool isWellSuited(Library library, int i);

        /// \return true, if the library is well suited for long
        bool isWellSuited(Library library, long l);

        /// \return true, if the library is well suited for bool
        bool isWellSuited(Library library, bool b);

        /// This function tries to resolve user selected library choices and default libraries s.t. a user selection "wins" over default libraries
        /// and a runtime error is thrown, if there are contradicting constraints.
        /// Default library is Plaintext.
        /// \param l
        /// \param r
        /// \return the library, which "won"
        Library resolveLibraries(Library l, Library r);


        friend M encrypt(long value, Library library);

        friend Ast make_AST(std::function<void()> f);

        /// computes string from library enum
        /// \param l
        /// \return string to corresponding library enum
        std::string toString(Library l);

    };

};

#endif //M_HPP
