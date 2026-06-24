#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "emergency_triage/storage/data_structures.hpp"

namespace emergency_triage {

class IPriorityCalculator {
public:
	virtual ~IPriorityCalculator() = default;
	virtual uint8_t computePriority(const EmergencyData& emergencyData, const TriageData& triageData) const = 0;
};

std::unique_ptr<IPriorityCalculator> createPriorityCalculator(const std::string& triageMethod);
}
