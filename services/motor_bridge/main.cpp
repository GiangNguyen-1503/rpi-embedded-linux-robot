// motor_bridge: receives pvm_motor_command_t and writes the command byte to the
// Arduino over UART (/dev/ttyACM0 or /dev/ttyUSB0).
//
// Day 4: listen on PVM_MOTOR_SOCKET, write to the serial port (termios, 9600 8N1),
//        compute latency = pvm_now_us() - t_capture_us and log it.
//        Fallback without hardware: --dry-run prints the command instead.
#include <csignal>
#include <cstdio>
#include "pvm/protocol.h"

static volatile std::sig_atomic_t g_running = 1;
static void on_signal(int) { g_running = 0; }

int main() {
    std::signal(SIGINT, on_signal);
    std::signal(SIGTERM, on_signal);

    std::printf("[motor_bridge] started, listening on %s\n", PVM_MOTOR_SOCKET);
    // TODO(day 4): socket server + termios serial output + latency log
    return 0;
}
