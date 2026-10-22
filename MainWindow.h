#pragma once
#include "CoreTypes.h"
#include "DynamicalSystem.h"
#include "PhasePortrait.h"

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/fl_draw.H>
#include <vector>

// Custom widget to handle drawing the phase portrait
class PlotCanvas: public Fl_Widget
{
    public:
        PlotCanvas(int X, int Y, int W, int H, const char* L = 0);
        
        // The core FLTK drawing function
        void draw() override;
        
        // Feed data to the canvas
        void setPhasePortrait(const PhasePortrait* portrait, const std::vector<Point2D>& statPoints);

        // Viewport settings for coordinate mapping
        double m_xMin = 0.0, m_xMax = 5.0;
        double m_yMin = 0.0, m_yMax = 5.0;

    private:
        const PhasePortrait* m_portrait = nullptr;
        std::vector<Point2D> m_statPoints;

        // Helper functions to map mathematical coordinates to screen pixels
        // Named these parameters mathX, mathY to avoid clashing with FLTK's x() and y() methods
        int mapX(double mathX) const;
        int mapY(double mathY) const;
};

// The main application window
class MainWindow: public Fl_Double_Window
{
    public:
        MainWindow(int w, int h, const char* title);

    private:
        // Core logic instances
        DynamicalSystem m_system;
        PhasePortrait m_portrait;
        std::vector<Point2D> m_currentStationaryPoints;
        int m_gridResolution = 45;

        // GUI Elements
        Fl_Value_Input* inp_a1; Fl_Value_Input* inp_b1; Fl_Value_Input* inp_c1;
        Fl_Value_Input* inp_a2; Fl_Value_Input* inp_b2; Fl_Value_Input* inp_c2;
        Fl_Button* btn_plot;
        Fl_Multiline_Output* out_statPoints;
        PlotCanvas* canvas;

        // FLTK Callback for the button
        static void cb_plot(Fl_Widget* w, void* data);
        void updatePlot();
};
