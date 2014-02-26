/* implementation of button code adapted from http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/opengl_programming.html#6 */

#include "stdafx.h"

struct Point {
	int x, y;
	Point(int x, int y) : x(x), y(y) {}
};
typedef struct Point Point;

PaintArm paintArm;

std::vector<Button> controlPanelButtons;
std::vector<Point> paint;

Mouse mouse = { 0, 0, 0, 0, 0 };

/*----------------------------------------------------------------------------------------
*	\brief	This function draws a text string to the screen using glut bitmap fonts.
*	\param	font	-	the font to use. it can be one of the following :
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
				b->callbackFunction();
			}
		}

		/*
		*	Set state back to zero.
		*/
		b->state = 0;
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

void drawRobotAreaContents() {
	// draw the robot
	glPushMatrix();
	gotoRobotArea();
	glBegin(GL_LINES);
		glVertex2f(slidePosX, slidePosY);
		glVertex2f(slidePosX + SLIDE_LENGTH, slidePosY);
	glEnd();

	// TODO: Draw the rest of the robot
	//Matrix* paintArmAxis01 = paintArm.get_T_Matrix(1, 1);
	//Matrix* paintArmAxis02 = paintArm.get_T_Matrix(0, 2);

	//printf("01X: %d, 01Y: %d\n", paintArmAxis01->get_elem(0, 3), paintArmAxis01->get_elem(1, 3));
	//printf("01X: %d, 01Y: %d\n", paintArmAxis02->get_elem(0, 3), paintArmAxis02->get_elem(1, 3));

	//glBegin(GL_LINES);
	//glVertex2f(paintArmAxis01->get_elem(0, 3), paintArmAxis01->get_elem(1, 3));
	//glVertex2f(paintArmAxis02->get_elem(0, 3), paintArmAxis02->get_elem(1, 3));
	//glEnd();

	glPopMatrix();
}

void drawControlPanelContents() {
	// draw the controls
	int cpTitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)cpTitle)) / 2;
	int cpTitleY = 0.05 * controlPanelHeight;
	
	int cpAxis1TitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpAxis1Title)) / 2;
	int cpAxis1TitleY = 0.12 * controlPanelHeight;

	int cpAxis2TitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpAxis2Title)) / 2;
	int cpAxis2TitleY = 0.27 * controlPanelHeight;

	int cpAxis3TitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpAxis2Title)) / 2;
	int cpAxis3TitleY = 0.42 * controlPanelHeight;

	int cpPaintBrushTitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpPaintBrushTitle)) / 2;
	int cpPaintBrushTitleY = 0.57 * controlPanelHeight;


	glPushMatrix();
	gotoControlPanel();
	font(GLUT_BITMAP_HELVETICA_18, cpTitle, cpTitleX, cpTitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpAxis1Title, cpAxis1TitleX, cpAxis1TitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpAxis2Title, cpAxis2TitleX, cpAxis2TitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpAxis3Title, cpAxis3TitleX, cpAxis3TitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpPaintBrushTitle, cpPaintBrushTitleX, cpPaintBrushTitleY);


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

	drawOverlay();
	drawRobotAreaContents();
	drawControlPanelContents();

	glFlush();
}



/*
 * Button Callbacks
 */
void axis1IncrementButtonCallback() {
	// move axis1 1px right
	printf("Axis 1 Increment Button Pressed!\n");
}

void axis1DecrementButtonCallback() {
	// move axis1 1px left
	printf("Axis 1 Decrement Button Pressed!\n");
}

void axis2IncrementButtonCallback() {
	// rotate axis2 +1 degree
	printf("Axis 2 Increment Button Pressed!\n");
}

void axis2DecrementButtonCallback() {
	// rotate axis2 -1 degree
	printf("Axis 2 Decrement Button Pressed!\n");
}

void axis3IncrementButtonCallback() {
	// rotate axis3 +1 degree
	printf("Axis 3 Increment Button Pressed!\n");
}

void axis3DecrementButtonCallback() {
	// rotate axis3 -1 degree
	printf("Axis 3 Decrement Button Pressed!\n");
}

void paintButtonCallback() {
	// add a new circle to paint
	printf("Paint Button Pressed!\n");
	// TODO: Get paint brush x,y coordinates
	//paint.push_back(Point(x,y));
}


void initButtons() {

	int buttonHeight = 0.05 * controlPanelHeight;
	int buttonWidth = (0.85 * controlPanelWidth) / 2;

	int decrementButtonX = 0.05 * controlPanelWidth;
	int incrementButtonX = (controlPanelWidth / 2) + 0.025 * controlPanelWidth;

	int axis1ButtonY = 0.15 * controlPanelHeight;
	Button axis1DecrementButton = { decrementButtonX, axis1ButtonY, buttonWidth, 25, 0, 0, "-1", axis1DecrementButtonCallback };
	Button axis1IncrementButton = { incrementButtonX, axis1ButtonY, buttonWidth, 25, 0, 0, "+1", axis1IncrementButtonCallback };

	int axis2ButtonY = 0.30 * controlPanelHeight;
	Button axis2DecrementButton = { decrementButtonX, axis2ButtonY, buttonWidth, 25, 0, 0, "-1", axis2DecrementButtonCallback };
	Button axis2IncrementButton = { incrementButtonX, axis2ButtonY, buttonWidth, 25, 0, 0, "+1", axis2IncrementButtonCallback };
	
	int axis3ButtonY = 0.45 * controlPanelHeight;
	Button axis3DecrementButton = { decrementButtonX, axis3ButtonY, buttonWidth, 25, 0, 0, "-1", axis3DecrementButtonCallback };
	Button axis3IncrementButton = { incrementButtonX, axis3ButtonY, buttonWidth, 25, 0, 0, "+1", axis3IncrementButtonCallback };

	int paintButtonY = 0.60 * controlPanelHeight;
	int paintButtonWidth = 0.9 * controlPanelWidth;
	Button paintButton = { decrementButtonX, paintButtonY, paintButtonWidth, 25, 0, 0, "Paint", paintButtonCallback };


	controlPanelButtons.push_back(axis1DecrementButton);
	controlPanelButtons.push_back(axis1IncrementButton);

	controlPanelButtons.push_back(axis2DecrementButton);
	controlPanelButtons.push_back(axis2IncrementButton);

	controlPanelButtons.push_back(axis3DecrementButton);
	controlPanelButtons.push_back(axis3IncrementButton);

	controlPanelButtons.push_back(paintButton);
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


void initGraphics() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, 0);
}

void initPaintArm() {
	(paintArm.get_T_Matrix(0, 0))->print(std::cout);
	// Base to first joint
	(paintArm.get_T_Matrix(1, 1))->print(std::cout);
	// First joint to second
	(paintArm.get_T_Matrix(2, 2))->print(std::cout);
	// Second joint to third
	(paintArm.get_T_Matrix(3, 3))->print(std::cout);
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

	initGraphics();
	initButtons();
	initPaintArm();

	glutMainLoop();
}