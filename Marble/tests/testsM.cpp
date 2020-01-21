//
// Created by mario on 08.01.20.
//
#include <fstream>
#include "gtest/gtest.h"
#include "Ast.h"
#include "AbstractExpr.h"
#include "AbstractStatement.h"
#include "M.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace Marble;

TEST(BasicTest, GoogleTestImport){
    ASSERT_EQ(1,1);
}

TEST(BasicAst, AstImport){
    ASSERT_NO_THROW(Ast());
}

TEST(BasicM, MImport){
    ASSERT_NO_THROW(Marble::M());
}

void empty_func(){
    M::output(M());
}

void empty_func2(){
    M x = M();
    M::output(x);
}

class MTest : public ::testing::Test {
    protected:
        std::function <void()> f;
};

TEST_F(MTest, EmptyBodyAST){
    f = empty_func;
    Ast ast = M::make_AST(f);
    // TODO: change to relative path
    std::ifstream file("/home/mario/Desktop/Granite/Marble/tests/expected_output/empty_func.json");
    json expected_j = json::parse(file);
    auto rn = ast.getRootNode();
    EXPECT_EQ(expected_j.dump(), rn->toString());
}

TEST_F(MTest,EmptyFuncEquality){
    f = empty_func;
    Ast ast = M::make_AST(f);
    Ast ast2 = M::make_AST(empty_func2);
    auto rn = ast.getRootNode();
    auto rn2 = ast2.getRootNode();
    EXPECT_EQ(rn->toString(), rn2->toString());
}