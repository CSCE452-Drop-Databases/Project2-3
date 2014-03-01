/* implementation of button code adapted from http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/opengl_programming.html#6 */

#include "stdafx.h"

struct Point {
	int x, y;
	Point(int x, int y) : x(x), y(y) {}
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

PaintArm paintArm;
Color paintBrushColor = Color( 1.0f, 1.0f, 1.0f );
int paintButtonMode = 0;

std::vector<Button> controlPanelButtons;
std::vector<PaintCircle> paint;

Mouse mouse = { 0, 0, 0, 0, 0 };

int axis1Num = 0;
int axis2Num = 0;
int axis3Num = 0;

int instantPaint = 0;

void paintRobotSleep(int ms) {
    // Sleep(75); // Comment if on Linux
    //sleep(((float)ms)/100.0); //Comment if on Windows
}

/*----------------------------------------------------------------------------------------
*	\brief	This function draws a text string to the screen using glut bitmap fonts.
*	\param	font	-	the font to use. it can be one of the following :s
*
*					GLUT_BITMAP_9_BY_15
*					GLUT_BITMAP_8_BY_13
*					GLUT_BITMAP_TIMES_ROMAN_10
*					GLUT_BITMAP_TIMES_ROMAN_24
*					GLUT_BITMAP_HELVETICA_10
*					GLUT_BITMAP_HELVETICA_12
*					GLUT_BITMAP_HELVETICA_18
*
*	\param	text	-	the text string to output
*	\param	x		-	the x co-ordinate
*	\param	y		-	the y co-ordinate
*/
void font(void *font, char *text, int x, int y)
{
	glRasterPos2i(x, y);

	while (*text != '\0')
	{
		glutBitmapCharacter(font, *text);
		++text;
	}
}

/*----------------------------------------------------------------------------------------
*	\brief	This function is used to see if a mouse click or event is within a button
*			client area.
*	\param	b	-	a pointer to the button to test
*	\param	x	-	the x coord to test
*	\param	y	-	the y-coord to test
*/
int ButtonClickTest(Button* b, int x, int y)
{
	int xOffset = WINDOW_WIDTH * (1 - GUI_MARGIN_PERCENT) - WINDOW_WIDTH * GUI_CONTROL_PANEL_WIDTH_PERCENT;
	int yOffset = WINDOW_HEIGHT * GUI_MARGIN_PERCENT;
	if (b)
	{
		/*
		*	If clicked within button area, then return true
		*/
		if (x > b->x + xOffset        &&
			x < b->x + b->w + xOffset &&
			y > b->y + yOffset        &&
			y < b->y + b->h + yOffset) {
			return 1;
		}
	}

	/*
	*	otherwise false.
	*/
	return 0;
}

/*----------------------------------------------------------------------------------------
*	\brief	This function checks if the mouse button was released in the button
*	\param	b	-	a pointer to the button to check.
*	\param	x	-	the x location of the mouse cursor.
*	\param	y	-	the y location of the mouse cursor.
*/
void ButtonRelease(Button *b, int x, int y)
{
	if (b)
	{
		/*
		*	If the mouse button was pressed within the button area
		*	as well as being released on the button.....
		*/
		if (ButtonClickTest(b, mouse.xpress, mouse.ypress) &&
			ButtonClickTest(b, x, y))
		{
			/*
			*	Then if a callback function has been set, call it.
			*/
			if (b->callbackFunction) {
				if (b->cbOnRelease) {
					b->callbackFunction();
				}
				else {
					if (b->type == Button::PRESS) {
                        glutIdleFunc(NULL);
                    }
				}
			}
			if (b->cbOnRelease && b->type == Button::TOGGLE) {
				b->mode = !b->mode;
			}

		}

        if (b->type == Button::TOGGLE) {
            glutIdleFunc(NULL);
        }

		if (b->type == Button::TOGGLE && b->mode) {
			b->state = 1;
		}
		else {
			b->state = 0;
		}
	}
}

/*----------------------------------------------------------------------------------------
*	\brief	This function checks if the button is being pressed
*	\param	b	-	a pointer to the button to check.
*	\param	x	-	the x location of the mouse cursor.
*	\param	y	-	the y location of the mouse cursor.
*/
void ButtonPress(Button *b, int x, int y)
{
	if (b)
	{
		/*
		*	if the mouse click was within the buttons client area,
		*	set the state to true.
		*/
		if (ButtonClickTest(b, x, y))
		{
			b->state = 1;
			if (!b->cbOnRelease) {
				b->mode = !b->mode;
				if ((b->type == Button::TOGGLE && b->mode && b->callbackFunction) || (b->type == Button::PRESS && b->callbackFunction)) {
					glutIdleFunc(b->callbackFunction);
				}
			}
		}
	}
}

/*----------------------------------------------------------------------------------------
*	\brief	This function checks if the mouse is currently over the button
*	\param	b	-	a pointer to the button to check.
*	\param	x	-	the x location of the mouse cursor.
*	\param	y	-	the y location of the mouse cursor.
*/
void ButtonPassive(Button *b, int x, int y)
{
	if (b)
	{
		/*
		*	if the mouse moved over the control
		*/
		if (ButtonClickTest(b, x, y))
		{
			/*
			*	If the cursor has just arrived over the control, set the highlighted flag
			*	and force a redraw. The screen will not be redrawn again until the mouse
			*	is no longer over this control
			*/
			if (b->highlighted == 0) {
				b->highlighted = 1;
				glutPostRedisplay();
			}
		}
		else

			/*
			*	If the cursor is no longer over the control, then if the control
			*	is highlighted (ie, the mouse has JUST moved off the control) then
			*	we set the highlighting back to false, and force a redraw.
			*/
		if (b->highlighted == 1)
		{
			b->highlighted = 0;
			glutPostRedisplay();
		}
	}
}

void ButtonsPressed(int x, int y) {
	for (int i = 0; i < controlPanelButtons.size(); ++i) {
		ButtonPress(&controlPanelButtons[i], x, y);
	}
}

void ButtonsReleased(int x, int y) {
	for (int i = 0; i < controlPanelButtons.size(); ++i) {
		ButtonRelease(&controlPanelButtons[i], x, y);
	}
}

void ButtonsPassive(int x, int y) {
	for (int i = 0; i < controlPanelButtons.size(); ++i) {
		ButtonPassive(&controlPanelButtons[i], x, y);
	}
}

/*----------------------------------------------------------------------------------------
*	\brief	This function draws the specified button.
*	\param	b	-	a pointer to the button to draw.
*/
void drawButton(Button *b)
{
	int fontx;
	int fonty;

	if (b)
	{
		/*
		*	We will indicate that the mouse cursor is over the button by changing its
		*	colour.
		*/
		if (b->highlighted)
			glColor3f(0.7f, 0.7f, 0.8f);
		else
			glColor3f(0.6f, 0.6f, 0.6f);

		/*
		*	draw background for the button.
		*/
		glBegin(GL_QUADS);
		glVertex2i(b->x, b->y);
		glVertex2i(b->x, b->y + b->h);
		glVertex2i(b->x + b->w, b->y + b->h);
		glVertex2i(b->x + b->w, b->y);
		glEnd();

		/*
		*	Draw an outline around the button with width 3
		*/
		glLineWidth(3);

		/*
		*	The colours for the outline are reversed when the button.
		*/
		if (b->state)
			glColor3f(0.4f, 0.4f, 0.4f);
		else
			glColor3f(0.8f, 0.8f, 0.8f);

		glBegin(GL_LINE_STRIP);
		glVertex2i(b->x + b->w, b->y);
		glVertex2i(b->x, b->y);
		glVertex2i(b->x, b->y + b->h);
		glEnd();

		if (b->state)
			glColor3f(0.8f, 0.8f, 0.8f);
		else
			glColor3f(0.4f, 0.4f, 0.4f);

		glBegin(GL_LINE_STRIP);
		glVertex2i(b->x, b->y + b->h);
		glVertex2i(b->x + b->w, b->y + b->h);
		glVertex2i(b->x + b->w, b->y);
		glEnd();

		glLineWidth(1);


		/*
		*	Calculate the x and y coords for the text string in order to center it.
		*/
		fontx = b->x + (b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)b->label)) / 2;
		fonty = b->y + (b->h + 10) / 2;

