#include <gtest/gtest.h>

#include "emergency_triage/storage/data_structures.hpp"
#include "core/triage.hpp"

TEST(TriageTest, Dummy) {
	emergency_triage::EmergencyParams emergencyParams = {.is_bleeding = false,
													   .is_extensive_wounds = false,
													   .is_penetrating_wounds = false,
													   .is_consciousness_depression = false,
													   .is_respiratory_depression = false,
													   .is_hemodynamic_depression = false,
													   .is_severe_combined_injury = false};
	emergency_triage::TriageData triageData = {
		.eye_response = 2,
		.verbal_response = 2,
		.motor_response = 2,
		.respiratory_rate = 25,
		.systolic_bp = 120,
	};

	// Stub, should be developed later
    EXPECT_EQ(computePriority(emergencyParams, triageData), 1);
}
