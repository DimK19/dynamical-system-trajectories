# dynamical-system-trajectories
Differential equation system trajectory plotter.

#### Setup Instructions (Windows)
1. Download the `mingw-w64-i686-fltk-*.pkg.tar.zst` file from https://repo.msys2.org/mingw/mingw32/ (for 32-bit compiler version) and extract it.
2. Create an `include` and a `lib` folder inside the root C++ project folder.
3. Copy Headers: Copy the FL folder from `mingw32\include` into the project’s include folder.
4. Copy Build Libraries: Copy all the contents from `mingw32\lib` into the project’s lib folder.
5. Copy Runtime Library: Copy `libfltk-1.4.dll` from `mingw32\bin` directly into the root of the project folder (next to the .cpp files).

_What is the difference between .dll and .dll.a?_

`libfltk.dll.a` (Import Library): Used by the compiler at build time. It does not contain the actual GUI code. Instead, it acts as a table of contents, telling the .exe file exactly how to talk to the DLL later.

`libfltk-1.4.dll` (Dynamic Link Library): Used by Windows at run time. This contains the actual compiled FLTK code. When running the .exe, Windows open this file to figure out how to draw the screen. If it is missing, the program crashes immediately. 


#### Compilation Command
```
g++ main.cpp MainWindow.cpp DynamicalSystem.cpp PhasePortrait.cpp -I./include -L./lib -lfltk -lgdi32 -lole32 -luuid -lcomctl32 -lcomdlg32 -o phase_plotter.exe
```

- `g++ [files.cpp]`: Instructs the compiler to compile the four source files.

- `-I./include`: The Include flag. Tells the compiler to look inside the local ./include folder when it encounters `#include <FL/Fl.H>`.

- `-L./lib`: The Library path flag. Tells the compiler to look inside the local ./lib folder when searching for pre-compiled libraries.

- `-lfltk`: The link flag. Tells the compiler to link the code against the FLTK library (it automatically looks for libfltk.dll.a or libfltk.a).

- `-lgdi32 -lole32 -luuid -lcomctl32 -lcomdlg32`: Standard Windows system libraries. FLTK translates its cross-platform drawing commands into native Windows drawing commands (GDI), and requires these to spawn the window.

- `-o phase_plotter.exe`: The output flag. Names the final executable file.


#### Screenshots

<div>
<img src="./docs/Screenshot 2026-10-25 040223.png" height="300px" />
<img src="./docs/Screenshot 2026-10-25 040346.png" height="300px" />
</div>