		/*
		*	if the button is pressed, make it look as though the string has been pushed
		*	down. It's just a visual thing to help with the overall look....
		*/
		if (b->state) {
			fontx += 2;
			fonty += 2;
		}

		/*
		*	If the cursor is currently over the button we offset the text string and draw a shadow
		*/
		if (b->highlighted)
		{
			glColor3f(0, 0, 0);
			font(GLUT_BITMAP_HELVETICA_10, b->label, fontx, fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1, 1, 1);
		font(GLUT_BITMAP_HELVETICA_10, b->label, fontx, fonty);
	}
}

void resetPaintArm() {
	paintArm = (*new PaintArm());
	paintArm.translate(0, axis1Num * SLIDE_AMOUNT, 0);
	paintArm.rotate(1, axis2Num * ROTATE_AMOUNT);
	paintArm.rotate(2, axis3Num * ROTATE_AMOUNT);
	glutPostRedisplay();
}

void gotoRobotArea() {
	// translates to the top left of robot area
	// in relation to the origin
	glTranslatef(WINDOW_WIDTH * GUI_MARGIN_PERCENT, WINDOW_HEIGHT * GUI_MARGIN_PERCENT, 0.0f);
}

void gotoControlPanel() {
	// translates to the top left of control panel
	// in relation to the origin
	glTranslatef(WINDOW_WIDTH * (1 - GUI_MARGIN_PERCENT) - WINDOW_WIDTH * GUI_CONTROL_PANEL_WIDTH_PERCENT, WINDOW_HEIGHT * GUI_MARGIN_PERCENT, 0.0f);
}

