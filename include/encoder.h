/**
 * @file encoder.h
 * @author ztnel (christian911@sympatico.ca)
 * @brief 
 * @version 0.1
 * @date 2022-10
 * 
 * @copyright Copyright © 2022 Christian Sargusingh
 * 
 */

#ifndef __ENCODER_H__
#define __ENCODER_H__

#define ENCODER_EVENT_BUF_LEN 25 

enum encoder_event {
  ENCODER_NULL,
  ENCODER_CW,
  ENCODER_CCW,
  ENCODER_SW,
};

void encoder_init(void);
void encoder_clear_events(void);
enum encoder_event encoder_next_event(void);

#endif // __ENCODER_H__
