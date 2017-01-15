// Copyright (c) Your Name 2016. All Rights Reserved.

#include "RobotMock.hpp"
#include "Timer.hpp"

enum class State {
    // Define state names here
};

int main() {
    constexpr double kGround = 0.0;
    constexpr double kHeightHalf = 7.2;
    constexpr double kHeight1 = 16.0;
    constexpr double kHeight2 = 28.76;

    RobotMock robot;
    State state = ;  // Assign the initial state here
    Timer timer;

    while (1) {
        // State machine logic goes here. Recommend switch-case structure.
    }
}
