#pragma once

#include <robotics/common.h>

#include <algorithm>
#include <cmath>

namespace Robotics::ClassicalControl {
    /**
     * @brief A class for implemeting a Proportional-Integral-Derivative controller
     */
    class PID {
      public:
        /**
         * @brief Creates a new PID controller
         * @param Kp proportional gain
         * @param Ki integral gain
         * @param Kd derivative gain
         */
        PID(double Kp, double Ki, double Kd) : Kp(Kp), Ki(Ki), Kd(Kd) {}

        /**
         * @brief Computes the feedback control action
         * @param current current state
         * @param target target value
         * @return the computed control action
         */
        double ComputeControlAction(double current, double target);

        /**
         * @brief Sets the time step used to propagate the dynamics
         * @param step desired time step
         */
        void SetTimeStep(double step) { dt = step; }

        /**
         * @brief Sets a lower and upper bound for the control action
         * @param min minimum control action
         * @param max maximum control action
         */
        void SetControlActionLimits(double min, double max)
        {
            is_limited = true;
            u_min = min;
            u_max = max;
        }

      private:
        double Kp, Ki, Kd;
        double dt{0.1};
        bool is_limited{false};
        double integral{0};
        double err_old{0};
        double u_min;
        double u_max;
    };

    double PID::ComputeControlAction(double current, double target)
    {
        double err = target - current;

        double P_term = Kp * err;

        integral += err * dt;
        double I_term = Ki * integral;

        double derivative = (err - err_old) / dt;
        double D_term = Kd * derivative;

        double u = P_term + I_term + D_term;

        if (is_limited) u = std::clamp(u, u_min, u_max);

        err_old = err;

        return u;
    }
}  // namespace Robotics::ClassicalControl