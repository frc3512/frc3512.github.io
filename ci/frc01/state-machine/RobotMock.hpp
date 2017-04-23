// Copyright (c) FRC Team 3512, Spartatroniks 2016. All Rights Reserved.

#pragma once

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

class RobotMock {
public:
    RobotMock();
    ~RobotMock();

    /*
     * Sets target height of elevator. The setpoint can range from 0 to 90
     * inches. The longer the distance to travel, the longer it takes to reach
     * setpoint. The time taken from 0 to 90 inches is about 2 seconds.
     */
    void SetSetpoint(double inches);

    // Returns true once mock feedback controller has reached target height
    bool AtSetpoint() const;

    // Opens/closes claw
    void SetClaw(bool closed);

    // Returns true if claw is closed
    bool GetClaw() const;

    /* Returns true if user button was pressed. In the mock class, this returns
     * true every 10 seconds and returns false on subsequent calls.
     */
    bool GetPressed();

private:
    std::thread m_inputThread;
    std::atomic<bool> m_threadRunning{false};

    bool m_buttonPressed = false;
    std::mutex m_buttonPressedMutex;

    double m_setpoint = 0;
    std::chrono::system_clock::time_point m_finishTime =
        std::chrono::system_clock::now();

    bool m_isClosed = true;
};