//Paints an ellipse that is vertically long
void paintArmEllipse(float center_x, float center_y, float radius_y, float width) {
	float const PI = 3.14159265;
	//radius_y is longest radius of ellipse
	//width of ellipse given as a proportion of height
	glBegin(GL_TRIANGLE_FAN);
	int sides = radius_y * 32;
	glVertex2f(center_x, center_y);
	for (int i = 0; i <= sides - 1; i++) {
		double theta = 2 * PI * i / sides;
		double point_x = (i / sides + width) * radius_y * cos(theta);
		double point_y = radius_y * sin(theta);
		glVertex2f(center_x + point_x, center_y + point_y);
	}
	glEnd();
	glFlush();
}

void drawEllipse(float xc, float yc, float x, float y)
{
	int p1[] = { (int)xc + x, (int)yc + y };
	int p2[] = { (int)xc - x, (int)yc + y };
	int p3[] = { (int)xc + x, (int)yc - y };
	int p4[] = { (int)xc - x, (int)yc - y };
	glVertex2iv(p1);
	glVertex2iv(p2);
	glVertex2iv(p3);
	glVertex2iv(p4);
}

void ellipseMidpoint(float xc, float yc, float rx, float ry)
{
	float rxSq = rx * rx;
	float rySq = ry * ry;
	float x = 0, y = ry, p;
	float px = 0, py = 2 * rxSq * y;
	drawEllipse(xc, yc, x, y);

	p = rySq - (rxSq * ry) + (0.25 * rxSq);
	while (px < py)
	{
		x++;
		px = px + 2 * rySq;
		if (p < 0)
			p = p + rySq + px;
		else
		{
			y--;
			py = py - 2 * rxSq;
			p = p + rySq + px - py;
		}
		drawEllipse(xc, yc, x, y);
	}

	p = rySq*(x + 0.5)*(x + 0.5) + rxSq*(y - 1)*(y - 1) - rxSq*rySq;
	while (y > 0)
	{
		y--;
		py = py - 2 * rxSq;
		if (p > 0)
			p = p + rxSq - py;
		else
		{
			x++;
			px = px + 2 * rySq;
			p = p + rxSq - py + px;
		}
		drawEllipse(xc, yc, x, y);
	}
}

Point axisMidpoint(int axis1, int axis2) {
	Matrix* paintArmAxis01 = paintArm.get_T_Matrix(0, axis1);
	Matrix* paintArmAxis02 = paintArm.get_T_Matrix(0, axis2);

	double paintArmAxis01X = paintArmAxis01->get_elem(0, 3);
	double paintArmAxis01Y = paintArmAxis01->get_elem(1, 3);

	double paintArmAxis02X = paintArmAxis02->get_elem(0, 3);
	double paintArmAxis02Y = paintArmAxis02->get_elem(1, 3);

	double midpointX = (paintArmAxis01X + paintArmAxis02X) / 2;
	double midpointY = (paintArmAxis01Y + paintArmAxis02Y) / 2;

	return Point(midpointX, midpointY);
}

