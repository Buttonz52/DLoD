//#include "XboxController.h"
//
////Try this for ZeroMemory in GetState()
//#include <Windows.h>
//
////Apparently this links the library
//#pragma comment(lib,"Xinput.lib")
//
//XButtonIDs XBtns;
//
//XButtonIDs::XButtonIDs()
//{
//	// Values correalate to the XINPUT_Buttons array
//	// #ButtonMapping
//	A = 0;
//	B = 1;
//	X = 2;
//	Y = 3;
//
//	DPad_Up = 4;
//	DPad_Down = 5;
//	DPad_Left = 6;
//	DPad_Right = 7;
//
//	L_Shoulder = 8;
//	R_Shoulder = 9;
//
//	L_Thumbstick = 10;
//	R_Thumbstick = 11;
//
//	StartBtn = 12;
//	BackBtn = 13;
//}
//
//XboxController::XboxController()
//{
//
//}
//
//XboxController::XboxController(int index)
//{
//	// minus 1 so that the controllers are numbered
//	// 1,2,3,4 instead of 0,1,2,3
//	controllerIndex = index - 1;
//
//	// Initialize all Xbox Controller buttons
//	for (int i = 0; i < btnCount; i++)
//	{
//		prev_BtnStates[i] = false;
//		curr_BtnStates[i] = false;
//		curr_BtnsDown[i] = false;
//	}
//}
//
//XboxController::~XboxController()
//{
//
//}
//
//XINPUT_STATE XboxController::GetState()
//{
//	XINPUT_STATE temp;
//	ZeroMemory(&temp, sizeof(XINPUT_STATE));
//
//	// gets the state of the controller @ controllerIndex
//	XInputGetState(controllerIndex, &temp);
//
//	return temp;
//}
//
//
//int XboxController::GetIndex()
//{
//	return controllerIndex;
//}
//
//bool XboxController::Connected()
//{
//	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
//	// get the state of the controller @ controllerIndex
//	DWORD result = XInputGetState(controllerIndex, &controllerState);
//
//	if (result == ERROR_SUCCESS) // controller is connected
//		return true;
//	else // not connected
//		return false;
//}
//
//void XboxController::Update()
//{
//	// gets current state of controller
//	controllerState = GetState();
//
//	// Iterate through all controller buttons
//	for (int i = 0; i < btnCount; i++)
//	{
//		// Set button state for current frame
//		curr_BtnStates[i] = (controllerState.Gamepad.wButtons & XINPUT_Btns[i]) == XINPUT_Btns[i];
//
//		// Set 'DOWN' state for current frame
//		curr_BtnsDown[i] = !prev_BtnStates[i] && curr_BtnStates[i];
//	}
//}
//
//void XboxController::RefreshState()
//{
//	memcpy(prev_BtnStates, curr_BtnStates, sizeof(prev_BtnStates));
//}
//
//bool XboxController::LStick_InDeadzone()
//{
//	short xAxis = controllerState.Gamepad.sThumbLX;
//	short yAxis = controllerState.Gamepad.sThumbLY;
//
//	// X axis is outside of deadzone
//	if (xAxis > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
//		xAxis < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
//		return false;
//
//	// Y axis is outside of deadzone
//	if (yAxis > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
//		yAxis < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
//		return false;
//
//	// One (or both axes) axis is inside of deadzone
//	return true;
//}
//
//bool XboxController::RStick_InDeadzone()
//{
//	//previously State
//	short xAxis = controllerState.Gamepad.sThumbRX;
//	short yAxis = controllerState.Gamepad.sThumbRY;
//
//	// X axis is outside of deadzone
//	if (xAxis > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
//		xAxis < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
//		return false;
//
//	// Y axis is outside of deadzone
//	if (yAxis > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
//		yAxis < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
//		return false;
//
//	// One (or both axes) axis is inside of deadzone
//	return true;
//}
//
//float XboxController::LeftStick_X()
//{
//	short sX = controllerState.Gamepad.sThumbLX;
//
//	// Return axis value, converted to a float
//	return (static_cast<float>(sX) / 32768.0f);
//}
//
//float XboxController::LeftStick_Y()
//{
//	short sY = controllerState.Gamepad.sThumbLY;
//
//	// Return axis value, converted to a float
//	return (static_cast<float>(sY) / 32768.0f);
//}
//
//float XboxController::RightStick_X()
//{
//	short sX = controllerState.Gamepad.sThumbRX;
//
//	// Return axis value, converted to a float
//	return (static_cast<float>(sX) / 32768.0f);
//}
//
//float XboxController::RightStick_Y()
//{
//	short sY = controllerState.Gamepad.sThumbRY;
//
//	// Return axis value, converted to a float
//	return (static_cast<float>(sY) / 32768.0f);
//}
//
//
///* Trigger values are in the range 0-255, divide by 255 to keep
//them in the 0.0f-1.0f range so that we can have the percentage
//pressed as well */
//float XboxController::LeftTrigger()
//{
//	BYTE trigger = controllerState.Gamepad.bLeftTrigger;
//
//	if (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
//		return trigger / 255.0f;
//
//	return 0.0f; // Trigger was not pressed
//}
//
//float XboxController::RightTrigger()
//{
//	BYTE trigger = controllerState.Gamepad.bRightTrigger;
//
//	if (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
//		return trigger / 255.0f;
//
//	return 0.0f; // Trigger was not pressed
//}
//
//void XboxController::Rumble(float leftMotor, float rightMotor)
//{
//	XINPUT_VIBRATION vibrationState;
//
//	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));
//
//	// Takes rumble amounts (0.0f-1.0f) and converts to 
//	// rumble range of 0-65535
//	int iLeftMotor = int(leftMotor * 65535.0f);
//	int iRightMotor = int(rightMotor * 65535.0f);
//
//	// Set vibration values
//	vibrationState.wLeftMotorSpeed = iLeftMotor;
//	vibrationState.wRightMotorSpeed = iRightMotor;
//
//	// Set the vibration state of controller @ controllerIndex
//	XInputSetState(controllerIndex, &vibrationState);
//}
//
//bool XboxController::GetButtonPressed(int button)
//{
//	// Compare bits to find equality 
//	if (controllerState.Gamepad.wButtons & XINPUT_Btns[button])
//	{
//		return true; // The button is pressed
//	}
//
//	return false; // Not pressed
//}