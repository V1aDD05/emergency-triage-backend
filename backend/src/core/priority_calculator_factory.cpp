#include <stdexcept>

#include "core/triage.hpp"
#include "emergency_triage/core/i_priority_calculator.hpp"

namespace emergency_triage {
std::unique_ptr<IPriorityCalculator> createPriorityCalculator(const std::string& triageMethod) {
	if (triageMethod == "SORT") {
		return std::make_unique<SORTPriorityCalculator>();
	} else {
		throw std::runtime_error("Unknown priority method" + triageMethod);
	}
}
}
