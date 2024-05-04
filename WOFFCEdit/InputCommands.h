#pragma once

struct InputCommands
{
	bool forward;
	bool back;
	bool right;
	bool left;
	bool up;
	bool down;
	bool rotRight;
	bool rotLeft;

	float mouseX;
	float mouseY;
	float mouseXDrag;
	float mouseYDrag;

	bool mouseLeftButton;
	bool mouseLeftButtonDown;
	bool mouseRightButton;
	bool rightMouseButtonDown;

	bool controlButton;

};
