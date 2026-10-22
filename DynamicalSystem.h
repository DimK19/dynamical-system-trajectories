#pragma once
#include "CoreTypes.h"
#include <vector>

class DynamicalSystem
{
    public:
        DynamicalSystem() = default;

        void setParameters(const Parameters& params);
        Parameters getParameters() const;

        Vector2D computeDerivative(const Point2D& point) const;

        std::vector<Point2D> findStationaryPoints() const;

        EigenResult calculateEigenvalues(const Point2D& point) const;

    private:
        Parameters m_params;
};
