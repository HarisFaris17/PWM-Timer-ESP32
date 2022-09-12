#include<Arduino.h>
#define LED 14
// the return of timerBegin() is hw_timer_t
// My_timer used for passing timer information to ther timerAttachInterrupt,timerAlaramWrite and timerAlarmEnable, hence when the timer reached the desired value, the callback function will be evoked
hw_timer_t *My_timer = NULL;

// this variable used in onTimer to indentify if the onTimer called to change the alarmValue to 6000000 or to 1000000
boolean change = true;

void IRAM_ATTR onTimer(){
  digitalWrite(LED, !digitalRead(LED));
  // whis used to detach interrupt functionallity to timer that specified by My_timer, in this case timer0.
  timerDetachInterrupt(My_timer);
  // change will specify if the evoked onTimer should make the alarmValue to 6000000 or 1000000
  if(change){
    // the timer0 specified in variable My_timer will be attached to interrupt, hence can call an alarm
    timerAttachInterrupt(My_timer, &onTimer, true);
    timerAlarmWrite(My_timer, 6000000, true);
    timerAlarmEnable(My_timer); //Just Enable
    change=false;
  }else{
    timerAttachInterrupt(My_timer, &onTimer, true);
    timerAlarmWrite(My_timer, 1000000, true);
    timerAlarmEnable(My_timer); //Just Enable
    change=true;
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  // turn on the timer0
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 1000000, true);
  timerAlarmEnable(My_timer); //Just Enable
}
void loop() {
  delay(10000);
  Serial.println("test");
}