void drawRobotAreaContents() {
	// draw the robot
	glPushMatrix();
	gotoRobotArea();

	glTranslatef(slidePosX + SLIDE_LENGTH / 2, slidePosY, 0.0f); // Translate to middle of bar (robot origin)

	for (PaintCircle p : paint) {
		glBegin(GL_LINES);
		glColor3f(p.c.r, p.c.g, p.c.b);
		ellipseMidpoint(p.p.x, -p.p.y, 5, 5);
		glEnd();
	}
	
	Matrix* paintArmAxis00 = paintArm.get_T_Matrix(0, 0);
	Matrix* paintArmAxis01 = paintArm.get_T_Matrix(0, 1);
	Matrix* paintArmAxis02 = paintArm.get_T_Matrix(0, 2);
	Matrix* paintArmAxis03 = paintArm.get_T_Matrix(0, 3);
	
	double paintArmAxis00X = paintArmAxis01->get_elem(0, 3);
	double paintArmAxis00Y = paintArmAxis01->get_elem(1, 3);

	double paintArmAxis01X = paintArmAxis01->get_elem(0, 3);
	double paintArmAxis01Y = paintArmAxis01->get_elem(1, 3);

	double paintArmAxis02X = paintArmAxis02->get_elem(0, 3);
	double paintArmAxis02Y = paintArmAxis02->get_elem(1, 3);

	double paintArmAxis03X = paintArmAxis03->get_elem(0, 3);
	double paintArmAxis03Y = paintArmAxis03->get_elem(1, 3);

	Point midpoint01 = axisMidpoint(0, 1);
	Point midpoint12 = axisMidpoint(1, 2);
	Point midpoint23 = axisMidpoint(2, 3);

	glPushMatrix();
	glTranslatef(midpoint23.x, -midpoint23.y, 0);
	glRotatef((-axis3Num * ROTATE_AMOUNT) + (-axis2Num * ROTATE_AMOUNT), 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.0f, 0.0f, 107.0f / 255.0f);
	ellipseMidpoint(0, 0, LINK_WIDTH / 2, LINK_LENGTH_3 / 2);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(midpoint12.x, -midpoint12.y, 0);
	glRotatef(-axis2Num * ROTATE_AMOUNT, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.0f, 107.0f / 255.0f, 0.0f);
	ellipseMidpoint(0, 0, LINK_WIDTH / 2, LINK_LENGTH_2 / 2);
	glEnd();
	glPopMatrix();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(107.0f / 255.0f, 0.0f, 0.0f);
		ellipseMidpoint(midpoint01.x, -midpoint01.y, LINK_WIDTH / 2, LINK_LENGTH_1 / 2);
	glEnd();
	
	glColor3f(0.0f, 1.0f, 0.0f);

	glPopMatrix();

	glPushMatrix();
	gotoRobotArea();
	glBegin(GL_LINES);
		glVertex2f(slidePosX, slidePosY);
		glVertex2f(slidePosX + SLIDE_LENGTH, slidePosY);
	glEnd();

	glPopMatrix();
}

void drawControlPanelContents() {
	// draw the controls
	int cpTitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)cpTitle)) / 2;
	int cpTitleY = 0.05 * controlPanelHeight;
	
	int cpAxis1TitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpAxis1Title)) / 2;
	int cpAxis1TitleY = 0.09 * controlPanelHeight;

	int cpAxis2TitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpAxis2Title)) / 2;
	int cpAxis2TitleY = 0.24 * controlPanelHeight;

	int cpAxis3TitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpAxis2Title)) / 2;
	int cpAxis3TitleY = 0.39 * controlPanelHeight;

	int cpPaintBrushTitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpPaintBrushTitle)) / 2;
	int cpPaintBrushTitleY = 0.54 * controlPanelHeight;

	int cpClearTitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpClearTitle)) / 2;
	int cpClearTitleY = 0.69 * controlPanelHeight;

	int cpColorTitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpColorTitle)) / 2;
	int cpColorTitleY = 0.84 * controlPanelHeight;

	glPushMatrix();
	gotoControlPanel();
	font(GLUT_BITMAP_HELVETICA_18, cpTitle, cpTitleX, cpTitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpAxis1Title, cpAxis1TitleX, cpAxis1TitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpAxis2Title, cpAxis2TitleX, cpAxis2TitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpAxis3Title, cpAxis3TitleX, cpAxis3TitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpPaintBrushTitle, cpPaintBrushTitleX, cpPaintBrushTitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpClearTitle, cpClearTitleX, cpClearTitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpColorTitle, cpColorTitleX, cpColorTitleY);

	for (Button b : controlPanelButtons) {
		drawButton(&b);
	}

	glPopMatrix();
}

