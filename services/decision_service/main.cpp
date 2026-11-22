// decision_service: receives pvm_obstacle_event_t, decides a motor command,
// forwards pvm_motor_command_t to motor_bridge.
//
// Day 2 (IPC): bind/listen/accept on PVM_VISION_SOCKET, print every event received.
// Day 4:       connect to PVM_MOTOR_SOCKET and forward commands.
// Day 5:       move the decision rule into a pure function and unit-test it.
#include <csignal>
#include <cstdio>
#include "pvm/protocol.h"

static volatile std::sig_atomic_t g_running = 1;
static void on_signal(int) { g_running = 0; }

int main() {
    std::signal(SIGINT, on_signal);
    std::signal(SIGTERM, on_signal);

    std::printf("[decision_service] started, listening on %s\n", PVM_VISION_SOCKET);
    // TODO(day 2): socket + bind + listen + accept + recv loop
    // Remember: unlink(PVM_VISION_SOCKET) before bind and on exit.
    return 0;
}
