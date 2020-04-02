//
// Created by mario on 08.01.20.
//
#include <fstream>
#include <string>
#include "gtest/gtest.h"
#include "Ast.h"
#include "AbstractBinaryExpr.h"
#include "G.hpp"
#include "nlohmann/json.hpp"
#include "Function.h"
#include "Return.h"
#include "Wool.hpp"
#include "PadVisitor.hpp"

using json = nlohmann::json;
using namespace Granite;

std::string expected_output = "../../../Granite/tests/expected_output/";

TEST(BasicTest, GoogleTestImport) {
  ASSERT_EQ(1, 1);
}

TEST(BasicAst, AstImport) {
  ASSERT_NO_THROW(Ast());
}

TEST(BasicM, MImport) {
  ASSERT_NO_THROW(Granite::G());
}

TEST(MTest, EmptyConstructorExprTest) {
  std::ifstream file(expected_output + "empty_constructor_expr.json");
  json expected_json = json::parse(file);
  EXPECT_EQ(G().getExpr()->toJson().dump(), expected_json.dump());

  G a = G();
  EXPECT_EQ(a.getExpr()->toJson().dump(), expected_json.dump());

  G b;
  EXPECT_EQ(b.getExpr()->toJson().dump(), expected_json.dump());
}

TEST(MTest, LiteralIntExprTest) {
  std::ifstream file(expected_output + "literal_int_expr.json");
  json expected_json = json::parse(file);
  EXPECT_EQ(G(42).getExpr()->toJson().dump(), expected_json.dump());

  G a = G(42);
  EXPECT_EQ(a.getExpr()->toJson().dump(), expected_json.dump());

  G b = 42;
  EXPECT_EQ(b.getExpr()->toJson().dump(), expected_json.dump());
}

TEST(MTest, LiteralBoolExprTest) {
  std::ifstream file(expected_output + "literal_bool_expr.json");
  json expected_json = json::parse(file);
  EXPECT_EQ(G(true).getExpr()->toJson().dump(), expected_json.dump());

  G a = G(true);
  EXPECT_EQ(a.getExpr()->toJson().dump(), expected_json.dump());

  G b = G(true);
  EXPECT_EQ(b.getExpr()->toJson().dump(), expected_json.dump());
}

TEST(MTest, ReturnLiteralIntTest) {
  G m = 42;
  Ast ast = Ast();
  Function *func = dynamic_cast<Function *>(ast.setRootNode(new Function("f")));
  std::ifstream file(expected_output + "return_literal_int.json");
  json expected_json = json::parse(file);

  func->addStatement(new Return((m.getExpr())));

  EXPECT_EQ(ast.getRootNode()->toJson().dump(), expected_json.dump());
}

void empty_func() {
  G::output(G());
}

void empty_func2() {
  G x = G();
  G::output(x);
}

void literalInt_func() {
  G::output(G(42));
}

void literalInt_func2() {
  G a = 42;
  G::output(a);
}

class FTest : public ::testing::Test {
 protected:
  std::function<void()> f;
};

TEST_F(FTest, EmptyFuncAST) {
  f = empty_func;
  std::ifstream file(expected_output + "empty_func.json");
  json expected_j = json::parse(file);

  Ast *ast = G::makeAST(f);
  auto rn = ast->getRootNode();

  EXPECT_EQ(expected_j.dump(), rn->toString(true));
}

TEST_F(FTest, EmptyFuncEquality) {
  f = empty_func;
  Ast *ast = G::makeAST(f);
  Ast *ast2 = G::makeAST(empty_func2);
  auto rn = ast->getRootNode();
  auto rn2 = ast2->getRootNode();
  EXPECT_EQ(rn->toString(true), rn2->toString(true));
}

TEST_F(FTest, LiteralIntEquality) {
  f = literalInt_func;
  Ast *ast = G::makeAST(f);
  Ast *ast2 = G::makeAST(literalInt_func2);
  auto rn = ast->getRootNode();
  auto rn2 = ast->getRootNode();
  std::ifstream file(expected_output + "return_literal_int.json");
  json expected_j = json::parse(file);

  EXPECT_EQ(rn->toString(true), rn2->toString(true));
  EXPECT_EQ(rn->toString(true), expected_j.dump());
}

TEST(EncTest, WithLib) {
  G m = encrypt(5, Wool::Library::HElib);
  EXPECT_EQ(m.getLib(), Wool::Library::HElib);
  EXPECT_EQ(m.isPlaintext(), false);
}

TEST(DecTest, Plaintext) {
  G a = encrypt(4);
  long res = decrypt(a);
  EXPECT_EQ(res, 4);
}


void f_dec_setlibBFV(){
    G a = 10;
    G b = encrypt(20);
    b.setLib(Wool::Library::SEALBFV);
    long c = decrypt(a*b);
    c += 10;
    G x = encrypt(c);
    G y = 1;
    output(x + y);
}

TEST(EvalTest, DecTestsetLibBFV){
    long ms = G::evaluate(f_dec_setlibBFV, Wool::Library::SEALBFV);
    std::cout << ms;
}

/// maximum multiplicative depth should be 3
void f_md(){
    G a = 10;
    a *= 10;
    a *= 10;
    a *= 10;
    output(a);
}

TEST(BasicMultDepth, MultDepthTest){
    ASSERT_EQ(G::analyse(f_md), 3);
}

void f_batch(){
    G a = batchEncrypt(std::vector<int>{1, 2, 3});
    output(a);
}

TEST(BasicBatch, PadTest){
    Ast* ast = G::makeAST(f_batch);
    pad(ast, Wool::Library::SEALBFV);
    //std::cout << ast->getRootNode()->toJson().dump();
}

void f_batch_add(){
    G a = batchEncrypt(std::vector<int>{1, 2, 3, 4});
    G b = batchEncrypt(std::vector<int>{2, 4, 6, 8});
    output(a+b);
}

TEST(BasicBatch, PadAddTest){
    Ast* ast = G::makeAST(f_batch_add);
    //std::cout << ast->getRootNode()->toJson().dump();
    pad(ast, Wool::Library::SEALBFV);
    //std::cout << ast->getRootNode()->toJson().dump();
}

TEST(BasicBatch, AddResultTest){
    ASSERT_EQ(3, G::result(f_batch_add, Wool::SEALBFV));
}

void f_fold(){
    G a = batchEncrypt(std::vector<int>{1, 2, 3, 4});
    output(a.fold(G::sum));
}

TEST(BasicBatch, RotateTest){
    Ast* ast = G::makeAST(f_fold);
    std::ifstream file(expected_output + "f_fold.json");
    json expected_j = json::parse(file);
    ASSERT_EQ(ast->getRootNode()->toJson().dump(),expected_j.dump());
}

TEST(BasicBatch, FoldTest){
    ASSERT_EQ(G::result(f_fold, Wool::Library::Plaintext), 10);
}

void f_fold2(){
    G a = batchEncrypt(std::vector<int> {0,1,1,1,1,0});
    output(a.fold(G::sum));
}

TEST(BasicBatch, Fold2){
    ASSERT_EQ(G::result(f_fold2, Wool::Plaintext), 4);
}

TEST(SHEEPBasic, HElib){

}

