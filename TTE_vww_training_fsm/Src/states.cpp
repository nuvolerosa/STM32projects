/*
 * states.cpp
 *
 *  Created on: Oct 15, 2025
 *      Author: nicole
 */

#include "states.h"
#include "states_func.h"
#include "lcd.h"
#include "camera.h"

// Starting and ending states
int state_init_and_instruction(int t_mode, int label){
	drawBlackBackground(1, 481, 1, 273);
	mode_init(t_mode);
	mode_instruction(label);
	return 0;
}
int state_instruction(int label){
	drawBlackBackground(1, 481, 1, 273);
	mode_instruction(label);
	return 0;
}
int state_end_demo(int score, int inf_cycle){
	drawBlackBackground(270, 481, 1, 273);
	sprintf(showbuf, "Demo End");
	displaystring(showbuf, 273, 10);
	int accuracy = (int)((((float)score)/((float)inf_cycle))*100);
	sprintf(showbuf, "acc = %d%%", accuracy);
	displaystring(showbuf, 273, 40);
	return 0;
}

// Camera state
int state_camera_cycle(void){
	ReadCapture();
	StartCapture();
	DecodeandProcessAndRGB(RES_W, RES_H, input, RGBbuf, 1);
	FillAndConvertToRGB(input, RGBbuf, RES_H, RES_W);
	loadRGB565LCD(10, 10, RES_W, RES_W, RGBbuf, 2);
	drawBlackBackground(1, 481, 250, 273);
	return 0;
}
int state_camera_cycle_empty(void){
	ReadCapture();
	StartCapture();
	DecodeandProcessAndRGB(RES_W, RES_H, input, RGBbuf, 1);
	FillAndConvertToRGB(input, RGBbuf, RES_H, RES_W);
	return 0;
}


