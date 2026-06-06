#include <gtest/gtest.h>
#include "core/triage.hpp"

TEST(TriageTest, Dummy)
{
    // Заглушка – проверяем, что функция computePriority существует и возвращает 0.
    EXPECT_EQ(computePriority(0), 0);
}