// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#include <signal.h>
#include <unistd.h>

#include <chrono>

#include "LiveGrapher/LiveGrapher.hpp"

using namespace std::chrono_literals;

int main() {
    LiveGrapher liveGrapher(3513);
    liveGrapher.SetSendInterval(10ms);

    // Ignore SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    while (1) {
        if (liveGrapher.HasIntervalPassed()) {
            liveGrapher.ResetInterval();
        }

        std::this_thread::sleep_for(1ms);
    }
}
