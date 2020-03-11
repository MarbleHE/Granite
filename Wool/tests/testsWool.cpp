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

void f_dec(){
    M a = 10;
    M b = encrypt(20);
    long c = decrypt(a*b);
    c += 10;
    M x = encrypt(c);
    M y = 1;
    output(x + y);
}

TEST(BasicWool, DecTest){
    Ast* ast = M::makeAST(f_dec);
    W w = W(*ast);
    long res = w.evaluateWith(Library::SEALBFV);
    ASSERT_EQ(res, 211);
}

void f_dec_setlibBFV(){
    M a = 10;
    M b = encrypt(20);
    b.setLib(Library::SEALBFV);
    long c = decrypt(a*b);
    c += 10;
    M x = encrypt(c);
    M y = 1;
    output(x + y);
}

TEST(BasicWool, DecTestsetLibBFV){
    Ast* ast = M::makeAST(f_dec_setlibBFV);
    W w = W(*ast);
    long res = w.evaluateWith(Library::Plaintext);
    ASSERT_EQ(res, 211);
}