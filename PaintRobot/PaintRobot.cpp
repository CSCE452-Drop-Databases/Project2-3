#include "stdafx.h"

typedef std::map<char*, Button> ButtonMap;
typedef ButtonMap::iterator it_ButtonMap;

Mouse mouse = { 0, 0, 0, 0, 0 };

PaintArm paintArm;
Color paintBrushColor = Color(1.0f, 1.0f, 1.0f);
int paintButtonMode = 0;
int instantPaint = 0; // for use with the macro paint buttons
ButtonMap controlPanelButtons;
std::vector<PaintCircle> paint;

//TODO remove these when drawing works without hack
int axis1Num = 0;
int axis2Num = 0;
int axis3Num = 0;

Point paintArmAxisMidpoint(int axis1, int axis2) {
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

void ButtonsPressed(int x, int y) {
	for (it_ButtonMap iterator = controlPanelButtons.begin(); iterator != controlPanelButtons.end(); iterator++) {
		ButtonPress(&iterator->second, x, y);
	}
}

void ButtonsReleased(int x, int y) {
	for (it_ButtonMap iterator = controlPanelButtons.begin(); iterator != controlPanelButtons.end(); iterator++) {
		ButtonRelease(&iterator->second, x, y, mouse.xpress, mouse.ypress);
	}
}

void ButtonsPassive(int x, int y) {
	for (it_ButtonMap iterator = controlPanelButtons.begin(); iterator != controlPanelButtons.end(); iterator++) {
		ButtonPassive(&iterator->second, x, y);
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
	glTranslatef(robotAreaOriginX, robotAreaOriginY, 0.0f);
}

void gotoControlPanelJoint() {
	// translates to the top left of joint control panel
	// in relation to the origin
	glTranslatef(controlPanelJointOriginX, controlPanelJointOriginY, 0.0f);
}

void gotoControlPanelWorld() {
	// translates to the top left of world control panel
	// in relation to the origin
	glTranslatef(controlPanelWorldOriginX, controlPanelWorldOriginY, 0.0f);
}

void drawRobotAreaContents() {
	// draw the robot
	glPushMatrix();
	gotoRobotArea();

	glTranslatef(slidePosX + SLIDE_LENGTH / 2, slidePosY, 0.0f); // Translate to middle of bar (robot origin)

	for (PaintCircle p : paint) {
		glBegin(GL_LINES);
		glColor3f(p.c.r, p.c.g, p.c.b);
		ellipseMidpoint(p.p.x, -p.p.y, PAINT_CIRCLE_RADIUS, PAINT_CIRCLE_RADIUS);
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

	Point midpoint01 = paintArmAxisMidpoint(0, 1);
	Point midpoint12 = paintArmAxisMidpoint(1, 2);
	Point midpoint23 = paintArmAxisMidpoint(2, 3);

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

void drawControlPanelJointContents() {
	// draw the controls
	int cpTitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)cpjTitle)) / 2;
	int cpTitleY = 20;

	int cpAxis1TitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpjAxis1Title)) / 2;
	int cpAxis1TitleY = 0.17 * controlPanelJointHeight;

	int cpAxis2TitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpjAxis2Title)) / 2;
	int cpAxis2TitleY = 0.32 * controlPanelJointHeight;

	int cpAxis3TitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpjAxis2Title)) / 2;
	int cpAxis3TitleY = 0.47 * controlPanelJointHeight;

	int cpClearTitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpjClearTitle)) / 2;
	int cpClearTitleY = 0.62 * controlPanelJointHeight;

	int cpColorTitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)cpjColorTitle)) / 2;
	int cpColorTitleY = 0.77 * controlPanelJointHeight;

	glPushMatrix();
	gotoControlPanelJoint();
	glColor3f(0.0f, 1.0f, 0.0f);
	font(GLUT_BITMAP_HELVETICA_18, cpjTitle, cpTitleX, cpTitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpjAxis1Title, cpAxis1TitleX, cpAxis1TitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpjAxis2Title, cpAxis2TitleX, cpAxis2TitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpjAxis3Title, cpAxis3TitleX, cpAxis3TitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpjClearTitle, cpClearTitleX, cpClearTitleY);
	font(GLUT_BITMAP_HELVETICA_12, cpjColorTitle, cpColorTitleX, cpColorTitleY);

	glPopMatrix();
}

