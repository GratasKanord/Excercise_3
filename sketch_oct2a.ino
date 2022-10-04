const int led1=23; 
const int led2=22; 
const int led3=21; 
const int led4=4;
long currentTime = 0;
 
unsigned long button_time = 0;  
unsigned long last_button_time = 0;

struct Button {
	const uint8_t PIN;
	bool pressed;
};

Button buttonRun = {35, false};
Button buttonStop = {34, false};

hw_timer_t *My_timer1 = NULL;
hw_timer_t *My_timer2 = NULL;
hw_timer_t *My_timer3 = NULL;
hw_timer_t *My_timer4 = NULL;

void IRAM_ATTR onTimer1(){
  digitalWrite(led4, !digitalRead(led4));  
}

void IRAM_ATTR onTimer2(){
  digitalWrite(led3, !digitalRead(led3));
}

void IRAM_ATTR onTimer3(){
  digitalWrite(led2, !digitalRead(led2));
}

void IRAM_ATTR onTimer4(){  
  digitalWrite(led1, !digitalRead(led1));
}

void IRAM_ATTR isrRun() {
  button_time = millis();
  if (button_time - last_button_time > 50)
  {
    buttonRun.pressed = true;
    last_button_time = button_time;
  }
    
      timerAlarmEnable(My_timer4); 
      timerAlarmEnable(My_timer3);
      timerAlarmEnable(My_timer2);
      timerAlarmEnable(My_timer1);

}

void IRAM_ATTR isrStop() {
  button_time = millis();
  if (button_time - last_button_time > 50)
  {
    buttonStop.pressed = true;
    last_button_time = button_time;
  }
  timerAlarmDisable(My_timer1);
  timerAlarmDisable(My_timer2);
  timerAlarmDisable(My_timer3);
  timerAlarmDisable(My_timer4);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  
}

void setup() {
  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT); 
  pinMode(led3, OUTPUT); 
  pinMode(led4, OUTPUT);
  pinMode(buttonRun.PIN, INPUT_PULLUP);
  pinMode(buttonStop.PIN, INPUT_PULLUP);

  My_timer1 = timerBegin(0, 80, true);
  My_timer2 = timerBegin(1, 80, true);
  My_timer3 = timerBegin(2, 80, true);
  My_timer4 = timerBegin(3, 80, true);

  timerAttachInterrupt(My_timer4, &onTimer4, true);
  timerAttachInterrupt(My_timer3, &onTimer3, true);
  timerAttachInterrupt(My_timer2, &onTimer2, true);
  timerAttachInterrupt(My_timer1, &onTimer1, true);

  timerAlarmWrite(My_timer4, 1000000, true);
  timerAlarmWrite(My_timer3, 1010000, true);
  timerAlarmWrite(My_timer2, 1015000, true);
  timerAlarmWrite(My_timer1, 1020000, true);

  attachInterrupt(buttonRun.PIN, isrRun, FALLING);  
  attachInterrupt(buttonStop.PIN, isrStop, FALLING); 
}

void loop() {


}