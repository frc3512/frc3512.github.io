// Copyright (c) FRC Team 3512, Spartatroniks 2016-2017. All Rights Reserved.

#include "StateMachine.hpp"

#include <iostream>
#include <string>
#include <utility>

StateMachine::StateMachine(std::string name) : State(std::move(name)) {
    Run = [this] { m_currentState->Run(); };
}

void StateMachine::AddState(std::unique_ptr<State> state) {
    // First state added is made the initial state
    if (m_states.empty()) {
        m_currentState = state.get();
    }

    m_states.push_back(std::move(state));
}

std::string StateMachine::StackTrace() const {
    // Prepend state machine's name to provide a stack trace
    return Name() + " > " + m_currentState->StackTrace();
}

std::string StateMachine::HandleEvent(std::string& event) {
    if (m_debugEnabled) {
        using namespace std::chrono;
        auto now = steady_clock::now();
        auto timeStamp = duration_cast<microseconds>(now - m_startTime).count();

        std::cout << '[' << timeStamp << ", " << Name() << "] event: " << event;
    }

    auto newState = m_currentState->HandleEvent(event);

    // If current state didn't handle event
    if (newState.length() == 0) {
        if (m_debugEnabled) {
            std::cout << " not consumed" << std::endl;
        }

        // Check whether state machine itself will handle event
        auto ret = CheckTransition(event);

        // If state machine requested a transition, consume the event
        if (ret.length() != 0) {
            event.resize(0);
        }

        return ret;
    } else {
        if (m_debugEnabled) {
            std::cout << " consumed -> transition from "
                      << m_currentState->Name() << " to " << newState
                      << std::endl;
        }
        if (!SetState(newState)) {
            // Failed to find state matching the returned name
            std::cout << "\"" << newState << "\" is not a known state\n";
        }

        return newState;
    }
}

bool StateMachine::SetState(const std::string& newState) {
    for (auto& state : m_states) {
        if (state->Name() == newState) {
            if (m_currentState != nullptr) {
                m_currentState->Exit();
            }
            m_currentState = state.get();
            m_currentState->Entry();

            return true;
        }
    }

    return false;
}

void StateMachine::EnableDebug(bool enable) { m_debugEnabled = enable; }
