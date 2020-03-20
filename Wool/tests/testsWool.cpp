//
// Created by mario on 17.02.20.
//
#include "gtest/gtest.h"
#include "circuit.hpp"
#include "M.hpp"
#include "Ast.h"
#include "BatchingVisitor.hpp"
#include "Return.h"
#include "Function.h"
#include "simple-circuits.hpp"
#include "circuit-util.hpp"
#include "context-clear.hpp"
#include "CircuitHelpers.hpp"

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

void f_batch(){
    M a = batchEncrypt((std::vector <int>){1,2,3});
    output(a);
}

TEST(BatchingTest, VisitorTest){
    Ast* ast = M::makeAST(f_batch);
    Function* f = (Function *) ast->getRootNode();
    Return* r = (Return *) f->getBodyStatements()[0];
    AbstractExpr * ae = r->getReturnExpressions()[0];
    BatchingVisitor bv;
    bv.visit(*ae);
    ASSERT_EQ(3,bv.getMaxSlots());
    ASSERT_EQ(0,bv.getSndMaxSlots());
}

void f_batch_add(){
    M a = batchEncrypt(std::vector<int>{1,2,3});
    M b = batchEncrypt(std::vector<int>{1,2,3});
    output(a+b);
}

TEST(BasicSlotSize, BatchingVisitorTest){
    Ast* ast = M::makeAST(f_batch_add);
    Function* f = (Function *) ast->getRootNode();
    Return* r = (Return *) f->getBodyStatements()[0];
    AbstractExpr * ae = r->getReturnExpressions()[0];
    BatchingVisitor bv;
    bv.visit(*ae);
    ASSERT_EQ(3,bv.getMaxSlots());
    ASSERT_EQ(0,bv.getSndMaxSlots());
}



TEST(BasicGate, RotateTest){
    Circuit c = rotateCircuit();
    std::vector<std::vector<long>> ptv = {{1,42,333}};
    std::vector<long> cptv = {2};
    DurationContainer dc;
    auto ctx = SHEEP::ContextClear<int64_t >();
    auto resv = ctx.eval_with_plaintexts(c, ptv, cptv, dc);
    for (auto x: resv[0]){
        std::cout << std::to_string(x) << " ";
    }
}

TEST(SHEEPconstIn, ParTest){
    Circuit c = par(single_unary_gate_circuit(Gate::Alias), constAliasCircuit());
    std::cout << c;
}

TEST(SHEEPconstIn, SeqTest){
    Circuit c = seq(single_unary_gate_circuit(Gate::Alias), rotateCircuit());
    std::cout << c;
}