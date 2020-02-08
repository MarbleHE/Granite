//
// Created by mario on 08.01.20.
//
#include <fstream>
#include <string>
#include "gtest/gtest.h"
#include "Ast.h"
#include "BinaryExpr.h"
#include "M.hpp"
#include "nlohmann/json.hpp"
#include "Function.h"
#include "Return.h"


using json = nlohmann::json;
using namespace Marble;

std::string expected_output = "../../Marble/tests/expected_output/";

TEST(BasicTest, GoogleTestImport) {
    ASSERT_EQ(1, 1);
}

TEST(BasicAst, AstImport) {
    ASSERT_NO_THROW(Ast());
}

TEST(BasicM, MImport) {
    ASSERT_NO_THROW(Marble::M());
}


TEST(MTest, EmptyConstructorExprTest) {
    std::ifstream file(expected_output + "empty_constructor_expr.json");
    json expected_json = json::parse(file);
    EXPECT_EQ(M().expr->toString(), expected_json.dump());

    M a = M();
    EXPECT_EQ(a.expr->toString(), expected_json.dump());

    M b;
    EXPECT_EQ(b.expr->toString(), expected_json.dump());
}

TEST(MTest, LiteralIntExprTest) {
    std::ifstream file(expected_output + "literal_int_expr.json");
    json expected_json = json::parse(file);
    EXPECT_EQ(M(42).expr->toString(), expected_json.dump());

    M a = M(42);
    EXPECT_EQ(a.expr->toString(), expected_json.dump());

    M b = 42;
    EXPECT_EQ(b.expr->toString(), expected_json.dump());
}

TEST(MTest, LiteralBoolExprTest) {
    std::ifstream file(expected_output + "literal_bool_expr.json");
    json expected_json = json::parse(file);
    EXPECT_EQ(M(true).expr->toString(), expected_json.dump());

    M a = M(true);
    EXPECT_EQ(a.expr->toString(), expected_json.dump());


    M b = M(true);
    EXPECT_EQ(b.expr->toString(), expected_json.dump());
}

TEST(MTest, ReturnLiteralIntTest){
    M m = 42;
    Ast ast = Ast();
    Function *func = dynamic_cast<Function *>(ast.setRootNode(new Function("f")));
    std::ifstream file(expected_output + "return_literal_int.json");
    json expected_json = json::parse(file);

    func->addStatement(new Return((m.expr)));

    EXPECT_EQ(ast.getRootNode()->toString(), expected_json.dump());
}

void empty_func() {
    M::output(M());
}

void empty_func2() {
    M x = M();
    M::output(x);
}

void literalInt_func() {
    M::output(M(42));
}

void literalInt_func2() {
    M a = 42;
    M::output(a);
}


class FTest : public ::testing::Test {
protected:
    std::function<void()> f;
};

TEST_F(FTest, LiteralIntAST) {
    f = empty_func;
    std::ifstream file(expected_output + "empty_func.json");
    json expected_j = json::parse(file);

    Ast ast = M::make_AST(f);
    auto rn = ast.getRootNode();

    EXPECT_EQ(expected_j.dump(), rn->toString());
    //TODO: SIGSEGV at destruction? SIGBUS when ~M() is manually implemented?
}

TEST_F(FTest, EmptyFuncEquality) {
    f = empty_func;
    Ast ast = M::make_AST(f);
    Ast ast2 = M::make_AST(empty_func2);
    auto rn = ast.getRootNode();
    auto rn2 = ast2.getRootNode();
    EXPECT_EQ(rn->toString(), rn2->toString());
}