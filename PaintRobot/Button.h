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
	int   x;							/* top left x coord of the button */
	int   y;							/* top left y coord of the button */
	int   w;							/* the width of the button */
	int   h;							/* the height of the button */
	int	  state;						/* the state, 1 if pressed, 0 otherwise */
	int	  highlighted;					/* is the mouse cursor over the control? */
	char* label;						/* the text label of the button */
	ButtonCallback callbackFunction;	/* A pointer to a function to call if the button is pressed */
};
