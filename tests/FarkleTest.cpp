// tests/farkle_layout_test.cpp
#include <gtest/gtest.h>
#include "Farkle.h"
#include "constants.h"

TEST(FarkleLayout, StartsAtCorrectX) {
    
    int x = Farkle::DieLayout::getDieXPosition(1);
    EXPECT_EQ(x, 100);
}

TEST(FarkleLayout, SecondIsOffsetRight) {
    int x = Farkle::DieLayout::getDieXPosition(2);
    EXPECT_EQ(x, 100 + DiceConstants::DIE_SIZE + 20);
}

TEST(FarkleLayout, ThirdOffsetIsRight) {
    int x = Farkle::DieLayout::getDieXPosition(3);
    EXPECT_EQ(x, 100 + DiceConstants::DIE_SIZE*2 + 40);
}

TEST(FarkleLayout, FourthOffsetIsRight) {
    int x = Farkle::DieLayout::getDieXPosition(4);
    EXPECT_EQ(x, 100 + DiceConstants::DIE_SIZE*3 + 60);
}

