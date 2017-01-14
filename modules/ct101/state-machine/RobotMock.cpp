// Copyright (c) FRC Team 3512, Spartatroniks 2016. All Rights Reserved.

#include "RobotMock.hpp"

#include <cmath>
#include <iostream>
#include <string>

using namespace std::chrono_literals;

RobotMock::RobotMock() {
    m_threadRunning = true;
    m_inputThread = std::thread([this] {
        while (m_threadRunning) {
            {
                std::lock_guard<std::mutex> lock(m_buttonPressedMutex);
                m_buttonPressed = true;
            }
            std::this_thread::sleep_for(10s);
        }
    });
}

RobotMock::~RobotMock() {
    m_threadRunning = false;
    m_inputThread.join();
}

void RobotMock::SetSetpoint(double inches) {
    using namespace std::chrono;
    auto elapsed = std::chrono::milliseconds(2000 / 90) *
        std::abs(static_cast<int>(m_setpoint - inches));
    m_finishTime = std::chrono::system_clock::now() + elapsed;
    m_setpoint = inches;

    std::cout << "Called SetSetpoint(" << inches << ")" << std::endl;
}

bool RobotMock::AtSetpoint() const {
    std::cout << "Called AtSetpoint()" << std::endl;

    return std::chrono::system_clock::now() >= m_finishTime;
}

void RobotMock::SetClaw(bool closed) {
    m_isClosed = closed;

    std::cout << "Called SetClaw(" << std::to_string(closed) << ")"
              << std::endl;
}

bool RobotMock::GetClaw() const {
    return m_isClosed;
}

bool RobotMock::GetPressed() {
    bool isPressed;

    {
        std::lock_guard<std::mutex> lock(m_buttonPressedMutex);
        isPressed = m_buttonPressed;
        m_buttonPressed = false;
    }

    if (isPressed) {
        std::cout << "Consumed GetPressed() event" << std::endl;
    }

    return isPressed;
}
