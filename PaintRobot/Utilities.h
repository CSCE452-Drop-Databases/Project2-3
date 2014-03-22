struct Point {
	double x, y;
	Point(double x, double y) : x(x), y(y) {}
};
typedef struct Point Point;

struct Color {
	float r;
	float g;
	float b;
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
};
typedef struct Color Color;

struct PaintCircle {
	Point p;
	Color c;
};
typedef struct PaintCircle PaintCircle;

/*----------------------------------------------------------------------------------------
*   \brief  This function draws a text string to the screen using glut bitmap fonts.
*   \param  font    -   the font to use. it can be one of the following :s
*
*                   GLUT_BITMAP_9_BY_15
*                   GLUT_BITMAP_8_BY_13
*                   GLUT_BITMAP_TIMES_ROMAN_10
*                   GLUT_BITMAP_TIMES_ROMAN_24
*                   GLUT_BITMAP_HELVETICA_10
*                   GLUT_BITMAP_HELVETICA_12
*                   GLUT_BITMAP_HELVETICA_18
*
*   \param  text    -   the text string to output
*   \param  x       -   the x co-ordinate
*   \param  y       -   the y co-ordinate
*/
void font(void *font, char *text, int x, int y);

void paintRobotSleep(int ms);

void drawEllipse(float xc, float yc, float x, float y);

void ellipseMidpoint(float xc, float yc, float rx, float ry);