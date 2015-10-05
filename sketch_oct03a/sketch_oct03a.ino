/*
 Debounce

 Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
 press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
 a minimum delay between toggles to debounce the circuit (i.e. to ignore
 noise).

 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached from pin 2 to +5V
 * 10K resistor attached from pin 2 to ground

 * Note: On most Arduino boards, there is already an LED on the board
 connected to pin 13, so you don't need any extra components for this example.


 created 21 November 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Limor Fried
 modified 28 Dec 2012
 by Mike Walters

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Debounce
 */

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin_1 = 2;    // the number of the pushbutton pin_1
const int buttonPin_2 = 3;    // the number of the pushbutton pin_2
const int ledPin_1 = 13;      // the number of the LED pin_1
const int ledPin_2 = 12;      // the number of the LED pin_2
const int choose_order_pin = 4; //The number of the choose order button 

// Variables will change:
int ledState_1 = LOW;         // the current state of the output pin
int buttonState_1;             // the current reading from the input pin
int lastButtonState_1 = LOW;   // the previous reading from the input pin

int ledState_2 = LOW;         // the current state of the output pin
int buttonState_2;             // the current reading from the input pin
int lastButtonState_2 = LOW;   // the previous reading from the input pin

int order_button_state;
int last_order_state = LOW;

int order = 0; //indicates if the order was chosen

typedef struct position{
  int x, y;
}Pos;


// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int allow_button = 1; //can just press button with no button pressed

long time_waiting = 0; //the time passed until this point.

long waiting_order_time = 20000; //Time to player select if he wants to start playing.

void waiting_order();

void (*state)() = waiting_order; 

void setup() {
  pinMode(buttonPin_1, INPUT);
  pinMode(ledPin_1, OUTPUT);

  // set initial LED_1 state
  digitalWrite(ledPin_1, ledState_1);

  pinMode(buttonPin_2, INPUT);
  pinMode(ledPin_2, OUTPUT);

  // set initial LED_2 state
  digitalWrite(ledPin_2, ledState_2);
  Serial.begin(9600);   
  
}

void turn_on_led(int ledPin,int state){
  digitalWrite(ledPin, state);
}

void debounce_button(const int buttonPin, long *lastDebounceTime, long debounceDelay, int *buttonState, int *lastButtonState, void(*action)()){
  int reading = digitalRead(buttonPin);
  if (allow_button){
    if (reading != *lastButtonState) {
      *lastDebounceTime = millis();
    }
  
    if ((millis() - *lastDebounceTime) > debounceDelay) {
      if (reading != *buttonState && allow_button) {
        *buttonState = reading;
        // only toggle the LED if the new button state is HIGH
        if (*buttonState == HIGH) {
          allow_button = false;
          action();
        }
      }
    }
    *lastButtonState = reading;
  }
}


void debounce(const int buttonPin, long *lastDebounceTime, long debounceDelay, int *buttonState, int *ledState, int *lastButtonState, int ledPin, void(*action)(int, int)){
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  //Serial.println(*buttonState);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (allow_button){
    if (reading != *lastButtonState) {
      // reset the debouncing timer
      *lastDebounceTime = millis();
    }
  
    if ((millis() - *lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:
      
      // if the button state has changed:
      if (reading != *buttonState && allow_button) {
        *buttonState = reading;
        // only toggle the LED if the new button state is HIGH
        if (*buttonState == HIGH) {
          allow_button = false;
          *ledState = !(*ledState);
        }
      }
    }
  

    // set the LED:
    action(ledPin, *ledState);
  
    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    *lastButtonState = reading;
  }
}

void computer_turn(){
  
}

void person_turn(){
  
}

void selected_order(){
    state = person_turn;
}

/**
 * Wait until the user selects if he wants to be start the game.
 */
void waiting_order(){
  debounce_button(choose_order_pin, &lastDebounceTime, debounceDelay, &order_button_state, &last_order_state, selected_order);
  if((millis() - time_waiting) > waiting_order_time){
    state = computer_turn;
  }
}

/**
 * Only allow a button to be pressed if no button is pressed
 */
void check_buttons(){
  int reading_1 = digitalRead(buttonPin_1), reading_2 = digitalRead(buttonPin_2);
  allow_button = !(reading_1 && reading_2);
}

void loop() {
  check_buttons();
  state();
  //debounce(buttonPin_1, &lastDebounceTime, debounceDelay, &buttonState_1, &ledState_1, &lastButtonState_1, ledPin_1, turn_on_led);
  //debounce(buttonPin_2, &lastDebounceTime, debounceDelay, &buttonState_2, &ledState_2, &lastButtonState_2, ledPin_2, turn_on_led);
  
}



