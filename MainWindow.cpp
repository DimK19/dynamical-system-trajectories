#include "MainWindow.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>

// --- PlotCanvas Implementation ---

PlotCanvas::PlotCanvas(int X, int Y, int W, int H, const char* L) 
    : Fl_Widget(X, Y, W, H, L) {}

void PlotCanvas::setPhasePortrait(const PhasePortrait* portrait, const std::vector<Point2D>& statPoints)
{
    m_portrait = portrait;
    m_statPoints = statPoints;
}

int PlotCanvas::mapX(double mathX) const
{
    // x() gets the canvas's left edge pixel. mathX is the mathematical coordinate.
    return x() + static_cast<int>((mathX - m_xMin) / (m_xMax - m_xMin) * w());
}

int PlotCanvas::mapY(double mathY) const
{
    // y() gets the canvas's top edge pixel. Y is inverted in screen coordinates.
    // Y is inverted in screen coordinates (0 is at the top)
    return y() + static_cast<int>((1.0 - (mathY - m_yMin) / (m_yMax - m_yMin)) * h());
}

void PlotCanvas::draw() {
    // Draw background
    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(), h());

    // Draw axes
    fl_color(FL_BLACK);
    fl_line_style(FL_DASH);
    fl_line(mapX(m_xMin), mapY(0.0), mapX(m_xMax), mapY(0.0)); // X axis
    fl_line(mapX(0.0), mapY(m_yMin), mapX(0.0), mapY(m_yMax)); // Y axis
    fl_line_style(FL_SOLID);

    if(!m_portrait) return;

    // Draw the vector field
    fl_color(FL_BLUE);
    const auto& vectorField = m_portrait->getVectorField();
    for(const auto& pair: vectorField)
    {
        const Point2D& pt = pair.first;
        const Vector2D& vec = pair.second;

        double length = std::sqrt(vec.dx * vec.dx + vec.dy * vec.dy);
        if(length > 0.0001) // Avoid dividing by zero
        {
            // Normalize the vector so all arrows are a standard readable size (e.g., 10 pixels long)
            double arrowLength = 10.0; 
            double normDx = (vec.dx / length);
            double normDy = (vec.dy / length);

            int startX = mapX(pt.x);
            int startY = mapY(pt.y);
            
            // Note: dy is inverted when adding to Y pixel coordinates
            int endX = startX + static_cast<int>(normDx * arrowLength);
            int endY = startY - static_cast<int>(normDy * arrowLength); 
            
            // Draw the line of the arrow
            fl_line(startX, startY, endX, endY);
            
            // Calculate arrowhead geometry
            double angle = std::atan2(-normDy, normDx); 
            double headLen = 4.0;
            double headAngle = 0.4; // roughly 23 degrees

            // Calculate the two back-swept points of the arrowhead
            int h1X = endX - static_cast<int>(headLen * std::cos(angle - headAngle));
            int h1Y = endY - static_cast<int>(headLen * std::sin(angle - headAngle));
            
            int h2X = endX - static_cast<int>(headLen * std::cos(angle + headAngle));
            int h2Y = endY - static_cast<int>(headLen * std::sin(angle + headAngle));

            // Draw the arrowhead wings
            fl_line(endX, endY, h1X, h1Y);
            fl_line(endX, endY, h2X, h2Y);
            
            /*
            // Draw a small dot at the base of the vector
            fl_pie(startX - 1, startY - 1, 3, 3, 0, 360); 
            */
        }
    }

    // Draw stationary points in RED
    fl_color(FL_RED);
    for(const auto& pt: m_statPoints)
    {
        int px = mapX(pt.x);
        int py = mapY(pt.y);
        fl_pie(px - 4, py - 4, 8, 8, 0, 360); // 8x8 pixel circle
    }
}

// --- MainWindow Implementation ---

