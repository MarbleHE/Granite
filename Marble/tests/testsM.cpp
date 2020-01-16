//
// Created by mario on 08.01.20.
//
#include <gtest/gtest.h>
#include "Ast.h"

TEST(BasicTest, Eq){
    ASSERT_EQ(1,1);
}

TEST(BasicAst, Eq){
    ASSERT_NO_THROW(Ast());
}