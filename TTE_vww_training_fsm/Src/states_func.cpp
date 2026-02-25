/*
 * states.cpp
 *
 *  Created on: Oct 14, 2025
 *      Author: nicole
 */

#include "states_func.h"

#include "stdio.h"
#include "string.h"
#include "stm32746g_discovery.h"
#include "lcd.h"
extern "C" {
#include "genNN.h"
#include "tinyengine_function.h"
}


char showbuf[150];

// Function definition
signed char out_int[OUTPUT_CH]; // probs to get 0 or 1
float labels[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

// pointer to data
int8_t *input 		     = (int8_t *)getInput();
uint16_t *RGBbuf			 = (uint16_t *)&input[RES_W * RES_H * RES_CH];

void train(int cls) {
  char cbuf[20];
  for (int i = 0; i < 10; i++) {
    if (i == cls) {
      labels[i] = 1.0f;
    } else
      labels[i] = 0.0f;
  }
  invoke(labels);
}

void invoke_new_weights_givenimg(signed char *out_int8) {
  invoke_inf();
  signed char *output = (signed char *)getOutput();
  for (int i = 0; i < 10; i++)
    out_int8[i] = output[i];
}

void mode_init(int t_mode){
	drawGreenBackground(90, 390, 10, 120);
	if (t_mode == 0){
		sprintf(showbuf, " Inference ");
	}else if(t_mode == 1){
		sprintf(showbuf, " Training ");
	}
	displaystring(showbuf, 100, 20);
	sprintf(showbuf, " starts ");
	displaystring(showbuf, 100, 45);
	sprintf(showbuf, " in few seconds ");
	displaystring(showbuf, 100, 70);
	HAL_Delay(3000);
}

void mode_instruction(int label){

	drawBlueBackground(90, 390, 150, 235);
	  if (label == 1){
		  sprintf(showbuf, " Stay in front ");
		  displaystring(showbuf, 100, 160);
		  sprintf(showbuf, " of the camera! ");
		  displaystring(showbuf, 100, 185);
	  } else if( label == 0){
		  sprintf(showbuf, " Exit from the ");
		  displaystring(showbuf, 100, 160);
		  sprintf(showbuf, " camera view! ");
		  displaystring(showbuf, 100, 185);
	  }
	  HAL_Delay(5000);
	  drawBlackBackground(1, 481, 1, 273);
}

void training_mode(int label, int cycle){
	  // Write message to screen
	  sprintf(showbuf, "Training in");
	  displaystring(showbuf, 273, 10);
	  sprintf(showbuf, "progress");
	  displaystring(showbuf, 273, 35);
	  sprintf(showbuf, "T-cycle: %d", cycle);
	  displaystring(showbuf, 273, 100);
	  // train model on label
	  int start = HAL_GetTick();
	  train(label);
	  int end = HAL_GetTick();
}

int inference_mode(int label){

	  int answer_right, prediction;

	  int start = HAL_GetTick();
	  invoke_new_weights_givenimg(out_int);
	  if(out_int[0] >= out_int[1]){
		  prediction = 0;
		  if(label == 0){
			  answer_right = 1;
		  }else if(label == 1){
			  answer_right = 0;
		  }
	  }else if(out_int[0] < out_int[1]){
		  prediction = 1;
		  if(label == 0){
			  answer_right = 0;
		  }else if(label == 1){
			  answer_right = 1;
		  }
	  }
	  int end = HAL_GetTick();
	  sprintf(showbuf, " Inference ");
	  displaystring(showbuf, 273, 10);
	  detectResponse(prediction, end - start, 0, answer_right, label);

	  return answer_right;
}

void FillAndConvertToRGB(int8_t *Buf_ptr, uint16_t *RGBbuf_ptr, int H, int W){

	for (int i = 0; i < W * 8 * 3; i++) {
	        	Buf_ptr[H * W * 3 + i] = -128;
	        }
	        for (int i = 0; i < W; i++) {
	          for (int j = 0; j < W; j++) {
	            uint8_t red = (int32_t)Buf_ptr[(128 * i + j) * 3] + 128;
	            uint8_t green = (int32_t)Buf_ptr[(128 * i + j) * 3 + 1] + 128;
	            uint8_t blue = (int32_t)Buf_ptr[(128 * i + j) * 3 + 2] + 128;

	            uint16_t b = (blue >> 3) & 0x1f;
	            uint16_t g = ((green >> 2) & 0x3f) << 5;
	            uint16_t r = ((red >> 3) & 0x1f) << 11;

	            RGBbuf_ptr[j + W * i] = (uint16_t)(r | g | b);
	          }
	        }
}
