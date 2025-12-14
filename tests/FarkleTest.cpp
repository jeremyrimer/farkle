// tests/farkle_layout_test.cpp
#include <gtest/gtest.h>
#include "Farkle.h"
#include "constants.h"

TEST(FarkleLayout, StartsAtCorrectX) {
    int x = Farkle::DieLayout::getDieXPosition(1);
    EXPECT_EQ(x, 50);
}

TEST(FarkleLayout, SecondIsOffsetRight) {
    int x = Farkle::DieLayout::getDieXPosition(2);
    EXPECT_EQ(x, 50 + DiceConstants::DIE_SIZE + 20);
}

TEST(FarkleLayout, ThirdOffsetIsRight) {
    int x = Farkle::DieLayout::getDieXPosition(3);
    EXPECT_EQ(x, 50 + DiceConstants::DIE_SIZE*2 + 40);
}

TEST(FarkleLayout, FourthOffsetIsRight) {
    int x = Farkle::DieLayout::getDieXPosition(4);
    EXPECT_EQ(x, 50 + DiceConstants::DIE_SIZE*3 + 60);
}

TEST(FarkleLayout, FifthOffsetIsRight) {
    int x = Farkle::DieLayout::getDieXPosition(5);
    EXPECT_EQ(x, 50 + DiceConstants::DIE_SIZE*4 + 80);
}

TEST(FarkleLayout, SixthOffsetIsRight) {
    int x = Farkle::DieLayout::getDieXPosition(6);
    EXPECT_EQ(x, 50 + DiceConstants::DIE_SIZE*5 + 100);
}

