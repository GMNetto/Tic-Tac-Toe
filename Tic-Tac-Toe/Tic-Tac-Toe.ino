
#include <math.h>
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
namespace tic {
typedef struct {
  int x, y;
} Pos;

Pos create_position(int x, int y) {
  Pos pos;
  pos.x = x;
  pos.y = y;
  return pos;
}

int board[][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int allow_button = 1; //can just press button with no button pressed

long time_waiting = 0; //the time passed until this point.

long waiting_order_time = 20000; //Time to player select if he wants to start playing.

void waiting_order();

void (*state)() = waiting_order;

void turn_on_led(int ledPin, int state) {
  digitalWrite(ledPin, state);
}

/**
 * Decouncing routine, It only works if the button was kept pressed for more than debounceDelay, if it changes the state of a variable 
 * no other button can be pressed while the button is still pressed.
 */
void debounce_button(const int buttonPin, long *lastDebounceTime, long debounceDelay, int *buttonState, int *lastButtonState, void(*action)()) {
  int reading = digitalRead(buttonPin);
  if (allow_button) {
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


void debounce(const int buttonPin, long *lastDebounceTime, long debounceDelay, int *buttonState, int *ledState, int *lastButtonState, int ledPin, void(*action)(int, int)) {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  //Serial.println(*buttonState);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (allow_button) {
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

void game_over() {
  //TODO
}

typedef struct {
  Pos pos;
  int weight;
} Pos_weight;

typedef struct {
  Pos *pos;
  int size_vector;
} Pos_vector_zsize;

Pos_weight create_Pos_weight(int x, int y, int weight) {
  Pos_weight pos;
  pos.pos = create_position(x, y);
  pos.weight = weight;
  return pos;
}

/**
 * Creates the AI next possible movements to use in the min max.
 */
Pos_vector_zsize get_next_position(int board[][3]) {
  int i, j, counter = 0;
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) {
      if (!board[i][j]) {
        counter++;
      }
    }
  }
  Pos_vector_zsize vec;
  vec.pos = (Pos*)malloc(sizeof(Pos) * counter);
  counter = 0;
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) {
      if (!board[i][j]) {
        vec.pos[0] = create_position(i, j);
        counter++;
      }
    }
  }
  vec.size_vector = counter;
  return vec;
}

Pos read_player_movement() {
  //TODO
}

void write_player_movement(Pos posit) {
  //TODO
}


/**
 * Check row and colun at the same time to save processing time
 */
bool check_row_column() {
  int i, j;
  for (i = 0; i < 3; ++i) {
    int sum_row = 0;
    int sum_column = 0;
    for (j = 0; j < 3; ++j) {
      sum_row += board[i][j];
      sum_column += board[j][i];
    }
    if (sum_row == 3) return true;
    if (sum_column == 3) return true;
  }
  return false;
}

bool check_diagonal() {
  int i, j;
  int sum_main = 0;
  int sum_opo = 0;
  for (i = 0; i < 3; ++i) {
    sum_main += board[i][i];
    sum_opo += board[2 - i][i];
  }
  if (sum_main == 3) return true;
  if (sum_opo == 3) return true;
  return false;
}

bool check_game_over() {
  return check_row_column() || check_diagonal();
}

/**
 * Min-max with alpha beta prunning for the AI
 */
int min_alpha_beta(int board[][3], int alpha, int beta);

int max_alpha_beta(int board[][3], int alpha, int beta) {
  int best_value = alpha;
  Pos_vector_zsize next_movements = get_next_position(board);
  int i;
  for (i = 0; i < next_movements.size_vector; ++i) {
    int copy[3][3];
    memcpy(copy,board, 9*sizeof(int));     
    copy[next_movements.pos[i].x][next_movements.pos[i].y] = 1;
    int value_from_child = min_alpha_beta(copy, best_value, beta);
    best_value = max(value_from_child, best_value);
    if (best_value >= beta) {
      break;
    }
  }
  free(next_movements.pos);
  return best_value;
}

int min_alpha_beta(int board[][3], int alpha, int beta) {
  int best_value = beta;
  Pos_vector_zsize next_movements = get_next_position(board);
  int i;
  for (i = 0; i < next_movements.size_vector; ++i) {
    int copy[3][3];
    memcpy(copy,board, 9*sizeof(int));     
    copy[next_movements.pos[i].x][next_movements.pos[i].y] = 1;
    int value_from_child = max_alpha_beta(copy, alpha, best_value);
    best_value = min(value_from_child, best_value);
    if (best_value <= alpha) {
      break;
    }
  }
  free(next_movements.pos);
  return best_value;
}

Pos search_next_position(){
  Pos next_position = {-1, -1};
  int alpha = -INFINITY, beta =  INFINITY;
  Pos_vector_zsize next_movements = get_next_position(board);
  int i, best_value = INFINITY;
  for (i = 0; i < next_movements.size_vector; ++i) {
    int copy[3][3];
    memcpy(copy,board, 9*sizeof(int));     
    copy[next_movements.pos[i].x][next_movements.pos[i].y] = 1;
    int value_from_child = max_alpha_beta(copy, alpha, best_value);
    if(value_from_child < best_value){
      value_from_child = best_value;
      next_position = {next_movements.pos[i].x, next_movements.pos[i].y};
    }
    if (best_value <= alpha) {
      break;
    }
  }
  free(next_movements.pos);
  return next_position;
}

////////////////////////////////////////////////////////////////////////////////////

void person_turn();

/**
 * We assumed that the AI will always generate a valid movement, so we just have to write it.
 */
void computer_turn() {
  Pos next_movement = search_next_position();
  write_player_movement(next_movement);
  if (check_game_over()) {
    state = game_over;
  } else {
    state = person_turn;
  }
}

bool check_pos(Pos posit) {
  return board[posit.x][posit.y] == 0;
}

/**
 * State representing person turn,if the movement read is not valid the state is not changed.
 * So, the game will still be wanting for a valid movement.
 */
void person_turn() {
  Pos pos = read_player_movement();
  bool valid_pos = check_pos(pos);
  if (valid_pos) {
    write_player_movement(pos);
    if (check_game_over()) {
      state = game_over;
    } else {
      state = computer_turn;
    }
  } else {
    state = person_turn;
  }
}

void selected_order() {
  state = person_turn;
}

/**
 * Wait until the user selects if he wants to be start the game.
 */
void waiting_order() {
  debounce_button(choose_order_pin, &lastDebounceTime, debounceDelay, &order_button_state, &last_order_state, selected_order);
  if ((millis() - time_waiting) > waiting_order_time) {
    state = computer_turn;
  }
}

/**
 * Only allow a button to be pressed if no button is pressed
 */
void check_buttons() {
  int reading_1 = digitalRead(buttonPin_1), reading_2 = digitalRead(buttonPin_2);
  allow_button = !(reading_1 && reading_2);
}

}


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

/**
 * Responsible to turn on the leds of the board 3x3 with 1.
 */
void update_leds(){
  //TODO
}

void loop() {
  tic::check_buttons();
  tic::state();
  update_leds();
  //debounce(buttonPin_1, &lastDebounceTime, debounceDelay, &buttonState_1, &ledState_1, &lastButtonState_1, ledPin_1, turn_on_led);
  //debounce(buttonPin_2, &lastDebounceTime, debounceDelay, &buttonState_2, &ledState_2, &lastButtonState_2, ledPin_2, turn_on_led);

}



