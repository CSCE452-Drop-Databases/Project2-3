#include "stdafx.h"

/*
	Draws text in font at position x, y
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

void paintRobotSleep(int ms) {
	std::chrono::milliseconds timespan(ms);
	std::this_thread::sleep_for(timespan);
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