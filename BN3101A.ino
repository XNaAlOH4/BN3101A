#include <Servo.h>
#include <HX711.h>

const int LOAD1_DOUT_PIN = 2;
const int LOAD1_SCK_PIN = 3;
const int LOAD2_DOUT_PIN = 4;
const int LOAD2_SCK_PIN = 5;
const int LOAD3_DOUT_PIN = 6;
const int LOAD3_SCK_PIN = 7;

const int SERVO1 = 9;
const int SERVO2 = 10;

const float cal_weight = 0.520f;
const float MIN_BAG_THRESHOLD = 0.1f;
const float ABNORMAL_FLOW_THRESHOLD = 1.f;

Servo clamp[2];
HX711 scale[2], out;
char active = 0;

struct warning_flag {
	uint32_t bag1_done : 1,
           bag1_half_done : 1,
           bag2_half_done : 1,
           bag2_done : 1,
           abnormal_flow : 1,
           unused : 27;
} flag;

// Setup interrupt for Timer 2
void interruptSetup2() {  // 500Hz Timer might be abit overkill but we can't go much lower due to Timer 1 being used to control servo
  TCCR2A = 0x02;          // Disable PWM and use CTC mode
  TCCR2B = 0x05;          // Prescaler clkIO/128
  #if F_CPU == 16000000L  // if using 16MHz crystal
    OCR2A = 0xF9;         // Set count to 249 for 2ms int
  #elif F_CPU == 8000000L // if using 8MHz crystal
    OCR2A = 0x7C;         // Set count to 124 for 2ms int
  #endif
  TIMSK2 = 0x02;          // Enable Output Compare A Match int
  sei();
}

ISR(TIMER2_COMPA_vect) {
  cli();
  float w = scale[active].get_units(10);
  float wout = out.get_units(10);

  // Since 2ms is the interval we can easily calculate flow
  float v = w/0.002f;
  float vout = wout/0.002f;

#ifdef ABNORMAL_FLOW_TEST
  if( (v+ABNORMAL_FLOW_THRESHOLD) > vout) {
    // Sound alarm
    flag.abnormal_flow = 1;
  }
#endif

  // Swap bag if current weight is below threshold
  if(w < MIN_BAG_THRESHOLD) {
    if(0 == active)
      flag.bag1_done = 1;
    else
      flag.bag2_done = 1;
    active ^= 1;
    // TODO: perform servo clamping
  }

  sei();
}

void setup() {
  clamp[0].attach(SERVO1);
  clamp[0].write(175);
  delay(15);
  clamp[0].write(175);
  delay(15);

  //clamp[1].attach(SERVO2);

  Serial.begin(9600);

  scale[0].begin(LOAD1_DOUT_PIN, LOAD1_SCK_PIN);
  scale[0].set_scale();

  float cal1 = scale[0].get_units(10);
  scale[0].set_scale(cal1/cal_weight);
  scale[0].tare();

  float w1 = scale[0].get_units(10);
  //clamp[0].write(135);
  //delay(15);
}

void loop() {
  float w = scale[active].get_units(10);

  if(w < -MIN_BAG_THRESHOLD) {
    clamp[0].write(125);
  }else {
    clamp[0].write(175);
  }
  //float wout = out.get_units(10);

  // Since 2ms is the interval we can easily calculate flow
  //float v = w/0.002f;
  //float vout = wout/0.002f;

  Serial.print("Weight = ");
  Serial.print(w);
  Serial.print(", ");
  Serial.print(-MIN_BAG_THRESHOLD);
  Serial.print(", ");
  Serial.println(w <-MIN_BAG_THRESHOLD);
}
