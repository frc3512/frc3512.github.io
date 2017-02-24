// =============================================================================
// File Name: KalmanFilter.cpp
// Description: Implements a Kalman filter for single input filtering
// Author: FRC Team 3512, Spartatroniks
// =============================================================================

#include "KalmanFilter.hpp"

KalmanFilter::KalmanFilter(double Q, double R) :
    m_Q(Q),
    m_R(R) {
    reset();
}

void KalmanFilter::update(double input) {
    std::lock_guard<std::recursive_mutex> lock(m_dataMutex);

    // Get the current dt since the last call to update()
    m_dt = std::chrono::system_clock::now() - m_lastTime;

    // Initialize estimate to measured value
    if (m_firstRun) {
        m_xHat = input;

        m_firstRun = false;
    }

    // Make a prediction - project the error covariance ahead
    m_P += m_Q * m_dt.count();

    /* Calculate the Kalman gain
     *   (m_P + m_R) is S, the estimate error
     */
    double K = m_P / (m_P + m_R);

    /* Update estimate with measurement zk (z_measured)
     *   'input' is zk in this equation
     *   (input - m_xHat) is y, the innovation
     */
    m_xHat += K * (input - m_xHat);

    // Correct error covariance
    m_P -= K * m_P;

    // Test for NaN (all comparisons with NaN will be false)
    if ((!(m_xHat &gt; 0.0)) &amp;&amp; (!(m_xHat  lock(m_dataMutex);
    m_Q = Q;
}

void KalmanFilter::setR(double R) {
    std::lock_guard<std::recursive_mutex> lock(m_dataMutex);
    m_R = R;
}

double KalmanFilter::getEstimate() {
    std::lock_guard<std::recursive_mutex> lock(m_dataMutex);
    return m_xHat;
}

void KalmanFilter::reset() {
    std::lock_guard<std::recursive_mutex> lock(m_dataMutex);

    m_xHat = 0.0;
    m_P = 0.0;
    m_lastTime = std::chrono::system_clock::now();
    m_firstRun = true;
}
