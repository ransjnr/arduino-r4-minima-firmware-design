#pragma once
#include <stdint.h>

namespace config {
constexpr const char* firmware = "device-controller-1.0.0";
constexpr uint32_t serialBaud = 115200;
constexpr uint32_t defaultOnMs = 200;
constexpr uint32_t defaultOffMs = 800;
constexpr uint32_t minDurationMs = 50;
constexpr uint32_t maxDurationMs = 5000;
constexpr unsigned int lineCapacity = 64;
constexpr unsigned int inputBudget = 16;
}
