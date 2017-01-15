// Copyright (c) FRC Team 3512, Spartatroniks 2016-2017. All Rights Reserved.

#include "DigitalInputEventGenerator.hpp"

#include <utility>

void DigitalInputEventGenerator::RegisterInputEvent(std::string eventName,
                                                    uint32_t channel,
                                                    bool onRisingEdge,
                                                    bool onFallingEdge,
                                                    EventAcceptor& acceptor) {
    m_events.push_back({std::move(eventName), DigitalInputHandler::Get(channel),
                        onRisingEdge, onFallingEdge});
    m_oldStates.emplace_back(false);
    m_newStates.emplace_back(false);
}

void DigitalInputEventGenerator::Poll(EventAcceptor& acceptor) {
    for (unsigned int i = 0; i < m_events.size(); i++) {
        // If checking for rising edge
        if (m_events[i].onRisingEdge) {
            // If input transitioned from low to high
            if (m_oldStates[i] == false && m_newStates[i] == true) {
                // Force a deep copy to keep the original event name intact
                std::string temp = m_events[i].name;
                acceptor.HandleEvent(temp);
            }
        }

        // If checking for falling edge
        if (m_events[i].onFallingEdge) {
            // If input transitioned from high to low
            if (m_oldStates[i] == true && m_newStates[i] == false) {
                // Force a deep copy to keep the original event name intact
                std::string temp = m_events[i].name;
                acceptor.HandleEvent(temp);
            }
        }
    }
}

void DigitalInputEventGenerator::Update() {
    // Update old states for next check and get new input states
    for (uint32_t i = 0; i < m_oldStates.size(); i++) {
        m_oldStates[i] = m_newStates[i];
        m_newStates[i] = m_events[i].input->Get();
    }
}
