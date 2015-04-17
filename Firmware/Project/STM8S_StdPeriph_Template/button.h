#ifndef __BUTTON_H__
#define __BUTTON_H__

//#define TM_RE_CHANGE_ROTATION

#define RIGHT_SPIN 0x01
#define LEFT_SPIN 0xff

typedef enum {
	eButtonEventNone = 0,
	eButtonEventPress,
	eButtonEventHold
} eButtonEvent_t;

#define IS_BUTTON_EVENT(EVENT)	(((EVENT) == eButtonEventNone) || \
								 ((EVENT) == eButtonEventPress) || \
								 ((EVENT) == eButtonEventHold))

eButtonEvent_t eButtonGetEvent(Button_TypeDef button);
void vButtonPostEvent(Button_TypeDef button, eButtonEvent_t event);
void vButtonHandler(Button_TypeDef button);
uint8_t ENC_GetStateEncoder(void);
void ENC_PollEncoder(void);


#endif //__BUTTON_H__
