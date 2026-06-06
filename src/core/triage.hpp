#ifndef TRIAGE_HPP
#define TRIAGE_HPP

#include <cstdint>

// Функция вычисления приоритета пациента на основе маски симптомов.
// На этапе 1 - заглушка, всегда возвращает 0.
uint8_t computePriority(uint32_t mask);

#endif