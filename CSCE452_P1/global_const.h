/*
File: global_const.h

Authors: Colton Williams, Charles Kernstock, Chris Murray, Paul Gaughan, Wesley Tang
Department of Computer Science
Texas A&M University

*/

#pragma once

#ifdef _WIN32
#include "stdafx.h"
#include <gl/glut.h>
#include <gl/gl.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#endif

int const DEFAULT_MATRIX_SIZE = 4;

double const DEG_TO_RADS = 3.14159265 / 180.0;

int const MATRIX_VALUE_ONE = 1;
int const MATRIX_VALUE_ZERO = 0;

int const LINK_LENGTH_1 = 3;
int const LINK_LENGTH_2 = 2;
int const LINK_LENGTH_3 = 1;
