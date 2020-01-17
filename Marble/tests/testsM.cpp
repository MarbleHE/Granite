//
// Created by mario on 08.01.20.
//
#include <gtest/gtest.h>
#include "Ast.h"
#include "M.hpp"

TEST(BasicTest, GoogleTestImport){
    ASSERT_EQ(1,1);
}

TEST(BasicAst, AstImport){
    ASSERT_NO_THROW(Ast());
}

TEST(BasicM, MImport){
    ASSERT_NO_THROW(Marble::M());
}