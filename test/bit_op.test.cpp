#include "gtest/gtest.h"
#include "code/bit_op.h"

enum class E : uint8_t
{
    none = 0,
    a   = 0b0001,
    b   = 0b0010,
    c   = 0b0100,
    mix = 0b0111,
    max = 0b1000,
};

use_bit_op(E);

TEST(bit_op, or)
{
    ASSERT_EQ(E::a | E::b | E::c, E::mix);
}

TEST(bit_op, and)
{
    ASSERT_EQ(E::mix & E::a, E::a);
}

TEST(bit_op, xor)
{
    ASSERT_EQ(E::mix ^ (E::a | E::b), E::c);
}
