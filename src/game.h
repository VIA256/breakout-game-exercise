#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>

#ifdef WIN32
#include <windows.h>
#pragma comment(lib, "glut32.lib")
#endif

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.141592653589793

extern int idling;

void init();

#endif
