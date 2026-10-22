#include "PhasePortrait.h"

void PhasePortrait::generateVectorField(const DynamicalSystem& system, 
                                        double xMin, double xMax, 
                                        double yMin, double yMax, 
                                        int resolutionX, int resolutionY)
{
    m_vectorField.clear();

    if (resolutionX <= 1 || resolutionY <= 1) return;

    double dx = (xMax - xMin) / (resolutionX - 1);
    double dy = (yMax - yMin) / (resolutionY - 1);

    for(int i = 0; i < resolutionX; ++i)
    {
        for(int j = 0; j < resolutionY; ++j)
        {
            Point2D pt;
            pt.x = xMin + i * dx;
            pt.y = yMin + j * dy;

            Vector2D vec = system.computeDerivative(pt);
            
            m_vectorField.push_back({pt, vec});
        }
    }
}

const std::vector<std::pair<Point2D, Vector2D>>& PhasePortrait::getVectorField() const
{
    return m_vectorField;
}
