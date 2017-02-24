// Copyright (c) FRC Team 3512, Spartatroniks 2016-2017. All Rights Reserved.

#pragma once

#include <memory>
#include <vector>

#include <DigitalInput.h>

/**
 * Provides an interface for sharing digital inputs between subsystems and the
 * event framework
 */
class DigitalInputHandler {
public:
    static DigitalInput* Get(uint32_t channel);

private:
    static std::vector<std::unique_ptr<DigitalInput>> m_inputs;
};
