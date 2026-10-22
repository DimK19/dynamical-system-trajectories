#include "MainWindow.h"

int main(int argc, char* argv[])
{
    // Create an 800x600 window
    MainWindow window(800, 600, "Dynamic System Phase Portrait");
    
    // Make the window visible
    window.show(argc, argv);
    
    // Hand over control to FLTK's event loop
    return Fl::run();
} 
