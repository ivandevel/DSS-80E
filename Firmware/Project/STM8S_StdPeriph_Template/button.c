#include "stm8s_eval.h"
#include "button.h"

/* Module defines */
#define BUTTON_PRESS_THRESHOLD_MS	(uint16_t)(20)
#define BUTTON_RELEASE_THRESHOLD_MS	(uint16_t)(20)
#define BUTTON_HOLD_THRESHOLD_MS	(uint16_t)(2000)
#define BUTTON_INDICATION_ENABLED 0

/* Module variables */
static eButtonEvent_t eButtonEvent[BUTTONn] = {eButtonEventNone, eButtonEventNone, eButtonEventNone};
static uint16_t usButtonPressTicks[BUTTONn] = {0,0,0};
static uint16_t usButtonReleaseTicks[BUTTONn] = {0,0,0};

volatile uint8_t bufEnc = 0;

void ENC_PollEncoder(void) {
	static uint8_t last_a = 1;
	uint8_t now_a;
	uint8_t now_b;
	
	//опрашиваем состояние линий энкодера
	now_a = (STM_EVAL_PBGetState(BUTTON_UP));
	now_b = (STM_EVAL_PBGetState(BUTTON_DOWN));

	//если на линии А произошло изменение состояния...
	if (now_a != last_a) {
		
		last_a = now_a;
		
		if (last_a == 0) {
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 0) {
				bufEnc = LEFT_SPIN;
			} else {
				bufEnc = RIGHT_SPIN;
			}
#else
			if (now_b == 0) {
				bufEnc = RIGHT_SPIN;
			} else {
				bufEnc = LEFT_SPIN;
			}
#endif
		} else {                  
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 0) {
				bufEnc = RIGHT_SPIN;
			} else {
				bufEnc = LEFT_SPIN;
			}
#else
			if (now_b == 0) {
				bufEnc = LEFT_SPIN;
			} else {
				bufEnc = RIGHT_SPIN;
			}
#endif                             
                }
	}
}

uint8_t ENC_GetStateEncoder(void) {
uint8_t tmp = bufEnc;
	if (tmp) {
		bufEnc = 0;
		return tmp;
	}
	return 0;
}


/*---------------------------- Private functions -----------------------------*/
void ResetCounters(Button_TypeDef button) {
	usButtonPressTicks[button] = 0;
	usButtonReleaseTicks[button] = 0;
}


/*---------------------------- Public functions ------------------------------*/
eButtonEvent_t eButtonGetEvent(Button_TypeDef button) {
eButtonEvent_t currentevent = (eButtonEvent[button]);
	vButtonPostEvent(button, eButtonEventNone);
	return(currentevent);
}


void vButtonPostEvent(Button_TypeDef button, eButtonEvent_t event) {
	assert_param(IS_BUTTON_EVENT(event));
	eButtonEvent[button] = event;
}


void vButtonHandler(Button_TypeDef button) {
	#if BUTTON_INDICATION_ENABLED
	switch (eButtonEvent[Button_LEFT]) {
	case eButtonEventNone:
		ssegWriteStr(" ", 1, SEG7);
		break;
	case eButtonEventPress:
		ssegWriteStr("P", 1, SEG7);
		break;
	case eButtonEventHold:
		ssegWriteStr("H", 1, SEG7);
		break;
	}

	switch (eButtonEvent[Button_RIGHT]) {
	case eButtonEventNone:
		ssegWriteStr(" ", 1, SEG9);
		break;
	case eButtonEventPress:
		ssegWriteStr("P", 1, SEG9);
		break;
	case eButtonEventHold:
		ssegWriteStr("H", 1, SEG9);
		break;
	}
	#endif

	if (STM_EVAL_PBGetState(button)) {
		/* Button isn't pressed or has been released */
		if (usButtonPressTicks[button]) {
			/* Button has been pressed previously */
			usButtonReleaseTicks[button]++;

			if (usButtonReleaseTicks[button] >= BUTTON_RELEASE_THRESHOLD_MS) {
				/* Button has been just released */
				/* Checking how long the button has been pressed */
				if ((usButtonPressTicks[button] >= BUTTON_PRESS_THRESHOLD_MS) &&
					 (usButtonPressTicks[button] < BUTTON_HOLD_THRESHOLD_MS)) {
					/* Short press */
					/* Send a 'button press' event */
					/* It will be stored until the main routine gets it */
					vButtonPostEvent(button, eButtonEventPress);
				} else {
					/* Button was pressed for too short time */
				}
				ResetCounters(button);
			}
		}
	} else {

		/* Button is pressed */
		/* Increase counter */
		usButtonPressTicks[button]++;

		if (usButtonPressTicks[button] >= BUTTON_HOLD_THRESHOLD_MS) {
			/* Long press */
			/* Send a 'button hold' event */
			/* It will be stored until the main routine gets it */
			vButtonPostEvent(button, eButtonEventHold);
		}
		/* Debouncing */
		if (usButtonReleaseTicks[button])
			usButtonReleaseTicks[button]--;
	}
}

