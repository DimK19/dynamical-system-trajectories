#pragma once
#include "CoreTypes.h"
#include "DynamicalSystem.h"
#include <vector>
#include <utility>

class PhasePortrait
{
    public:
        PhasePortrait() = default;

        // Generates a grid of points and calculates the vector at each point.
        // resolutionX and resolutionY act as your "dpi" or sample count.
        void generateVectorField(const DynamicalSystem& system, 
                                 double xMin, double xMax, 
                                 double yMin, double yMax, 
                                 int resolutionX, int resolutionY);

        const std::vector<std::pair<Point2D, Vector2D>>& getVectorField() const;

    private:
        std::vector<std::pair<Point2D, Vector2D>> m_vectorField;
};
