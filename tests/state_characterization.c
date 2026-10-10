/* Host characterization only: no MMIO or board execution. */
#include <assert.h>
#include <stdio.h>
#include "state.h"
volatile int Start_Flag, Cruise_Flag, Accel_Flag, Brake_Flag, Gear_Flag;
volatile CarState_t current_state;
volatile uint32_t adcResult;
volatile int input_lock, mode, current_speed, gear, distance;
static int brake_calls, cruise_calls;
void Velocity_init(void) { current_speed = 0; }
void Handle_init(void) {}
void DRV_Brake_Control(void) { brake_calls++; }
void DRV_Control(void) {}
void DRV_Coasting_Control(void) {}
void Handle_Steering(void) {}
void Led_blink(void) {}
void Keep_Constant_Speed(void) { cruise_calls++; }
int main(void) {
 current_state=STATE_OFF; Start_Flag=1; Process_State_Transition();
 assert(current_state==STATE_OFF); /* Known defect, not desired behavior. */
 current_state=STATE_P; Gear_Flag=1; current_speed=0; Process_State_Transition();
 assert(current_state==STATE_D);
 Gear_Flag=1; current_speed=1; Process_State_Transition();
 assert(current_state==STATE_D && Gear_Flag==1); /* Latched request. */
 Gear_Flag=0; current_state=STATE_D; Cruise_Flag=1; Process_State_Transition();
 assert(current_state==STATE_CRUISE);
 Brake_Flag=1; Process_State_Action();
 assert(cruise_calls==1 && brake_calls==0); /* Known missing brake cancellation. */
 Start_Flag=0; Process_State_Transition(); assert(current_state==STATE_OFF);
 puts("6 characterization checks passed; known defects reproduced, NOT fixed");
 return 0;
}
