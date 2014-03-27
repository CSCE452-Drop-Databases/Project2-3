// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include "global_const.h"

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "Utilities.h"
#include "Button.h"
#include "Mouse.h"








#ifdef _WIN32
#include <tchar.h>
#include "windows.h"
#include "targetver.h"
#include <gl/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h> #include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif