#pragma once

struct Parameters
{
    double a1 = 0.0, b1 = 0.0, c1 = 0.0;
    double a2 = 0.0, b2 = 0.0, c2 = 0.0;
};

struct Point2D
{
    double x = 0.0;
    double y = 0.0;
};

struct Vector2D
{
    double dx = 0.0;
    double dy = 0.0;
};

struct EigenResult
{
    double lambda1_real, lambda1_imag;
    double lambda2_real, lambda2_imag;
};
