/* implementation of button code adapted from http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/opengl_programming.html#6 */
/*
*	We will define a function pointer type. ButtonCallback is a pointer to a function that
*	looks a bit like this :
*
*	void func() {
*	}
*/
typedef void(*ButtonCallback)();

/*
*	This is a simple structure that holds a button.
*/
struct Button
{
	enum ButtonType { TOGGLE, PRESS };
	int   x;							/* top left x coord of the button */
	int   y;							/* top left y coord of the button */
	int   w;							/* the width of the button */
	int   h;							/* the height of the button */
	int   type;                         /* Button type, TOGGLE, or PRESS */
	int   mode;                         /* 0 off, 1 on; for use with TOGGLE */
	int	  state;						/* the state, 1 if pressed, 0 otherwise */
	int	  highlighted;					/* is the mouse cursor over the control? */
	char* label;						/* the text label of the button */
	ButtonCallback callbackFunction;	/* A pointer to a function to call if the button is pressed */
	int cbOnRelease;                    /* 0 if Callback when pressed, 1 if Callback when button released */
};

/*----------------------------------------------------------------------------------------
*   \brief  This function is used to see if a mouse click or event is within a button
*           client area.
*   \param  b   -   a pointer to the button to test
*   \param  x   -   the x coord to test
*   \param  y   -   the y-coord to test
*/
int ButtonClickTest(Button* b, int x, int y);

/*----------------------------------------------------------------------------------------
*   \brief  This function checks if the mouse button was released in the button
*   \param  b   -   a pointer to the button to check.
*   \param  x   -   the x location of the mouse cursor.
*   \param  y   -   the y location of the mouse cursor.
*/
void ButtonRelease(Button *b, int mCurX, int mCurY, int mPressX, int mPressY);

/*----------------------------------------------------------------------------------------
*   \brief  This function checks if the button is being pressed
*   \param  b   -   a pointer to the button to check.
*   \param  x   -   the x location of the mouse cursor.
*   \param  y   -   the y location of the mouse cursor.
*/
void ButtonPress(Button *b, int x, int y);

/*----------------------------------------------------------------------------------------
*   \brief  This function checks if the mouse is currently over the button
*   \param  b   -   a pointer to the button to check.
*   \param  x   -   the x location of the mouse cursor.
*   \param  y   -   the y location of the mouse cursor.
*/
void ButtonPassive(Button *b, int x, int y);

/*----------------------------------------------------------------------------------------
*   \brief  This function draws the specified button.
*   \param  b   -   a pointer to the button to draw.
*/
void drawButton(Button *b);