void drawControlPanelWorldContents() {
	int cpTitleX = (controlPanelWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)cpwTitle)) / 2;
	int cpTitleY = 20;

	glPushMatrix();
	gotoControlPanelWorld();
	glColor3f(0.0f, 1.0f, 0.0f);
	font(GLUT_BITMAP_HELVETICA_18, cpwTitle, cpTitleX, cpTitleY);

	glPopMatrix();

}

void drawButtons() {
	for (it_ButtonMap iterator = controlPanelButtons.begin(); iterator != controlPanelButtons.end(); iterator++) {
		drawButton(&iterator->second);
	}
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

	// draw control panel joint
	glPushMatrix();
	gotoControlPanelJoint();
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, controlPanelJointHeight);
	glVertex2f(controlPanelWidth, controlPanelJointHeight);
	glVertex2f(controlPanelWidth, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();

	// draw control panel world
	glPushMatrix();
	gotoControlPanelWorld();
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, controlPanelWorldHeight);
	glVertex2f(controlPanelWidth, controlPanelWorldHeight);
	glVertex2f(controlPanelWidth, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	drawRobotAreaContents();
	drawControlPanelJointContents();
	drawControlPanelWorldContents();
	drawButtons();
	drawOverlay();

	glFlush();
}
void paintCircle() {
	Point endEffector = paintArm.getEndEffectorCoords();
	PaintCircle circle = { endEffector, Color(paintBrushColor.r, paintBrushColor.g, paintBrushColor.b) };
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
		if (paintButtonMode) paintCircle();
		paintRobotSleep(JOINT_SLEEP_TIME);
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
		if (paintButtonMode) paintCircle();
		paintRobotSleep(JOINT_SLEEP_TIME);
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
	if (paintButtonMode) paintCircle();
	paintRobotSleep(JOINT_SLEEP_TIME);
}

void axis2DecrementButtonCallback() {
	// rotate axis2 -1 degree
	printf("Axis 2 Decrement Button Pressed!\n");
	axis2Num++;
	resetPaintArm();
	if (paintButtonMode) paintCircle();
	paintRobotSleep(JOINT_SLEEP_TIME);
}

void axis3IncrementButtonCallback() {
	// rotate axis3 +1 degree
	printf("Axis 3 Increment Button Pressed!\n");
	axis3Num--;
	resetPaintArm();
	if (paintButtonMode) paintCircle();
	paintRobotSleep(JOINT_SLEEP_TIME);
}

void axis3DecrementButtonCallback() {
	// rotate axis3 -1 degree
	printf("Axis 3 Decrement Button Pressed!\n");
	axis3Num++;
	resetPaintArm();
	if (paintButtonMode) paintCircle();
	paintRobotSleep(JOINT_SLEEP_TIME);
}

void paintButtonCallback() {
	// toggle painting mode
	printf("Paint Button Pressed!\n");
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
	controlPanelButtons["paint"].mode = 0;
	controlPanelButtons["paint"].state = 0;
	paintButtonMode = 0;
	glutPostRedisplay();
}

void colorWhiteButtonCallback() {
	printf("White Button Pressed!\n");
	controlPanelButtons["colorWhite"].mode = 0;
	controlPanelButtons["colorWhite"].state = 0;
	paintBrushColor = Color(1.0f, 1.0f, 1.0f);
	controlPanelButtons["colorRed"].mode = 0;
	controlPanelButtons["colorBlue"].mode = 0;
	controlPanelButtons["colorGreen"].mode = 0;

	controlPanelButtons["colorRed"].state = 0;
	controlPanelButtons["colorBlue"].state = 0;
	controlPanelButtons["colorGreen"].state = 0;

	glutPostRedisplay();
}

