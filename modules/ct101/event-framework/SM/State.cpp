// Copyright (c) FRC Team 3512, Spartatroniks 2016-2017. All Rights Reserved.

#include "State.hpp"

#include <utility>

State::State(std::string name) : m_name{std::move(name)} {}

std::string State::Name() const { return m_name; }

std::string State::StackTrace() const { return Name(); }

std::string State::HandleEvent(std::string& event) {
    auto ret = CheckTransition(event);

    // If state requested a transition, consume the event
    if (ret.length() != 0) {
        event.resize(0);
    }

    return ret;
}
