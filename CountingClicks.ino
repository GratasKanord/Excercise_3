int maxCLicks = 2;
int currentCLicks = 0;
int tenClicksTimer = 0;
long timeLimit = 1000000; //1 second in microsec
bool disableTimer = false;
 
hw_timer_t *My_timer = NULL;


struct Button {
	const uint8_t PIN;
	uint32_t numberKeyPresses;
	bool pressed;
};

Button button = {35, 0, false};

unsigned long button_time = 0;  
unsigned long last_button_time = 0;

void IRAM_ATTR isr() {
  button_time = millis();
  if (button_time - last_button_time > 50)
  {
    button.numberKeyPresses++;
    button.pressed = true;
    last_button_time = button_time;
  }
}

void IRAM_ATTR onTimer(){
  if(currentCLicks > maxCLicks){
    detachInterrupt(button.PIN);
    Serial.printf("You exceeded the limit of clicks in 1 second! Max limit: %u times per second! Counter stopped!", maxCLicks); 
    disableTimer = true;       
  }
  else{
    currentCLicks = 0;
  }
}

void setup() { 
  Serial.begin(115200);
	pinMode(button.PIN, INPUT_PULLUP);
	attachInterrupt(button.PIN, isr, FALLING);
  
  // Creating timer
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);  
  timerAlarmWrite(My_timer, timeLimit, true);
  timerAlarmEnable(My_timer);
} 
 
void loop() {
  if(!disableTimer){
    if (button.pressed) {
		  Serial.printf("Button pressed: %u times\n", button.numberKeyPresses);
		  button.pressed = false;
      currentCLicks++;
      tenClicksTimer++;
      
      if(tenClicksTimer == 10){
        timeLimit -= 100000;
        timerAlarmWrite(My_timer, timeLimit, true);
        tenClicksTimer = 0;
        Serial.printf("You made 10 clicks! Your new time limit decreased on 100 milliseconds!");
      }  
	  }
  }
  else{
    timerAlarmDisable(My_timer);
  }

}
