


#define RE_PK_MASK ((1 << PK5) | (1 << PK4) | (1 << PK3))
// rotary encoder debounce upper threshold (ms)
#define RE_DEBOUNCE_THRESH 30
// encoder will enter sleep after 5s of no input
#define ENCODER_TIMEOUT 5000
#define ENCODER_EVENT_BUF_LEN 100

enum encoder_event {
  ENCODER_CCW,
  ENCODER_CW,
  ENCODER_SW
};


enum encoder_event encoder_next_event(void);
void encoder_init(void);
void encoder_wake(void);
void encoder_sleep(void);
