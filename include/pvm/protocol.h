// Message definitions shared by every service.
// Keep structs fixed-size and free of pointers: they are copied as raw bytes
// between processes.
#pragma once

#include <stdint.h>
#include <time.h>

#define PVM_VISION_SOCKET "/tmp/pvm_vision.sock"  // camera_service -> decision_service
#define PVM_MOTOR_SOCKET  "/tmp/pvm_motor.sock"   // decision_service -> motor_bridge

enum pvm_zone { PVM_ZONE_LEFT = 0, PVM_ZONE_CENTER = 1, PVM_ZONE_RIGHT = 2 };

// Sent by camera_service for every processed frame.
typedef struct {
    uint64_t frame_id;
    uint64_t t_capture_us;     // when the frame was captured (for latency measurement)
    uint8_t  obstacle[3];      // indexed by pvm_zone: 1 = obstacle detected
} pvm_obstacle_event_t;

// Sent by decision_service to motor_bridge.
typedef struct {
    uint64_t frame_id;
    uint64_t t_capture_us;     // copied from the event, so motor_bridge can compute latency
    char     command;          // 'F', 'B', 'L', 'R' or 'S' (same protocol as the Arduino)
} pvm_motor_command_t;

// Monotonic time in microseconds (not affected by wall-clock changes).
static inline uint64_t pvm_now_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
}
