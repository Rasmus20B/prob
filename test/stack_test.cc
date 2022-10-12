#include "../src/stack.h"

#include <cstddef>
#include <gtest/gtest.h>

TEST (prob, stackPushPopNorm) {

  prob::Stack<int> s;
  s.push(10);
  EXPECT_EQ(10, s.pop());

}

TEST(prob, stackPopEmpty) {
  prob::Stack<int> s;
  EXPECT_EQ(false, s.pop().has_value());
}

TEST(prob, stackPushFull) {
  prob::Stack<int> s;
  while(!s.full()) {
    s.push(2);
  }
  s.push(10);
  EXPECT_EQ(2, s.pop());
}
