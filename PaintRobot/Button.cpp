#include "stdafx.h"

int ButtonClickTest(Button* b, int x, int y)
{
	if (b)
	{
		/*
		*   If clicked within button area, then return true
		*/
		if (x > b->x        &&
			x < b->x + b->w &&
			y > b->y        &&
			y < b->y + b->h) {
			return 1;
		}
	}

	/*
	*   otherwise false.
	*/
	return 0;
}

void ButtonRelease(Button *b, int mCurX, int mCurY, int mPressX, int mPressY)
{
	if (b)
	{
		/*
		*   If the mouse button was pressed within the button area
		*   as well as being released on the button.....
		*/
		if (ButtonClickTest(b, mPressX, mPressY) &&
			ButtonClickTest(b, mCurX, mCurY))
		{
			/*
			*   Then if a callback function has been set, call it.
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

void ButtonPress(Button *b, int x, int y)
{
	if (b)
	{
		/*
		*   if the mouse click was within the buttons client area,
		*   set the state to true.
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

void ButtonPassive(Button *b, int x, int y)
{
	if (b)
	{
		/*
		*   if the mouse moved over the control
		*/
		if (ButtonClickTest(b, x, y))
		{
			/*
			*   If the cursor has just arrived over the control, set the highlighted flag
			*   and force a redraw. The screen will not be redrawn again until the mouse
			*   is no longer over this control
			*/
			if (b->highlighted == 0) {
				b->highlighted = 1;
				glutPostRedisplay();
			}
		}
		else {

			/*
			*   If the cursor is no longer over the control, then if the control
			*   is highlighted (ie, the mouse has JUST moved off the control) then
			*   we set the highlighting back to false, and force a redraw.
			*/
			if (b->highlighted == 1)
			{
				b->highlighted = 0;
				glutPostRedisplay();
			}
		}
	}
}

void drawButton(Button *b)
{
	int fontx;
	int fonty;

	if (b)
	{
		/*
		*   We will indicate that the mouse cursor is over the button by changing its
		*   colour.
		*/
		if (b->highlighted)
			glColor3f(0.7f, 0.7f, 0.8f);
		else
			glColor3f(0.6f, 0.6f, 0.6f);

		/*
		*   draw background for the button.
		*/
		glBegin(GL_QUADS);
		glVertex2i(b->x, b->y);
		glVertex2i(b->x, b->y + b->h);
		glVertex2i(b->x + b->w, b->y + b->h);
		glVertex2i(b->x + b->w, b->y);
		glEnd();

		/*
		*   Draw an outline around the button with width 3
		*/
		glLineWidth(3);

		/*
		*   The colours for the outline are reversed when the button.
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
		*   Calculate the x and y coords for the text string in order to center it.
		*/
		fontx = b->x + (b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)b->label)) / 2;
		fonty = b->y + (b->h + 10) / 2;

		/*
		*   if the button is pressed, make it look as though the string has been pushed
		*   down. It's just a visual thing to help with the overall look....
		*/
		if (b->state) {
			fontx += 2;
			fonty += 2;
		}

		/*
		*   If the cursor is currently over the button we offset the text string and draw a shadow
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