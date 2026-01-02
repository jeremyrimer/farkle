// tests/farkle_layout_test.cpp
#include <gtest/gtest.h>
#include "Dice.h"
#include "constants.h"

TEST(FarkleLayout, StartsAtCorrectX) {
    int x = DieLayout::getDieXPosition(1);
    EXPECT_EQ(x, 50);
}

TEST(FarkleLayout, SecondIsOffsetRight) {
    int x = DieLayout::getDieXPosition(2);
    EXPECT_EQ(x, 50 + DiceConstants::DIE_SIZE + 20);
}

TEST(FarkleLayout, ThirdOffsetIsRight) {
    int x = DieLayout::getDieXPosition(3);
    EXPECT_EQ(x, 50 + DiceConstants::DIE_SIZE*2 + 40);
}

TEST(FarkleLayout, FourthOffsetIsRight) {
    int x = DieLayout::getDieXPosition(4);
    EXPECT_EQ(x, 50 + DiceConstants::DIE_SIZE*3 + 60);
}

TEST(FarkleLayout, FifthOffsetIsRight) {
    int x = DieLayout::getDieXPosition(5);
    EXPECT_EQ(x, 50 + DiceConstants::DIE_SIZE*4 + 80);
}

TEST(FarkleLayout, SixthOffsetIsRight) {
    int x = DieLayout::getDieXPosition(6);
    EXPECT_EQ(x, 50 + DiceConstants::DIE_SIZE*5 + 100);
}

