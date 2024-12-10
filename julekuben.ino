#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define led_pin 7
#define btn1_pin 8
#define btn2_pin 9

#define bounce_delay 50

#define blink_time 50

int btn1_state = LOW;
int btn2_state = LOW;

int last_btn1_state = LOW;
int last_btn2_state = LOW;

unsigned long last_btn1_debounce_time = 0;
unsigned long last_btn2_debounce_time = 0;

unsigned long led_time_on = 0;
int led_on = 0;

unsigned int count = 0;
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void printCount() {
  char buffer[16]; 
  sprintf(buffer, "Tryck: %u", count);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(buffer);
}
 
void led_turn_off(){
  if (led_on == 1){
    digitalWrite(led_pin, LOW);
    led_on = 0;
  }
}

void led_turn_on(){
  if(led_on == 0){
    digitalWrite(led_pin, HIGH);
    led_on = 1;
    led_time_on = millis();
  }
}

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(btn1_pin, INPUT_PULLUP);
  pinMode(btn2_pin, INPUT_PULLUP);
  lcd.begin(16, 2);  //Defining 16 columns and 2 rows of lcd display
  lcd.backlight();   //To Power ON the back light
  printCount();
}



void loop() {
  int reading1 = digitalRead(btn1_pin);
  int reading2 = digitalRead(btn2_pin);

  if (reading1 != last_btn1_state) {
    last_btn1_debounce_time = millis();
  }

  if (reading2 != last_btn2_state) {
    last_btn2_debounce_time = millis();
  }

  if ((millis() - last_btn1_debounce_time) > bounce_delay) {
    if (reading1 != btn1_state) {
      btn1_state = reading1;

      if (btn1_state == HIGH) {
        led_turn_on();
        count++;
        printCount();
        
      }
    }
  }

  if ((millis() - last_btn2_debounce_time) > bounce_delay) {
    if (reading2 != btn2_state) {
      btn2_state = reading2;

      if (btn2_state == HIGH) {
        led_turn_on();
        count++;
        printCount();
      } 
    }
  }

  if((millis() - led_time_on) > blink_time){
    led_turn_off();
  }

  last_btn1_state = reading1;
  last_btn2_state = reading2;
}