/* Draws the UI overlay */
void drawOverlay() {
	glPushMatrix();
	// draw robot area
	gotoRobotArea();
	glBegin(GL_LINE_STRIP);
		glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, robotAreaHeight);
		glVertex2f(robotAreaWidth, robotAreaHeight);
		glVertex2f(robotAreaWidth, 0.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();

	// draw control panel
	glPushMatrix();

	gotoControlPanel();
	glBegin(GL_LINE_STRIP);
		glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, controlPanelHeight);
		glVertex2f(controlPanelWidth, controlPanelHeight);
		glVertex2f(controlPanelWidth, 0.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	drawRobotAreaContents();
	drawControlPanelContents();
	drawOverlay();

	glFlush();
}
void paintCircle() {
	Matrix* paintArmAxis03 = paintArm.get_T_Matrix(0, 3);
	double paintArmAxis03X = paintArmAxis03->get_elem(0, 3);
	double paintArmAxis03Y = paintArmAxis03->get_elem(1, 3);
	PaintCircle circle = { Point(paintArmAxis03X, paintArmAxis03Y), Color( paintBrushColor.r, paintBrushColor.g, paintBrushColor.b ) };
	paint.push_back(circle);
}

/*
 * Button Callbacks
 */
void axis1IncrementButtonCallback() {
	// move axis1 1px right
	printf("Axis 1 Increment Button Pressed!\n");
	if (abs(axis1Num * SLIDE_AMOUNT + SLIDE_AMOUNT) <= SLIDE_LENGTH / 2) {
		axis1Num++;
		resetPaintArm();
		if (controlPanelButtons[6].mode) paintCircle();
		paintRobotSleep(75);
	}
	else {
		std::cout << "ERROR: Cannot move base off of slide!" << std::endl;
	}
	
}

void axis1DecrementButtonCallback() {
	// move axis1 1px left
	printf("Axis 1 Decrement Button Pressed!\n");
	if (abs(axis1Num * SLIDE_AMOUNT - SLIDE_AMOUNT) <= SLIDE_LENGTH / 2) {
		axis1Num--;
		resetPaintArm();
		if (controlPanelButtons[6].mode) paintCircle();
		paintRobotSleep(75);
	}
	else {
		std::cout << "ERROR: Cannot move base off of slide!" << std::endl;
	}
}

void axis2IncrementButtonCallback() {
	// rotate axis2 +1 degree
	printf("Axis 2 Increment Button Pressed!\n");
	axis2Num--;
	resetPaintArm();
	if (controlPanelButtons[6].mode) paintCircle();
	paintRobotSleep(75);
}

void axis2DecrementButtonCallback() {
	// rotate axis2 -1 degree
	printf("Axis 2 Decrement Button Pressed!\n");
	axis2Num++;
	resetPaintArm();
	if (controlPanelButtons[6].mode) paintCircle();
	paintRobotSleep(75);
}

void axis3IncrementButtonCallback() {
	// rotate axis3 +1 degree
	printf("Axis 3 Increment Button Pressed!\n");
	axis3Num--;
	resetPaintArm();
	if (controlPanelButtons[6].mode) paintCircle();
	paintRobotSleep(75);
}

void axis3DecrementButtonCallback() {
	// rotate axis3 -1 degree
	printf("Axis 3 Decrement Button Pressed!\n");
	axis3Num++;
	resetPaintArm();
	if (controlPanelButtons[6].mode) paintCircle();
	paintRobotSleep(75);
}

