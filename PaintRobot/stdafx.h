// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "global_const.h"

#include <stdio.h>
#include <tchar.h>

#include <cmath>
#include <iostream>
#include <vector>
#include "Button.h"
#include "Mouse.h"
#include "Matrix.h"
#include "PaintArm.h"

#ifdef _WIN32
#include "targetver.h"
#include <gl/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif