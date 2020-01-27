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

void empty_func() {
    M::output(M());
}

void empty_func2() {
    M x = M();
    M::output(x);
}

void simple_func() {
    M::output(M(42));
}


class MTest : public ::testing::Test {
protected:
    std::function<void()> f;
};

TEST(MTest, LiteralIntAST) {
    std::function<void()> f = simple_func;
    // TODO: change to relative path
    std::ifstream file(expected_output + "empty_func.json");
    json expected_j = json::parse(file);

    Ast ast = M::make_AST(f);
    auto rn = ast.getRootNode();
    auto bdy = ((Function *) rn)->getBody();
    for (auto x: bdy) std::cout << "VALLUE" << ((LiteralInt *) ((Return *) x)->getValue())->getValue();
    //std::cout << rn->toString();

    //EXPECT_EQ(expected_j.dump(), rn->toString());
}

TEST_F(MTest, EmptyFuncEquality) {
    f = empty_func;
    Ast ast = M::make_AST(f);
    Ast ast2 = M::make_AST(empty_func2);
    auto rn = ast.getRootNode();
    auto rn2 = ast2.getRootNode();
    EXPECT_EQ(rn->toString(), rn2->toString());
}