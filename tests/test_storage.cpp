#include <gtest/gtest.h>
#include "storage/storage.hpp"

TEST(StorageTest, Dummy)
{
    // Заглушка - проверяем, что хранилище можно создать.
    // Реальные тесты будут добавлены на следующих этапах.
    PatientStorage storage;
    EXPECT_EQ(storage.getAllPatients().size(), 0);
}