void colorRedButtonCallback() {
	printf("Red Button Pressed!\n");
	controlPanelButtons["colorRed"].mode = 0;
	controlPanelButtons["colorRed"].state = 0;
	paintBrushColor = Color(1.0f, 0.0f, 0.0f);
	controlPanelButtons["colorWhite"].mode = 0;
	controlPanelButtons["colorBlue"].mode = 0;
	controlPanelButtons["colorGreen"].mode = 0;

	controlPanelButtons["colorWhite"].state = 0;
	controlPanelButtons["colorBlue"].state = 0;
	controlPanelButtons["colorGreen"].state = 0;

	glutPostRedisplay();
}

void colorBlueButtonCallback() {
	printf("Blue Button Pressed!\n");
	controlPanelButtons["colorBlue"].mode = 0;
	controlPanelButtons["colorBlue"].state = 0;
	paintBrushColor = Color(0.0f, 0.0f, 1.0f);
	controlPanelButtons["colorWhite"].mode = 0;
	controlPanelButtons["colorRed"].mode = 0;
	controlPanelButtons["colorGreen"].mode = 0;

	controlPanelButtons["colorWhite"].state = 0;
	controlPanelButtons["colorRed"].state = 0;
	controlPanelButtons["colorGreen"].state = 0;

	glutPostRedisplay();
}

void colorGreenButtonCallback() {
	printf("Green Button Pressed!\n");
	controlPanelButtons["colorGreen"].mode = 0;
	controlPanelButtons["colorGreen"].state = 0;
	paintBrushColor = Color(0.0f, 1.0f, 0.0f);
	controlPanelButtons["colorWhite"].mode = 0;
	controlPanelButtons["colorRed"].mode = 0;
	controlPanelButtons["colorBlue"].mode = 0;

	controlPanelButtons["colorWhite"].state = 0;
	controlPanelButtons["colorRed"].state = 0;
	controlPanelButtons["colorBlue"].state = 0;

	glutPostRedisplay();
}

void changeEndEffector(double dx, double dy) {
	Point endEffector = paintArm.getEndEffectorCoords();
	double endPosX = endEffector.x + dx;
	double endPosY = endEffector.y + dy;
	int invkin = paintArm.calc_Inverse_Kinematics(endPosX, endPosY);

	if (invkin == 0) {
		//it is reachable!
		//TODO update/redraw arm, don't reset it's position
		resetPaintArm();
		if (paintButtonMode) paintCircle();
		paintRobotSleep(JOINT_SLEEP_TIME);
	} else if (invkin == 1) {
		//error
		printf("Error: point not reachable:\n%d, %d\n", endPosX, endPosY);
	} else {
		printf("Unknown Error: %i\n", invkin);
	}
}

void worldXDecrementButtonCallback() {
	printf("World X Decrement Button Pressed!\n");
	changeEndEffector(-1, 0);
}

void worldXIncrementButtonCallback() {
	printf("World X Increment Button Pressed!\n");
	changeEndEffector(1, 0);
}

void worldYDecrementButtonCallback() {
	printf("World Y Decrement Button Pressed!\n");
	changeEndEffector(0, -1);
}

void worldYIncrementButtonCallback() {
	printf("World Y Increment Button Pressed!\n");
	changeEndEffector(0, 1);
}

