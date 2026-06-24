#pragma once

#include <cstdint>

#include "emergency_triage/core/i_priority_calculator.hpp"
#include "emergency_triage/storage/data_structures.hpp"

namespace emergency_triage {

class SORTPriorityCalculator : public IPriorityCalculator {
public:
	uint8_t computePriority(const EmergencyData& emergencyData, const TriageData& triageData) const override{
		// Stub, should be developed later
		[[maybe_unused]] emergencyData;
		[[maybe_unused]] triageData;
		return 1;
	};
};
}
