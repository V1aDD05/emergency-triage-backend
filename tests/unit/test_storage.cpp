#include <gtest/gtest.h>

#include "emergency_triage/storage/storage.hpp"

TEST(StorageTest, Dummy) {
    // Stub, should be developed later
	emergency_triage::PatientStorage storage;
	EXPECT_EQ(storage.getAllPatients().size(), 0);
}
