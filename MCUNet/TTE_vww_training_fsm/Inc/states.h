/*
 * states.h
 *
 *  Created on: Oct 15, 2025
 *      Author: nicole
 */

#ifndef STATES_H_
#define STATES_H_

int state_init_and_instruction(int t_mode, int label);
int state_instruction(int label);
int state_end_demo(int score, int inf_cycle);
int state_camera_cycle(void);
int state_camera_cycle_empty(void);

#endif /* STATES_H_ */