void paintButtonCallback() {
	// toggle painting mode
	printf("Paint Button Pressed!\n");
	//controlPanelButtons[6].mode = !controlPanelButtons[6].mode;
	//controlPanelButtons[6].state = !controlPanelButtons[6].state;
	paintButtonMode = !paintButtonMode;
}

void clearButtonCallback() {
	printf("Clear Button Pressed!\n");
	paint.clear();
	glutPostRedisplay();
}

void resetButtonCallback() {
	printf("Reset Button Pressed!\n");
	paint.clear();
	paintArm = (*new PaintArm());
	axis1Num = 0;
	axis2Num = 0;
	axis3Num = 0;
	controlPanelButtons[6].mode = 0;
	controlPanelButtons[6].state = 0;
	glutPostRedisplay();
}

void colorWhiteButtonCallback() {
	printf("White Button Pressed!\n");
	controlPanelButtons[9].mode = 0;
	controlPanelButtons[9].state = 0;
	paintBrushColor = Color( 1.0f, 1.0f, 1.0f ); 
	controlPanelButtons[10].mode = 0;
	controlPanelButtons[11].mode = 0;
	controlPanelButtons[12].mode = 0;

	controlPanelButtons[10].state = 0;
	controlPanelButtons[11].state = 0;
	controlPanelButtons[12].state = 0;

	glutPostRedisplay();
}

void colorRedButtonCallback() {
	printf("Red Button Pressed!\n");
	controlPanelButtons[10].mode = 0;
	controlPanelButtons[10].state = 0;
	paintBrushColor = Color( 1.0f, 0.0f, 0.0f );
	controlPanelButtons[9].mode = 0;
	controlPanelButtons[11].mode = 0;
	controlPanelButtons[12].mode = 0;

	controlPanelButtons[9].state = 0;
	controlPanelButtons[11].state = 0;
	controlPanelButtons[12].state = 0;

	glutPostRedisplay();
}

void colorBlueButtonCallback() {
	printf("Blue Button Pressed!\n");
	controlPanelButtons[11].mode = 0;
	controlPanelButtons[11].state = 0;
	paintBrushColor = Color( 0.0f, 0.0f, 1.0f );
	controlPanelButtons[9].mode = 0;
	controlPanelButtons[10].mode = 0;
	controlPanelButtons[12].mode = 0;

	controlPanelButtons[9].state = 0;
	controlPanelButtons[10].state = 0;
	controlPanelButtons[12].state = 0;

	glutPostRedisplay();
}

void colorGreenButtonCallback() {
	printf("Green Button Pressed!\n");
	controlPanelButtons[12].mode = 0;
	controlPanelButtons[12].state = 0;
	paintBrushColor = Color( 0.0f, 1.0f, 0.0f );
	controlPanelButtons[9].mode = 0;
	controlPanelButtons[10].mode = 0;
	controlPanelButtons[11].mode = 0;

	controlPanelButtons[9].state = 0;
	controlPanelButtons[10].state = 0;
	controlPanelButtons[11].state = 0;

	glutPostRedisplay();
}

