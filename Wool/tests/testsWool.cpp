//
// Created by mario on 17.02.20.
//
#include <fstream>
#include <string>
#include "gtest/gtest.h"
#include "circuit.hpp"


std::string expected_output = "../../Wool/tests/expected_output/";

TEST(BasicTest, GoogleTestImport) {
    ASSERT_EQ(1, 1);
}

TEST(BasicSHEEP, CircuitImport) {
    ASSERT_NO_THROW(Circuit());
}