/* Initialization Functions */
// Init buttons in Joint Control Panel
void initJointButtons() {
	int buttonWidth = (0.85 * controlPanelWidth) / 2;

	int decrementButtonX = 0.07 * controlPanelWidth + controlPanelJointOriginX;
	int incrementButtonX = (controlPanelWidth / 2) + 0.025 * controlPanelWidth + controlPanelJointOriginX;

	int axis1ButtonY = 0.20 * controlPanelJointHeight + controlPanelJointOriginY;
	Button axis1DecrementButton = { decrementButtonX, axis1ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "-1", axis1DecrementButtonCallback, 0 };
	Button axis1IncrementButton = { incrementButtonX, axis1ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "+1", axis1IncrementButtonCallback, 0 };

	int axis2ButtonY = 0.35 * controlPanelJointHeight + controlPanelJointOriginY;
	Button axis2DecrementButton = { decrementButtonX, axis2ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "-1", axis2DecrementButtonCallback, 0 };
	Button axis2IncrementButton = { incrementButtonX, axis2ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "+1", axis2IncrementButtonCallback, 0 };

	int axis3ButtonY = 0.50 * controlPanelJointHeight + controlPanelJointOriginY;
	Button axis3DecrementButton = { decrementButtonX, axis3ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "-1", axis3DecrementButtonCallback, 0 };
	Button axis3IncrementButton = { incrementButtonX, axis3ButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "+1", axis3IncrementButtonCallback, 0 };

	int clearButtonY = 0.65 * controlPanelJointHeight + controlPanelJointOriginY;
	Button clearButton = { decrementButtonX, clearButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "Clear Paint", clearButtonCallback, 1 };
	Button resetButton = { incrementButtonX, clearButtonY, buttonWidth, 25, Button::PRESS, 0, 0, 0, "Reset", resetButtonCallback, 1 };

	int colorRow1ButtonY = 0.80 * controlPanelJointHeight + controlPanelJointOriginY;
	Button colorWhiteButton = { decrementButtonX, colorRow1ButtonY, buttonWidth, 25, Button::TOGGLE, 1, 1, 0, "White", colorWhiteButtonCallback, 1 };
	Button colorRedButton = { incrementButtonX, colorRow1ButtonY, buttonWidth, 25, Button::TOGGLE, 0, 0, 0, "Red", colorRedButtonCallback, 1 };

	int colorRow2ButtonY = 0.80 * controlPanelJointHeight + 30 + controlPanelJointOriginY;
	Button colorBlueButton = { decrementButtonX, colorRow2ButtonY, buttonWidth, 25, Button::TOGGLE, 0, 0, 0, "Blue", colorBlueButtonCallback, 1 };
	Button colorGreenButton = { incrementButtonX, colorRow2ButtonY, buttonWidth, 25, Button::TOGGLE, 0, 0, 0, "Green", colorGreenButtonCallback, 1 };

	controlPanelButtons["axis1Decrement"] = axis1DecrementButton;
	controlPanelButtons["axis1Increment"] = axis1IncrementButton;

	controlPanelButtons["axis2Decrement"] = axis2DecrementButton;
	controlPanelButtons["axis2Increment"] = axis2IncrementButton;

	controlPanelButtons["axis3Decrement"] = axis3DecrementButton;
	controlPanelButtons["axis3Increment"] = axis3IncrementButton;

	controlPanelButtons["clear"] = clearButton;
	controlPanelButtons["reset"] = resetButton;

	controlPanelButtons["colorWhite"] = colorWhiteButton;
	controlPanelButtons["colorRed"] = colorRedButton;
	controlPanelButtons["colorBlue"] = colorBlueButton;
	controlPanelButtons["colorGreen"] = colorGreenButton;
	
}