void initButtons() {

	int buttonHeight = 0.05 * controlPanelHeight;
	int buttonWidth = (0.85 * controlPanelWidth) / 2;

	int decrementButtonX = 0.07 * controlPanelWidth;
	int incrementButtonX = (controlPanelWidth / 2) + 0.025 * controlPanelWidth;

	int axis1ButtonY = 0.12 * controlPanelHeight;
	Button axis1DecrementButton = { decrementButtonX, axis1ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "-1", axis1DecrementButtonCallback, 0 };
	Button axis1IncrementButton = { incrementButtonX, axis1ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "+1", axis1IncrementButtonCallback, 0 };

	int axis2ButtonY = 0.27 * controlPanelHeight;
	Button axis2DecrementButton = { decrementButtonX, axis2ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "-1", axis2DecrementButtonCallback, 0 };
	Button axis2IncrementButton = { incrementButtonX, axis2ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "+1", axis2IncrementButtonCallback, 0 };
	
	int axis3ButtonY = 0.42 * controlPanelHeight;
	Button axis3DecrementButton = { decrementButtonX, axis3ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "-1", axis3DecrementButtonCallback, 0 };
	Button axis3IncrementButton = { incrementButtonX, axis3ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "+1", axis3IncrementButtonCallback, 0 };

	int paintButtonY = 0.57 * controlPanelHeight;
	int paintButtonWidth = 0.9 * controlPanelWidth;
	Button paintButton = { decrementButtonX, paintButtonY, paintButtonWidth, 25, Button::TOGGLE, 0, 0, 0, "Paint", paintButtonCallback, 1 };

	int clearButtonY = 0.72 * controlPanelHeight;
	Button clearButton = { decrementButtonX, clearButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "Clear Paint", clearButtonCallback, 1 };
	Button resetButton = { incrementButtonX, clearButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "Reset", resetButtonCallback, 1 };

	int colorRow1ButtonY = 0.87 * controlPanelHeight;
	Button colorWhiteButton = { decrementButtonX, colorRow1ButtonY, buttonWidth, 25, Button::TOGGLE, 1, 1, 0, "White", colorWhiteButtonCallback, 1 };
	Button colorRedButton = { incrementButtonX, colorRow1ButtonY, buttonWidth, 25, Button::TOGGLE, 0, 0, 0, "Red", colorRedButtonCallback, 1 };

	int colorRow2ButtonY = 0.87 * controlPanelHeight + 30;
	Button colorBlueButton = { decrementButtonX, colorRow2ButtonY, buttonWidth, 25, Button::TOGGLE, 0, 0, 0, "Blue", colorBlueButtonCallback, 1 };
	Button colorGreenButton = { incrementButtonX, colorRow2ButtonY, buttonWidth, 25, Button::TOGGLE, 0, 0, 0, "Green", colorGreenButtonCallback, 1 };



	controlPanelButtons.push_back(axis1DecrementButton); // 0
	controlPanelButtons.push_back(axis1IncrementButton); // 1

	controlPanelButtons.push_back(axis2DecrementButton); // 2
	controlPanelButtons.push_back(axis2IncrementButton); // 3

	controlPanelButtons.push_back(axis3DecrementButton); // 4
	controlPanelButtons.push_back(axis3IncrementButton); // 5

	controlPanelButtons.push_back(paintButton); // 6

	controlPanelButtons.push_back(clearButton); // 7
	controlPanelButtons.push_back(resetButton); // 8

	controlPanelButtons.push_back(colorWhiteButton); // 9
	controlPanelButtons.push_back(colorRedButton); // 10
	controlPanelButtons.push_back(colorBlueButton); // 11
	controlPanelButtons.push_back(colorGreenButton); // 12
}



