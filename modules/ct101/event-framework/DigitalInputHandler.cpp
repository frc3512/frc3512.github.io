// Copyright (c) FRC Team 3512, Spartatroniks 2016-2017. All Rights Reserved.

#include "DigitalInputHandler.hpp"

std::vector<std::unique_ptr<DigitalInput>> DigitalInputHandler::m_inputs(10);

DigitalInput* DigitalInputHandler::Get(uint32_t channel) {
    // Lazily construct digital input
    if (m_inputs[channel] == nullptr) {
        m_inputs[channel] = std::make_unique<DigitalInput>(channel);
    }
    return m_inputs[channel].get();
}
