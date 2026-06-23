#include <gtest/gtest.h>

#include "emergency_triage/storage/data_structures.hpp"
#include "core/triage.hpp"

TEST(TriageTest, Dummy) {
	emergency_triage::EmergencyData emergencyData = {.is_bleeding = false,
													   .is_extensive_wounds = false,
													   .is_penetrating_wounds = false,
													   .is_consciousness_depression = false,
													   .is_respiratory_depression = false,
													   .is_hemodynamic_depression = false,
													   .is_severe_combined_injury = false};
	uint8_t eye_response = 2;
	uint8_t verbal_response = 2;
	uint8_t motor_response = 2;
	uint8_t respiratory_rate = 25;
	uint16_t systolic_bp = 120;
	emergency_triage::TriageData triageData(
		eye_response, verbal_response, motor_response, respiratory_rate, systolic_bp);

	// Stub, should be developed later
    EXPECT_EQ(computePriority(emergencyData, triageData), 1);
}
