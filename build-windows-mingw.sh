ls build/windows > /dev/null 2>&1 || mkdir -p build/windows
i686-w64-mingw32-gcc main.c -Wall -Wextra -Wpedantic -std=c99 -mwindows glut32.lib -lopengl32 -lm -o build/windows/breakout.exe
