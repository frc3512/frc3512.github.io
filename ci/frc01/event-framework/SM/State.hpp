// Copyright (c) FRC Team 3512, Spartatroniks 2016-2017. All Rights Reserved.

#pragma once

#include <functional>
#include <string>

#include "../Events/EventAcceptor.hpp"

/**
 * Defines State in StateMachine class
 */
class State : public EventAcceptor {
public:
    explicit State(std::string name);

    // Returns the name of the state.
    std::string Name() const;

    // Returns a stack trace for the state machine, showing the current state.
    virtual std::string StackTrace() const;

    // Entry() is run when the state is first transitioned to.
    std::function<void()> Entry = [] {};

    /* CheckTransition() checks whether the event whose name was passed in
     * should cause a transition. If so, the name of the next desired state is
     * returned. Otherwise, "" is returned and the current state will be
     * maintained. The event is considered consumed if a non-empty string
     * is returned, so it won't be passed up the state machine hierarchy.
     */
    std::function<std::string(const std::string&)> CheckTransition =
        [](const std::string& event) { return ""; };

    // Run() is run while the state machine is in that state.
    std::function<void()> Run = [] {};

    // Exit() is run when the state is being transitioned away from.
    std::function<void()> Exit = [] {};

private:
    // Contains the name of the state
    std::string m_name;

    /* Returns the value of CheckTransition() if in the State class.
     * StateMachine overrides this to do extra handling and recursion (see
     * StateMachine.hpp for details).
     */
    std::string HandleEvent(std::string& event) override;

    friend class StateMachine;
};
