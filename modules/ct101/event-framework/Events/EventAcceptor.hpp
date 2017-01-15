// Copyright (c) FRC Team 3512, Spartatroniks 2016-2017. All Rights Reserved.

#pragma once

#include <string>

/**
 * Receives events
 */
class EventAcceptor {
public:
    virtual ~EventAcceptor() = default;

    virtual std::string HandleEvent(std::string& event) = 0;
};
