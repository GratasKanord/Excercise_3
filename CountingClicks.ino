int maxCLicks = 2;
int currentCLicks = 0;
int tenClicksTimer = 0;

int timeLimit = 1000;
long currentTime = 0;

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


void setup() { 
  Serial.begin(115200);
	pinMode(button.PIN, INPUT_PULLUP);
	attachInterrupt(button.PIN, isr, FALLING);
} 
 
void loop() {
  currentCLicks = 0;
  currentTime = millis();
  while (millis() < (currentTime + timeLimit)){
    if (button.pressed) {
		  Serial.printf("Button pressed: %u times\n", button.numberKeyPresses);
		  button.pressed = false;
      
      currentCLicks++;
      if(currentCLicks > maxCLicks){
        detachInterrupt(button.PIN);
        Serial.printf("You exceeded the limit of clicks in 1 second! Max limit: %u times per second! Counter stopped!", maxCLicks);        
      }
      
      tenClicksTimer++;
      if(tenClicksTimer == 10){
        timeLimit = timeLimit - 100;
        tenClicksTimer = 0;
        Serial.printf("You made 10 clicks! Your new time limit is: %u milliseconds!", timeLimit);
      }  
	  }   
  }
}
