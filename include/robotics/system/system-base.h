#pragma once

#include <robotics/common.h>

#include <Eigen/Dense>

namespace Robotics::Model {

    /**
     * @brief A class for implemeting a generic dynamical system. Mostly serves to describe an
     * interface.
     */
    template <int StateSize, int InputSize, int OutputSize>
    class SystemBase {
      public:
        using State = ColumnVector<StateSize>;
        using Input = ColumnVector<InputSize>;
        using Output = ColumnVector<OutputSize>;

        using StateMatrix = SquareMatrix<StateSize>;
        using InputMatrix = Matrix<StateSize, InputSize>;
        using OutputMatrix = Matrix<OutputSize, StateSize>;
        using FeedthroughMatrix = Matrix<OutputSize, StateSize>;

        /**
         * @brief Propagates the state for one time step
         * @param u system input
         */
        virtual void PropagateDynamics(const Input& u) = 0;

        /**
         * @brief Sets the time step used to propagate the dynamics
         * @param step desired time step
         */
        void SetTimeStep(double step) { dt = step; };

        /**
         * @brief Updates the internal state of the system, which will become the new initial
         * condition
         * @param state new state
         */
        void SetInitialState(State state) { x = state; }

        /**
         * @brief Gets the latest state computed
         * @return the latest state
         */
        State GetState() const { return x; }

        /**
         * @brief Gets the current output of the system
         * @return the current output
         */
        Output GetOutput()
        {
            y = C * x;
            return y;
        }

        /**
         * @brief Gets the system's output matrix
         * @return theoutput matrix
         */
        OutputMatrix GetOutputMatrix() const { return C; }

        /**
         * @brief Gets a noisy output reading
         * @param noise noise statistic for the output reading
         * @return the noisy output reading
         */
        Output GetNoisyMeasurement(const SquareMatrix<OutputSize>& noise)
        {
            return C * x + noise * random.GetColumnVector<OutputSize>();
        }

      protected:
        double dt{0.1};
        State x{State::Zero()};
        Output y{Output::Zero()};

        StateMatrix A;
        InputMatrix B;
        OutputMatrix C;
        FeedthroughMatrix D;

        Robotics::NormalDistributionRandomGenerator random;
    };

}  // namespace Robotics::Model