/*----------------------------------------------------------------------------------------
*	\brief	This function is called whenever a mouse button is pressed or released
*	\param	button	-	GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
*	\param	state	-	GLUT_UP or GLUT_DOWN depending on whether the mouse was released
*						or pressed respectivly.
*	\param	x		-	the x-coord of the mouse cursor.
*	\param	y		-	the y-coord of the mouse cursor.
*/
void mouseButtonPressed(int button, int state, int x, int y)
{
	/*
	*	update the mouse position
	*/
	mouse.x = x;
	mouse.y = y;

	/*
	*	has the button been pressed or released?
	*/
	if (state == GLUT_DOWN)
	{
		/*
		*	This holds the location of the first mouse click
		*/
			mouse.xpress = x;
			mouse.ypress = y;

		/*
		*	Which button was pressed?
		*/
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			mouse.lmb = 1;
			ButtonsPressed(x, y);
		case GLUT_MIDDLE_BUTTON:
			mouse.mmb = 1;
			break;
		case GLUT_RIGHT_BUTTON:
			mouse.rmb = 1;
			break;
		}
	}
	else
	{
		/*
		*	Which button was released?
		*/
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			mouse.lmb = 0;
			ButtonsReleased(x, y);
			break;
		case GLUT_MIDDLE_BUTTON:
			mouse.mmb = 0;
			break;
		case GLUT_RIGHT_BUTTON:
			mouse.rmb = 0;
			break;
		}
	}

	/*
	*	Force a redraw of the screen. If we later want interactions with the mouse
	*	and the 3D scene, we will need to redraw the changes.
	*/
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
*	\brief	This function is called whenever the mouse cursor is moved AND A BUTTON IS HELD.
*	\param	x	-	the new x-coord of the mouse cursor.
*	\param	y	-	the new y-coord of the mouse cursor.
*/
void mouseMotion(int x, int y)
{
	/*
	*	update the mouse position
	*/
	mouse.x = x;
	mouse.y = y;


	/*
	*	Check MyButton to see if we should highlight it cos the mouse is over it
	*/
	ButtonsPassive(x, y);

	/*
	*	Force a redraw of the screen
	*/
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
*	\brief	This function is called whenever the mouse cursor is moved AND NO BUTTONS ARE HELD.
*	\param	x	-	the new x-coord of the mouse cursor.
*	\param	y	-	the new y-coord of the mouse cursor.
*/
void mousePassiveMotion(int x, int y)
{
	/*
	*	update the mouse position
	*/
	mouse.x = x;
	mouse.y = y;

	/*
	*	Check MyButton to see if we should highlight it cos the mouse is over it
	*/
	ButtonsPassive(x, y);

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		controlPanelButtons[6].mode = 1;
		controlPanelButtons[6].state = 1;
		paintButtonMode = 1;
		for (int i = 0; i < 360; ++i) {
			axis2DecrementButtonCallback();
			if (i % 5 == 0) {
				axis3DecrementButtonCallback();
			}
			if (!instantPaint) draw();
		}
		controlPanelButtons[6].mode = 0;
		controlPanelButtons[6].state = 0;
		paintButtonMode = 0;
		break;
	case '2':
		controlPanelButtons[6].mode = 1;
		controlPanelButtons[6].state = 1;
		paintButtonMode = 1;
		for (int i = 0; i < 90; ++i) {
			axis3IncrementButtonCallback();
			if (i % 9 == 0) {
				axis2DecrementButtonCallback();
			}
			if (i % 3 == 0) {
				axis2IncrementButtonCallback();
			}
			if (!instantPaint) draw();
		}
		controlPanelButtons[6].mode = 0;
		controlPanelButtons[6].state = 0;
		paintButtonMode = 0;
		break;
	case '3':
		controlPanelButtons[6].mode = 1;
		controlPanelButtons[6].state = 1;
		paintButtonMode = 1;
		for (int i = 0; i < 45; ++i) {
			axis3IncrementButtonCallback();
			if (i % 9 == 0) {
				axis1IncrementButtonCallback();
			}
			if (i % 6 == 0) {
				axis1DecrementButtonCallback();
			}
			if (i % 5 == 0) {
				axis2DecrementButtonCallback();
			}
			if (i % 3 == 0) {
				axis2IncrementButtonCallback();
			}
			if (!instantPaint) draw();
		}
		controlPanelButtons[6].mode = 0;
		controlPanelButtons[6].state = 0;
		paintButtonMode = 0;
		break;
	case '4':
		controlPanelButtons[6].mode = 1;
		controlPanelButtons[6].state = 1;
		paintButtonMode = 1;
		for (int i = 0; i < 45; ++i) {
			axis3IncrementButtonCallback();
			if (i % 9 == 0) {
				axis1DecrementButtonCallback();
			}
			if (i % 6 == 0) {
				axis1IncrementButtonCallback();
			}
			if (i % 5 == 0) {
				axis2DecrementButtonCallback();
			}
			if (i % 3 == 0) {
				axis2IncrementButtonCallback();
			}
			if (!instantPaint) draw();
		}
		controlPanelButtons[6].mode = 0;
		controlPanelButtons[6].state = 0;
		paintButtonMode = 0;
		break;
	case 'c':
		clearButtonCallback();
		break;
	case 'r':
		resetButtonCallback();
		break;
	case 'q':
		axis1DecrementButtonCallback();
		break;
	case 'w':
		axis1IncrementButtonCallback();
		break;	
	case 'a':
		axis2DecrementButtonCallback();
		break;
	case 's':
		axis2IncrementButtonCallback();
		break;
	case 'z':
		axis3DecrementButtonCallback();
		break;
	case 'x':
		axis3IncrementButtonCallback();
		break;
	case 'i':
		instantPaint = !instantPaint;
		break;
	}
	glutPostRedisplay();
}

void initGraphics() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, 0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(200, 100);
	glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(draw);
	glutMouseFunc(mouseButtonPressed);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mousePassiveMotion);
	glutKeyboardFunc(keyboard);

	initGraphics();
	initButtons();

	glutMainLoop();
}
