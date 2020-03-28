//
// Created by mario on 17.02.20.
//
#include "gtest/gtest.h"
#include "circuit.hpp"
#include "G.hpp"
#include "Ast.h"
#include "BatchingVisitor.hpp"
#include "Return.h"
#include "Function.h"
#include "simple-circuits.hpp"
#include "circuit-util.hpp"
#include "context-clear.hpp"
#include "CircuitHelpers.hpp"
#include "context-tfhe-bool.hpp"

using namespace Granite;
using namespace Wool;
using namespace std;

TEST(BasicTest, GoogleTestImport) {
  ASSERT_EQ(1, 1);
}

TEST(BasicSHEEP, CircuitImport) {
  ASSERT_NO_THROW(Circuit());
}

void f_1(){
    G a = encrypt(5);
    G b = 20;
    output(a+b);
}


TEST(BasicWool, EvalEasy) {
    Ast* ast = G::makeAST(f_1);
    W w = W(*ast);
    long res = w.evaluateWith(Library::Plaintext);
    ASSERT_EQ(res, 25);
}

TEST(BasicWool, EvalSEALBFV){
    Ast* ast = G::makeAST(f_1);
    W w = W(*ast);
    long res = w.evaluateWith(Library::SEALBFV);
    ASSERT_EQ(res, 25);
}

/// maximum multiplicative depth should be 3
void f_md(){
    G a = 10;
    a *= 10;
    a *= 10;
    a *= 10;
    output(a);
}

TEST(BasicWool, MultDepthTest){
    W w = W(*G::makeAST(f_md));
    ASSERT_EQ(w.getMultDepth(), 3);
}

void f_dec(){
    G a = 10;
    G b = encrypt(20);
    long c = decrypt(a*b);
    c += 10;
    G x = encrypt(c);
    G y = 1;
    output(x + y);
}

TEST(BasicWool, DecTest){
    Ast* ast = G::makeAST(f_dec);
    W w = W(*ast);
    long res = w.evaluateWith(Library::SEALBFV);
    ASSERT_EQ(res, 211);
}

void f_dec_setlibBFV(){
    G a = 10;
    G b = encrypt(20);
    b.setLib(Library::SEALBFV);
    long c = decrypt(a*b);
    c += 10;
    G x = encrypt(c);
    G y = 1;
    output(x + y);
}

TEST(BasicWool, DecTestsetLibBFV){
    Ast* ast = G::makeAST(f_dec_setlibBFV);
    W w = W(*ast);
    long res = w.evaluateWith(Library::Plaintext);
    ASSERT_EQ(res, 211);
}

void f_batch(){
    G a = batchEncrypt((std::vector <int>){1, 2, 3});
    output(a);
}

TEST(BatchingTest, VisitorTest){
    Ast* ast = G::makeAST(f_batch);
    Function* f = (Function *) ast->getRootNode();
    Return* r = (Return *) f->getBodyStatements()[0];
    AbstractExpr * ae = r->getReturnExpressions()[0];
    BatchingVisitor bv;
    bv.visit(*ae);
    ASSERT_EQ(3,bv.getMaxSlots());
    ASSERT_EQ(0,bv.getSndMaxSlots());
}

void f_batch_add(){
    G a = batchEncrypt(std::vector<int>{1, 2, 3});
    G b = batchEncrypt(std::vector<int>{1, 2, 3});
    output(a+b);
}

TEST(BasicSlotSize, BatchingVisitorTest){
    Ast* ast = G::makeAST(f_batch_add);
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


class LogicCircuitTest : public ::testing::Test {
protected:
    Circuit c;
    BaseContext<bool>* ctx;
    DurationContainer dc;
    vector<vector<vector<bool>>> inputs;
    vector<vector<bool>> results;
    void SetUp() override {
        ctx = new SHEEP::ContextClear<bool>();
        inputs = {{{0},{0}},
                  {{0},{1}},
                  {{1},{0}},
                  {{1},{1}}};
    }
    void TearDown() override {
        delete ctx;
    }
    void eval(){
        for (auto x : inputs){
            results.push_back(ctx->eval_with_plaintexts(c,x,dc)[0]);
        }
    }
    void verify(bool oo, bool oi, bool io, bool ii){
        EXPECT_EQ(results[0][0],oo);
        EXPECT_EQ(results[1][0], oi);
        EXPECT_EQ(results[2][0], io);
        EXPECT_EQ(results[3][0], ii);
    }
};

TEST_F(LogicCircuitTest, BoolAdd){
    c = single_binary_gate_circuit(Gate::Add);
    eval();
    verify(0,1,1,0);
}

TEST_F(LogicCircuitTest, BoolMultiply){
    c = single_binary_gate_circuit(Gate::Multiply);
    eval();
    verify(0,0,0,1);
}

TEST_F(LogicCircuitTest, BoolNegate){
    c = single_unary_gate_circuit(Gate::Negate);
    EXPECT_EQ(ctx->eval_with_plaintexts(c,{{0}},dc)[0][0],1);
    EXPECT_EQ(ctx->eval_with_plaintexts(c,{{1}},dc)[0][0],0);
}

TEST_F(LogicCircuitTest, BoolEqual) {
    c = booleanEqualCircuit();
    eval();
    verify(1,0,0,1);
}

TEST_F(LogicCircuitTest, BoolOr){
    c = booleanOrCircuit();
    eval();
    verify(0,1,1,1);
}

TEST_F(LogicCircuitTest, BoolCompare){
    c = single_binary_gate_circuit(Gate::Compare);
    eval();
    verify(0,0,1,0);
}


class ArithmeticCircuitTest : public ::testing::Test {
protected:
    Circuit c;
    BaseContext<int64_t>* ctx;
    DurationContainer dc;
    vector<vector<vector<long>>> inputs;
    vector<vector<long>> results;
    void SetUp() override {
        ctx = new SHEEP::ContextClear<int64_t>();
        inputs = {{{33},{33}},
                  {{42},{1}},
                  {{1},{42}},
                  {{-1},{1}},
                  {{-22},{-11}},
                  {{-33},{-33}}};
    }
    void TearDown() override {
        delete ctx;
    }
    void eval(){
        for (auto x : inputs){
            results.push_back(ctx->eval_with_plaintexts(c,x,dc)[0]);
        }
    }
    void verify(vector<long> v){
        EXPECT_EQ(results[0][0],v[0]);
        EXPECT_EQ(results[1][0], v[1]);
        EXPECT_EQ(results[2][0], v[2]);
        EXPECT_EQ(results[3][0], v[3]);
        EXPECT_EQ(results[4][0],v[4]);
        EXPECT_EQ(results[5][0], v[5]);
    }
};

TEST_F(ArithmeticCircuitTest, GreaterTest){
    c = single_binary_gate_circuit(Gate::Compare);
    eval();
    verify({0,1,0,0,0,0,});
}

TEST_F(ArithmeticCircuitTest, SmallerTest){
    c = smallerCircuit();
    eval();
    verify({0,0,1,1,1,0});
}


TEST_F(ArithmeticCircuitTest, UnequalTest){
    c = unequalCircuit();
    eval();
    verify({0,1,1,1,1,0});
}

