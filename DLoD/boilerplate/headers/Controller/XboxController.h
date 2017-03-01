//#define _X86_
#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include <Windows.h>
#include <Xinput.h>

#pragma once

/* Source tutorial: https://lcmccauley.wordpress.com/2014/01/05/gamepad-input-tutorial/ */

static const WORD XINPUT_Btns[] = {
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK
};

struct XButtonIDs
{
	XButtonIDs();

	int A, B, X, Y;

	// D-Pad
	int DPad_Up, DPad_Down, DPad_Left, DPad_Right;

	// Bumper buttons
	int L_Shoulder, R_Shoulder;

	// Thumbstick buttons
	int L_Thumbstick, R_Thumbstick;

	int StartBtn;
	int BackBtn;
};

class XboxController
{
public:
	XboxController(); 
	XboxController(const int &index);
	~XboxController();

	void Update(); 
	void RefreshState(); // saves button states per frame

	// Stick Functionallity
	bool LStick_InDeadzone();
	bool RStick_InDeadzone();

	float LeftStick_X();  // Return X axis of left stick
	float LeftStick_Y();  // Return Y axis of left stick
	float RightStick_X(); // Return X axis of right stick
	float RightStick_Y(); // Return Y axis of right stick

	// Trigger Functionality
	float LeftTrigger();
	float RightTrigger(); 

	// Button Functionally
	bool GetButtonPressed(const int &button);
	bool GetButtonDown(const int &button);

	// Utility 
	XINPUT_STATE GetState();
	int GetIndex();
	bool Connected();

	// Controller vibration (0.0f = cancel, 1.0f = max)
	void Rumble(float leftMotor = 0.0f, float rightMotor = 0.0f);

private:
	XINPUT_STATE controllerState; // Controller state
	int controllerIndex; // Index of the controller (up to 4)

	static const int btnCount = 14;// Total buttons
	bool prev_BtnStates[btnCount]; // Previous frame button states
	bool curr_BtnStates[btnCount]; // Current frame button states
						
	bool curr_BtnsDown[btnCount]; // Buttons still held down on current frame
};

#endif

// XButtonIDs can now be used as XBtns externally
extern XButtonIDs XBtns;