/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008-2016. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <mutex>

/**
 * Timer objects measure accumulated time in seconds.
 * The timer object functions like a stopwatch. It can be started, stopped, and
 * cleared. When the timer is running its value counts up in seconds. When
 * stopped, the timer holds the current value. The implementation simply records
 * the time when started and subtracts the current time whenever the value is
 * requested.
 */
class Timer {
public:
    Timer();
    virtual ~Timer() = default;

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    double Get() const;
    void Reset();
    void Start();
    void Stop();
    bool HasPeriodPassed(double period);

private:
    double m_startTime = 0.0;
    double m_accumulatedTime = 0.0;
    bool m_running = false;
    mutable std::mutex m_mutex;
};
