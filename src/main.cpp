#include<Arduino.h>
#define LED 14
hw_timer_t *My_timer = NULL;
boolean change = true;
void IRAM_ATTR onTimer(){
  digitalWrite(LED, !digitalRead(LED));
  timerDetachInterrupt(My_timer);
  if(change){
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
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 1000000, true);
  timerAlarmEnable(My_timer); //Just Enable
}
void loop() {
  delay(10000);
  Serial.println("test");
}
