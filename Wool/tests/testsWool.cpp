//
// Created by mario on 17.02.20.
//
#include "gtest/gtest.h"
#include "circuit.hpp"
#include "M.hpp"
#include "Ast.h"

using namespace Marble;
using namespace Wool;

TEST(BasicTest, GoogleTestImport) {
  ASSERT_EQ(1, 1);
}

TEST(BasicSHEEP, CircuitImport) {
  ASSERT_NO_THROW(Circuit());
}

void f_1(){
    M a = encrypt(5);
    M b = 20;
    output(a+b);
}


TEST(BasicWool, EvalEasy) {
    Ast* ast = M::makeAST(f_1);
    W w = W(*ast);
    long res = w.evaluateWith(Library::Plaintext);
    ASSERT_EQ(res, 25);
}

TEST(BasicWool, EvalSEALBFV){
    Ast* ast = M::makeAST(f_1);
    W w = W(*ast);
    long res = w.evaluateWith(Library::SEALBFV);
    ASSERT_EQ(res, 25);
}

/// maximum multiplicative depth should be 3
void f_md(){
    M a = 10;
    a *= 10;
    a *= 10;
    a *= 10;
    output(a);
}

TEST(BasicWool, MultDepthTest){
    W w = W(*M::makeAST(f_md));
    ASSERT_EQ(w.getMultDepth(), 3);
}