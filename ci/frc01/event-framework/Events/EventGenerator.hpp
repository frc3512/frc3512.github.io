// Copyright (c) FRC Team 3512, Spartatroniks 2016-2017. All Rights Reserved.

#pragma once

#include <string>

#include "EventAcceptor.hpp"

/**
 * Receives events
 */
class EventGenerator {
public:
    virtual ~EventGenerator() = default;

    virtual void Poll(EventAcceptor& acceptor) = 0;
};
