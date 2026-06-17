#include <gtest/gtest.h>

#include "storage/storage.hpp"

TEST(StorageTest, Dummy) {
    // Stub, should be developed later
    PatientStorage storage;
    EXPECT_EQ(storage.getAllPatients().size(), 0);
}
