#include "DynamicalSystem.h"
#include <stdexcept>
#include <cmath>

void DynamicalSystem::setParameters(const Parameters& params)
{
    m_params = params;
}

Parameters DynamicalSystem::getParameters() const
{
    return m_params;
}

Vector2D DynamicalSystem::computeDerivative(const Point2D& point) const
{
    Vector2D vec;
    // Calculate dx/dt and dy/dt using m_params, point.x, and point.y
    // x' = x(a1 - b1*x - c1*y)
    // y' = y(a2 - b2*y - c2*x)
    
    vec.dx = point.x * (m_params.a1 - m_params.b1 * point.x - m_params.c1 * point.y);
    vec.dy = point.y * (m_params.a2 - m_params.b2 * point.y - m_params.c2 * point.x);
    
    return vec;
}

std::vector<Point2D> DynamicalSystem::findStationaryPoints() const
{
    std::vector<Point2D> stationaryPoints;

    // 1. Trivial equilibrium (Origin)
    stationaryPoints.push_back({0.0, 0.0});

    // 2. y-axis equilibrium (x = 0)
    if (m_params.b2 != 0.0)
    {
        stationaryPoints.push_back({0.0, m_params.a2 / m_params.b2});
    }

    // 3. x-axis equilibrium (y = 0)
    if (m_params.b1 != 0.0)
    {
        stationaryPoints.push_back({m_params.a1 / m_params.b1, 0.0});
    }

    // 4. Coexistence equilibrium (Interior point)
    double determinant = (m_params.b1 * m_params.b2) - (m_params.c1 * m_params.c2);
    
    if(determinant != 0.0)
    {
        double x_int = (m_params.a1 * m_params.b2 - m_params.a2 * m_params.c1) / determinant;
        double y_int = (m_params.a2 * m_params.b1 - m_params.a1 * m_params.c2) / determinant;
        
        stationaryPoints.push_back({x_int, y_int});
    }

    return stationaryPoints;
}

EigenResult DynamicalSystem::calculateEigenvalues(const Point2D& point) const
{
    EigenResult result = {0.0, 0.0, 0.0, 0.0};
    
    // 1. Calculate the Jacobian elements at (x, y)
    // f(x,y) = a1*x - b1*x^2 - c1*x*y
    // fx = a1 - 2b1x - c1y
    // fy = -c1x
    // g(x,y) = a2*y - b2*y^2 - c2*x*y
    double j11 = m_params.a1 - 2.0 * m_params.b1 * point.x - m_params.c1 * point.y;
    double j12 = -m_params.c1 * point.x;
    double j21 = -m_params.c2 * point.y;
    double j22 = m_params.a2 - 2.0 * m_params.b2 * point.y - m_params.c2 * point.x;

    // 2. Setup the quadratic equation: λ^2 - Tr(J)*λ + det(J) = 0
    double trace = j11 + j22;
    double determinant = (j11 * j22) - (j12 * j21);
    
    // A = 1, B = -tr, C = det
    // discriminant = B^2 - 4AC
    double discriminant = (trace * trace) - (4.0 * determinant);

    // 3. Solve and separate into real and imaginary parts
    if(discriminant >= 0.0)
    {
        // Real roots (discriminant is positive or zero)
        result.lambda1_real = (trace + std::sqrt(discriminant)) / 2.0;
        result.lambda1_imag = 0.0;
        
        result.lambda2_real = (trace - std::sqrt(discriminant)) / 2.0;
        result.lambda2_imag = 0.0;
    }
    else
    {
        // Complex roots (discriminant is negative)
        // Real part is -B/2A, Imaginary part is sqrt(-discriminant)/2A
        result.lambda1_real = trace / 2.0;
        result.lambda1_imag = std::sqrt(-discriminant) / 2.0;
        
        result.lambda2_real = trace / 2.0;
        result.lambda2_imag = -std::sqrt(-discriminant) / 2.0; // Conjugate
    }

    return result;
}