// Init buttons in World Control Panel
void initWorldButtons() {
	int buttonWidth = 40;
	int buttonHeight = 40;

	int colButtonX = controlPanelWidth / 2 - buttonWidth / 2 + controlPanelWorldOriginX;
	int rowButtonY = (controlPanelWorldHeight - 20) / 2 - buttonHeight / 2 + 20 + controlPanelWorldOriginY;

	int paintButtonY = (controlPanelWorldHeight - 20) / 2 + controlPanelWorldOriginY;
	Button paintButton = { colButtonX, paintButtonY, buttonWidth, buttonHeight, Button::TOGGLE, 0, 0, 0, "Paint", paintButtonCallback, 1 };

	int decrementButtonXX = colButtonX - buttonWidth - 10;
	Button xDecrementButton = { decrementButtonXX, rowButtonY, buttonWidth, buttonHeight, Button::PRESS, 0, 0, 0, "-1", worldXDecrementButtonCallback, 0 };

	int incrementButtonXX = colButtonX + buttonWidth + 10;
	Button xIncrementButton = { incrementButtonXX, rowButtonY, buttonWidth, buttonHeight, Button::PRESS, 0, 0, 0, "+1", worldXIncrementButtonCallback, 0 };

	int decrementButtonYY = rowButtonY + buttonHeight + 10;
	Button yDecrementButton = { colButtonX, decrementButtonYY, buttonWidth, buttonHeight, Button::PRESS, 0, 0, 0, "-1", worldYDecrementButtonCallback, 0 };

	int incrementButtonYY = rowButtonY - buttonHeight - 10;
	Button yIncrementButton = { colButtonX, incrementButtonYY, buttonWidth, buttonHeight, Button::PRESS, 0, 0, 0, "+1", worldYIncrementButtonCallback, 0 };

	controlPanelButtons["paint"] = paintButton;
	controlPanelButtons["xDecrement"] = xDecrementButton;
	controlPanelButtons["xIncrement"] = xIncrementButton;
	controlPanelButtons["yDecrement"] = yDecrementButton;
	controlPanelButtons["yIncrement"] = yIncrementButton;
}

/*----------------------------------------------------------------------------------------
*   \brief  This function is called whenever a mouse button is pressed or released
*   \param  button  -   GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
*   \param  state   -   GLUT_UP or GLUT_DOWN depending on whether the mouse was released
*                       or pressed respectivly.
*   \param  x       -   the x-coord of the mouse cursor.
*   \param  y       -   the y-coord of the mouse cursor.
*/
void mouseButtonPressed(int button, int state, int x, int y)
{
	/*
	*   update the mouse position
	*/
	mouse.x = x;
	mouse.y = y;

	/*
	*   has the button been pressed or released?
	*/
	if (state == GLUT_DOWN)
	{
		/*
		*   This holds the location of the first mouse click
		*/
		mouse.xpress = x;
		mouse.ypress = y;

		/*
		*   Which button was pressed?
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
		*   Which button was released?
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
	*   Force a redraw of the screen. If we later want interactions with the mouse
	*   and the 3D scene, we will need to redraw the changes.
	*/
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
*   \brief  This function is called whenever the mouse cursor is moved AND A BUTTON IS HELD.
*   \param  x   -   the new x-coord of the mouse cursor.
*   \param  y   -   the new y-coord of the mouse cursor.
*/
void mouseMotion(int x, int y)
{
	/*
	*   update the mouse position
	*/
	mouse.x = x;
	mouse.y = y;

	/*
	*   Check MyButton to see if we should highlight it cos the mouse is over it
	*/
	ButtonsPassive(x, y);

	/*
	*   Force a redraw of the screen
	*/
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
*   \brief  This function is called whenever the mouse cursor is moved AND NO BUTTONS ARE HELD.
*   \param  x   -   the new x-coord of the mouse cursor.
*   \param  y   -   the new y-coord of the mouse cursor.
*/
void mousePassiveMotion(int x, int y)
{
	/*
	*   update the mouse position
	*/
	mouse.x = x;
	mouse.y = y;

	/*
	*   Check MyButton to see if we should highlight it cos the mouse is over it
	*/
	ButtonsPassive(x, y);

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		paintButtonMode = 1;
		for (int i = 0; i < 360; ++i) {
			axis2DecrementButtonCallback();
			if (i % 5 == 0) {
				axis3DecrementButtonCallback();
			}
			if (!instantPaint) draw();
		}
		paintButtonMode = 0;
		break;
	case '2':
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
		paintButtonMode = 0;
		break;
	case '3':
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
		paintButtonMode = 0;
		break;
	case '4':
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
	initJointButtons();
	initWorldButtons();

	glutMainLoop();
}