MainWindow::MainWindow(int w, int h, const char* title) 
    : Fl_Double_Window(w, h, title) 
{
    int inputX = 40;
    int inputW = 60;
    int inputH = 25;
    int startY = 20;

    // First Equation (dx/dt)
    Fl_Box* lbl_eq1 = new Fl_Box(10, startY, 180, inputH, "x' = x(a1 - b1*x - c1*y)");
    inp_a1 = new Fl_Value_Input(inputX, startY + 30, inputW, inputH, "a1:");
    inp_b1 = new Fl_Value_Input(inputX, startY + 60, inputW, inputH, "b1:");
    inp_c1 = new Fl_Value_Input(inputX, startY + 90, inputW, inputH, "c1:");

    // Second Equation (dy/dt)
    Fl_Box* lbl_eq2 = new Fl_Box(10, startY + 130, 180, inputH, "y' = y(a2 - b2*y - c2*x)");
    inp_a2 = new Fl_Value_Input(inputX, startY + 160, inputW, inputH, "a2:");
    inp_b2 = new Fl_Value_Input(inputX, startY + 190, inputW, inputH, "b2:");
    inp_c2 = new Fl_Value_Input(inputX, startY + 220, inputW, inputH, "c2:");

    // Default test values
    inp_a1->value(1.0); inp_b1->value(0.5); inp_c1->value(0.5);
    inp_a2->value(1.0); inp_b2->value(0.5); inp_c2->value(0.5);

    // Plot Button
    btn_plot = new Fl_Button(10, startY + 270, 180, 40, "Update / Plot");
    btn_plot->callback(cb_plot, this);
    
    // Stationary Points Output Box
    out_statPoints = new Fl_Multiline_Output(10, startY + 320, 180, 150);
    out_statPoints->textsize(12);
    out_statPoints->color(FL_BACKGROUND2_COLOR); // White background
    out_statPoints->value("Stationary Points:\n(Click Update)");

    // Drawing Canvas
    canvas = new PlotCanvas(200, 10, w - 210, h - 20, "Phase Portrait");
    canvas->box(FL_DOWN_BOX);

    end(); // Close window layout
}

void MainWindow::cb_plot(Fl_Widget* w, void* data)
{
    MainWindow* window = static_cast<MainWindow*>(data);
    window->updatePlot();
}

void MainWindow::updatePlot()
{
    // 1. Gather parameters
    Parameters params;
    params.a1 = inp_a1->value(); params.b1 = inp_b1->value(); params.c1 = inp_c1->value();
    params.a2 = inp_a2->value(); params.b2 = inp_b2->value(); params.c2 = inp_c2->value();
    m_system.setParameters(params);

    // 2. Compute
    m_currentStationaryPoints = m_system.findStationaryPoints();
    
    // Format and display the stationary points
    std::ostringstream oss;
    oss << "Stationary Points:\n";
    for(const auto& pt: m_currentStationaryPoints)
    {
        // Print with 3 decimal places
        oss << "(" << std::fixed << std::setprecision(3) << pt.x << ", " << pt.y << ")\n";
    }
    out_statPoints->value(oss.str().c_str());

    // Console output for eigenvalues
    std::cout << "--- New Plot ---" << std::endl;
    for(const auto& pt: m_currentStationaryPoints)
    {
        EigenResult eigen = m_system.calculateEigenvalues(pt);
        std::cout << "Stat Point (" << pt.x << ", " << pt.y << ") -> "
                  << "L1: " << eigen.lambda1_real << " + " << eigen.lambda1_imag << "i, "
                  << "L2: " << eigen.lambda2_real << " + " << eigen.lambda2_imag << "i\n";
    }

    // 3. Generate Grid and Pass to Canvas
    m_portrait.generateVectorField(m_system, canvas->m_xMin, canvas->m_xMax, 
                                   canvas->m_yMin, canvas->m_yMax, 
                                   m_gridResolution, m_gridResolution);
    
    canvas->setPhasePortrait(&m_portrait, m_currentStationaryPoints);
    
    // 4. Force FLTK to redraw the canvas
    canvas->redraw();
}
