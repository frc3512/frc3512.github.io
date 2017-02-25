// =============================================================================
// File Name: KalmanFilter.hpp
// Description: Implements a Kalman filter for single input filtering
// Author: FRC Team 3512, Spartatroniks
// =============================================================================

#ifndef KALMAN_FILTER_HPP
#define KALMAN_FILTER_HPP

#include <mutex>
#include <chrono>

class KalmanFilter {
public:
    KalmanFilter(double Q, double R);

    void update(double input);

    void setQ(double Q);
    void setR(double R);

    // Return the filtered value
    double getEstimate();

    void reset();

private:
    // Used for getting and setting variables between main and sampling thread
    std::recursive_mutex m_dataMutex;

    // Running estimate of state calculated by filter
    double m_xHat;

    // Covariance of error
    double m_P;

    // Process noise variance
    double m_Q;

    // Measurement noise variance
    double m_R;

    // Holds dt in update()
    std::chrono::duration<double> m_dt;

    // Used to find dt in update()
    std::chrono::time_point<chrono::system_clock> m_lastTime;

    // If true, update() sets current estimate to first measurement
    bool m_firstRun;
};

#endif // KALMAN_FILTER_HPP
