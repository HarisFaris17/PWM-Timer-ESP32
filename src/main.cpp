#include<Arduino.h>
#define PWM_FREQ 490
#define MICRO_SEC 1000000
// the return of timerBegin() is hw_timer_t
// myTimer used for passing timer information to ther timerAttachInterrupt,timerAlaramWrite and timerAlarmEnable, hence when the timer reached the desired value, the callback function will be evoked
hw_timer_t *myTimer = NULL;

// this variable used in onTimer to indentify if the onTimer called to change the alarmValue to 6000000 or to 1000000
bool state = true;

int timer = 0;
int pwmPin = 14;

int alarmVal = 0;

const int PWMFreq = 490;
const int microSec = 1000000;

void IRAM_ATTR changeStatePWM();
void configTimer();
void valuePWM(int,bool=false);
void endTimer();

// void IRAM_ATTR onTimer(){
//   digitalWrite(LED, !digitalRead(LED));
//   // whis used to detach interrupt functionallity to timer that specified by myTimer, in this case timer0.
//   timerDetachInterrupt(myTimer);
//   // change will specify if the evoked onTimer should make the alarmValue to 6000000 or 1000000
//   if(state){
//     // the timer0 specified in variable myTimer will be attached to interrupt, hence can call an alarm
//     timerAttachInterrupt(myTimer, &onTimer, true);
//     timerAlarmWrite(myTimer, 500000, true);
//     timerAlarmEnable(myTimer); //Just Enable
//     change=false;
//   }else{
//     timerAttachInterrupt(myTimer, &onTimer, true);
//     timerAlarmWrite(myTimer, 500000, true);
//     timerAlarmEnable(myTimer); //Just Enable
//     state=true;
//   }
// }
void setup() {
  Serial.begin(115200);
  pinMode(pwmPin, OUTPUT);
  // turn on the timer0
  // myTimer = timerBegin(timer, 80, true);
  // timerAttachInterrupt(myTimer, &onTimer, true);
  // timerAlarmWrite(myTimer, 1000000, true);
  // timerAlarmEnable(myTimer); //Just Enable
  configTimer();
  valuePWM(255,false);
}
void loop() {
  delay(10000);
  Serial.println("test");
}

void configTimer(){
  myTimer = timerBegin(timer,80,true);
  Serial.println("Timer configured");
}

void valuePWM(int pwmVal,bool percent){
  timerAttachInterrupt(myTimer,&changeStatePWM,true);
  if(percent){
    if(pwmVal<0||pwmVal>100) {
      Serial.println("The value for PWM should be beetween 0 and 100");
      Serial.println("As a result, the PWM value will be 50%");
      alarmVal = int(0.5*microSec/PWMFreq);
    }
    else{
      alarmVal = int((pwmVal/100.0)*microSec/PWMFreq);
      Serial.println("Alarm percent : "+alarmVal);
    }
  }
  else{
    if(pwmVal<0||pwmVal>255) {
      Serial.println("The value for PWM should be beetween 0 and 255");
      Serial.println("As a result, the PWM value will be 50%");
      alarmVal = int(0.5*microSec/PWMFreq);
    } 
    else{
      alarmVal = int((pwmVal/255.0)*microSec/PWMFreq);
      Serial.println("Alarm value : "+alarmVal);
    }
  }
  Serial.println(alarmVal);
  digitalWrite(pwmPin,HIGH);
  state=true;
  timerAlarmWrite(myTimer,alarmVal,true);
  timerAlarmEnable(myTimer);
  Serial.println("PWM configured");
}

void IRAM_ATTR changeStatePWM(){
  // Serial.println(digitalRead(pwmPin));
  timerDetachInterrupt(myTimer);
  timerAttachInterrupt(myTimer,&changeStatePWM,true);
  if(state){
    timerAlarmWrite(myTimer,alarmVal,true);
    digitalWrite(pwmPin,HIGH);
    state=false;
  }
  else{
    timerAlarmWrite(myTimer,microSec/PWMFreq-alarmVal,true);
    digitalWrite(pwmPin,LOW);
    state=true;
  }
  timerAlarmEnable(myTimer);
}

void endTimer(){
  timerDetachInterrupt(myTimer);
  timerAlarmDisable(myTimer);
  timerEnd(myTimer);
}