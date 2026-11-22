// camera_service: captures frames, detects obstacles, publishes pvm_obstacle_event_t.
//
// Day 2 (IPC):     connect to PVM_VISION_SOCKET and send a fake event every 100 ms.
// Day 3 (threads): one capture thread + one processing thread, joined by a
//                  mutex/condition-variable queue.
// Later:           replace the fake detector with OpenCV (or a video file as fallback).
#include <csignal>
#include <cstdio>
#include "pvm/protocol.h"

static volatile std::sig_atomic_t g_running = 1;
static void on_signal(int) { g_running = 0; }

int main() {
    std::signal(SIGINT, on_signal);
    std::signal(SIGTERM, on_signal);

    std::printf("[camera_service] started, will publish to %s\n", PVM_VISION_SOCKET);
    // TODO(day 2): socket(AF_UNIX, SOCK_SEQPACKET, 0) + connect + send loop
    return 0;
}
