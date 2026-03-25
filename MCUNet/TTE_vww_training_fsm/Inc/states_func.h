/*
 * states.h
 *
 *  Created on: Oct 14, 2025
 *      Author: nicole
 */

#ifndef STATES_FUNC_H_
#define STATES_FUNC_H_

#define OUTPUT_CH 2
#define RES_W 128
#define RES_H 120
#define RES_CH 3

// Functions and variables for NN utilisation
extern signed char out_int[OUTPUT_CH];
extern float labels[10];

void train(int cls);
void invoke_new_weights_givenimg(signed char *out_int8);

// Function and variables for modes
extern char showbuf[150];

void mode_init(int t_mode);
void mode_instruction(int label);
void training_mode(int label, int cycle);
int inference_mode(int label);

//
typedef signed char int8_t;
typedef unsigned short uint16_t;

extern int8_t *input;
extern uint16_t *RGBbuf;

void FillAndConvertToRGB(int8_t *Buf_ptr, uint16_t *RGBbuf_ptr, int H, int W);

#endif /* STATES_FUNC_H_ */
