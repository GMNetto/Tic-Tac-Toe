
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
const int buttonPin_0 = 2;    // the number of the pushbutton pin_1
const int buttonPin_1 = 3;    // the number of the pushbutton pin_2
const int buttonPin_2 = 4;    // the number of the pushbutton pin_1
const int buttonPin_3 = 5;    // the number of the pushbutton pin_2
const int buttonPin_4 = 6;    // the number of the pushbutton pin_1
const int buttonPin_5 = 7;    // the number of the pushbutton pin_2
const int buttonPin_6 = 8;    // the number of the pushbutton pin_1
const int buttonPin_7 = 9;    // the number of the pushbutton pin_2
const int buttonPin_8 = 10;    // the number of the pushbutton pin_1

const int choose_order_pin = 11; //The number of the choose order button

const int ledRow_0 = 11;
const int ledRow_1 = 12;
const int ledRow_2 = 13;
const int ledColumn_Player_0 = A0;
const int ledColumn_Computer_0 = A1;
const int ledColumn_Player_1 = A2;
const int ledColumn_Computer_1 = A3;
const int ledColumn_Player_2 = A4;
const int ledColumn_Computer_2 = A5;

//const int ledColumn_Player_0 = 14;
//const int ledColumn_Computer_0 = 15;
//const int ledColumn_Player_1 = 16;
//const int ledColumn_Computer_1 = 17;
//const int ledColumn_Player_2 = 18;
//const int ledColumn_Computer_2 = 19;

uint8_t led0, led1, led2; //used for the led loop

//const int ledPin_1 = 13;      // the number of the LED pin_1
//const int ledPin_2 = 12;      // the number of the LED pin_2
//const int choose_order_pin = 11; //The number of the choose order button

// Variables will change:
int buttonState_0 = HIGH;
int buttonState_1 = HIGH;    
int buttonState_2 = HIGH;    
int buttonState_3 = HIGH;    
int buttonState_4 = HIGH;    
int buttonState_5 = HIGH;    
int buttonState_6 = HIGH;
int buttonState_7 = HIGH;
int buttonState_8 = HIGH;  

                      
int lastButtonState_0 = LOW; 
int lastButtonState_1 = LOW; 
int lastButtonState_2 = LOW; 
int lastButtonState_3 = LOW; 
int lastButtonState_4 = LOW; 
int lastButtonState_5 = LOW; 
int lastButtonState_6 = LOW; 
int lastButtonState_7 = LOW; 
int lastButtonState_8 = LOW; 

int order_button_state = HIGH;
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

int const buttonPin_to_board[][2] = {{0,0}, {0, 1}, {0,2}, {1, 0},{1,1}, {1, 2}, {2,0}, {2, 1}, {2,2}, {3, 0}};

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime_0 = 0;
long lastDebounceTime_1 = 0;
long lastDebounceTime_2 = 0;
long lastDebounceTime_3 = 0;
long lastDebounceTime_4 = 0;
long lastDebounceTime_5 = 0;
long lastDebounceTime_6 = 0;
long lastDebounceTime_7 = 0;
long lastDebounceTime_8 = 0;

long lastDebounceTime = 0;


long debounceDelay = 50;    // the debounce time; increase if the output flickers

int allow_button = 1; //can just press button with no button pressed

long time_waiting = 0; //the time passed until this point.

long waiting_order_time = 5000; //Time to player select if he wants to start playing.

void waiting_order();

void (*state)() = waiting_order;

void turn_on_led(int ledPin, int state) {
  digitalWrite(ledPin, state);
}

int check_buttons() {
  int reading_0 = digitalRead(buttonPin_0), reading_1 = digitalRead(buttonPin_1),
  reading_2 = digitalRead(buttonPin_2), reading_3 = digitalRead(buttonPin_3),
  reading_4 = digitalRead(buttonPin_4), reading_5 = digitalRead(buttonPin_5),
  reading_6 = digitalRead(buttonPin_6), reading_7 = digitalRead(buttonPin_7),
  reading_8 = digitalRead(buttonPin_8);
  int buttons =  (reading_0 + reading_1 + reading_2 + reading_3 + reading_4 + reading_5 + reading_6 + reading_7 + reading_8) ;
  Serial.println(buttons);
  return buttons == 8;
}


/**
 * Decouncing routine, It only works if the button was kept pressed for more than debounceDelay, if it changes the state of a variable 
 * no other button can be pressed while the button is still pressed.
 */
Pos debounce_button(const int buttonPin, long *lastDebounceTime, long debounceDelay, int *buttonState, int *lastButtonState) {
  int reading = digitalRead(buttonPin);
  if (reading != *lastButtonState) {
    *lastDebounceTime = millis();
  }

  if ((millis() - *lastDebounceTime) > debounceDelay) {
    if (reading != *buttonState) {
      *buttonState = reading;
      // only toggle the LED if the new button state is HIGH
      if (*buttonState == HIGH) {
        allow_button = false;
        int real_pos[2] = {buttonPin_to_board[buttonPin - 2][0], buttonPin_to_board[buttonPin - 2][1]};
        return create_position(real_pos[0], real_pos[1]);
      }
    }
  }
  *lastButtonState = reading;
  return create_position(-1, -1);
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

void update_leds();
void update_leds(int board[][3]);

void print_next_movements(Pos_vector_zsize vec){
  int i;
  for(i = 0; i < vec.size_vector; ++i){
    Serial.print(vec.pos[i].x);
    Serial.print(" ");
    Serial.print(vec.pos[i].y);
  }
  Serial.println();
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
        vec.pos[counter] = create_position(i, j);
        counter++;
      }
    }
  }
  vec.size_vector = counter;
  return vec;
}


Pos read_player_movement() {
  Pos pos = debounce_button(buttonPin_0, &lastDebounceTime_0, debounceDelay, &buttonState_0, &lastButtonState_0);
  if(pos.x > -1)return pos;
  pos = debounce_button(buttonPin_1, &lastDebounceTime_1, debounceDelay, &buttonState_1, &lastButtonState_1);
  if(pos.x > -1)return pos;
  pos = debounce_button(buttonPin_2, &lastDebounceTime_2, debounceDelay, &buttonState_2, &lastButtonState_2);
  if(pos.x > -1)return pos;
  pos = debounce_button(buttonPin_3, &lastDebounceTime_3, debounceDelay, &buttonState_3, &lastButtonState_3);
  if(pos.x > -1)return pos;
  pos = debounce_button(buttonPin_4, &lastDebounceTime_4, debounceDelay, &buttonState_4, &lastButtonState_4);
  if(pos.x > -1)return pos;
  pos = debounce_button(buttonPin_5, &lastDebounceTime_5, debounceDelay, &buttonState_5, &lastButtonState_5);
  if(pos.x > -1)return pos;
  pos = debounce_button(buttonPin_6, &lastDebounceTime_6, debounceDelay, &buttonState_6, &lastButtonState_6);
  if(pos.x > -1)return pos;
  pos = debounce_button(buttonPin_7, &lastDebounceTime_7, debounceDelay, &buttonState_7, &lastButtonState_7);
  if(pos.x > -1)return pos;
  pos = debounce_button(buttonPin_8, &lastDebounceTime_8, debounceDelay, &buttonState_8, &lastButtonState_8);
  return pos;
}

void write_player_movement(Pos posit, int player_id) {
  //Stub
  board[posit.x][posit.y] = player_id;
}


/**
 * Check row and colun at the same time to save processing time
 */
int check_row_column(int board[][3]) {
  int i, j;
  for (i = 0; i < 3; ++i) {
    int sum_row_1 = 0;
    int sum_row_2 = 0;
    int sum_column_1 = 0;
    int sum_column_2 = 0;
    for (j = 0; j < 3; ++j) {
      sum_row_1 += (board[i][j] == 1);
      sum_column_1 += (board[j][i] == 1);
      sum_row_2 += (board[i][j] == 2);
      sum_column_2 += (board[j][i] == 2);
    }
    if (sum_row_1 == 3) return 1;
    if (sum_column_1 == 3) return 1;
    if (sum_row_2 == 3) return 3;
    if (sum_column_2 == 3) return 3;
  }
  return 0;
}

int check_diagonal(int board[][3]) {
  int i, j;
  int sum_main_1 = 0;
  int sum_opo_1 = 0;
  int sum_main_2 = 0;
  int sum_opo_2 = 0;
  for (i = 0; i < 3; ++i) {
    sum_main_1 += (board[i][i] == 1);
    sum_opo_1 += (board[2 - i][i] == 1);
    sum_main_2 += (board[i][i] == 2);
    sum_opo_2 += (board[2 - i][i] == 2);
  }
  if (sum_main_1 == 3) return 1;
  if (sum_opo_1 == 3) return 1;
  if (sum_main_2 == 3) return 3;
  if (sum_opo_2 == 3) return 3;
  return 0;
}

int check_game_over(int board[][3]) {
  int result = check_row_column(board) + check_diagonal(board);
  if(result == 0) return 0;
  if(result%3 == 0) return -1;
  return 1;
}

/**
 * Min-max with alpha beta prunning for the AI
 */
int min_alpha_beta(int board[][3], int alpha, int beta);

int max_alpha_beta(int board[][3], int alpha, int beta) {
  int best_value = alpha;
  int answer = check_game_over(board);
  if(answer != 0){
    return answer;
  }
  Pos_vector_zsize next_movements = get_next_position(board);
  int i;
  for (i = 0; i < next_movements.size_vector; ++i) {
    int copy[3][3];
    memcpy(copy,board, 9*sizeof(int));     
    copy[next_movements.pos[i].x][next_movements.pos[i].y] = 1;
    int value_from_child = min_alpha_beta(copy, best_value, beta);
    //update_leds(copy); 
    //Serial.println(value_from_child);
    best_value = max(value_from_child, best_value);
    if (best_value >= beta) {
      break;
    }
  }
  free(next_movements.pos);
  if(next_movements.size_vector == 0) {
    int answer = check_game_over(board);
    return answer;
  }
  return best_value;
}

int min_alpha_beta(int board[][3], int alpha, int beta) {
  int best_value = beta;
  int answer = check_game_over(board);
  if(answer != 0){
    return answer;
  }
  Pos_vector_zsize next_movements = get_next_position(board);
  int i;
  for (i = 0; i < next_movements.size_vector; ++i) {
    int copy[3][3];
    memcpy(copy,board, 9*sizeof(int));     
    copy[next_movements.pos[i].x][next_movements.pos[i].y] = 2;
    int value_from_child = max_alpha_beta(copy, alpha, best_value);
    //update_leds(copy); 
    //Serial.println(value_from_child);
    best_value = min(value_from_child, best_value);
    if (best_value <= alpha) {
      break;
    }
  }
  free(next_movements.pos);
  if(next_movements.size_vector == 0) {
    int answer = check_game_over(board);
    return answer;
  }
  return best_value;
}

Pos search_next_position(){
  Pos next_position = {-1, -1};
  int alpha = -INFINITY, beta =  INFINITY;
  //int board[][3] = {{0, 0, 0}, {0, 0, 1}, {2, 2, 1}};
  Pos_vector_zsize next_movements = get_next_position(board);
  print_next_movements(next_movements);
  int i, best_value = alpha;
  for (i = 0; i < next_movements.size_vector; ++i) {
    int copy[3][3];
    memcpy(copy,board, 9*sizeof(int));     
    copy[next_movements.pos[i].x][next_movements.pos[i].y] = 1;
    int value_from_child = min_alpha_beta(copy, best_value, beta);
    //update_leds(copy);  
    //Serial.print(next_movements.pos[i].x);
    //Serial.print(" ");
    //Serial.print(next_movements.pos[i].y);
    //Serial.print(" ");
    //Serial.println(value_from_child);
    if(value_from_child > best_value){
      best_value = value_from_child;
      next_position = {next_movements.pos[i].x, next_movements.pos[i].y};
    }
    if (best_value >= beta) {
      break;
    }
  }
  free(next_movements.pos);
  return next_position;
}

////////////////////////////////////////////////////////////////////////////////////

/**
 * Only allow a button to be pressed if no button is pressed
 */

void person_turn();

void do_nothing(){};

/**
 * We assumed that the AI will always generate a valid movement, so we just have to write it.
 */
void computer_turn() {
  Serial.println("Searching");
  //update_leds();
  Pos next_movement = search_next_position();

  write_player_movement(next_movement, 1);
  update_leds();
  Serial.println(next_movement.x);
  Serial.println(next_movement.y);
  delay(10000);
  if (check_game_over(board)) {
    state = game_over;
  } else {
    state = person_turn;
    //state = do_nothing;
  }
}

bool check_pos(Pos posit) {
  return posit.x != -1 && board[posit.x][posit.y] == 0;
}

/**
 * State representing person turn,if the movement read is not valid the state is not changed.
 * So, the game will still be wanting for a valid movement.
 */
void person_turn() {
  Pos pos = read_player_movement();
  bool valid_pos = check_pos(pos);
  Serial.println(pos.x);
  if (valid_pos) {
    write_player_movement(pos, 2);
    if (check_game_over(board)) {
      state = game_over;
    } else {
      delay(10000);
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
  Pos pos = debounce_button(choose_order_pin, &lastDebounceTime, debounceDelay, &order_button_state, &last_order_state);
  if(pos.x == 3 && pos.y == 0){
    state = person_turn;
  }
  if ((millis() - time_waiting) > waiting_order_time) {
    Serial.println("Computer turn.");
    state = computer_turn;
  }
}



/**
 * Responsible to turn on the leds of the board 3x3 with 1.
 */
void update_leds(){
  //TODO
  int i, j;
  for(i = 0; i < 3; ++i){
    for(j = 0; j < 3; ++j){
      Serial.print("\t");
      Serial.print(board[i][j]);
    }
    Serial.println();
  }
  Serial.println();
  
for(i=0;i<3;i++){ //goes by column
  if(i==0){
    digitalWrite(ledRow_0, HIGH);
    digitalWrite(ledRow_1, LOW);
    digitalWrite(ledRow_2, LOW);
  }else if(i==1){
    digitalWrite(ledRow_0, LOW);
    digitalWrite(ledRow_1, HIGH);
    digitalWrite(ledRow_2, LOW);    
  }else{
    digitalWrite(ledRow_0, LOW);
    digitalWrite(ledRow_1, LOW);
    digitalWrite(ledRow_2, HIGH);
  }
  led0 = board[i][0];
  led1 = board[i][1];
  led2 = board[i][2];
  if(led0==1){
    Serial.print("COMPUTER LED");
    digitalWrite(ledColumn_Player_0, HIGH); //off
    digitalWrite(ledColumn_Computer_0, LOW); //on
    //delay(5000);
  }else if(led0==2){
    digitalWrite(ledColumn_Player_0, LOW); //on
    digitalWrite(ledColumn_Computer_0, HIGH); //off
  }else{
    digitalWrite(ledColumn_Player_0, HIGH); //off
    digitalWrite(ledColumn_Computer_0, HIGH); //off
  }
  
  if(led1==1){
    digitalWrite(ledColumn_Player_1, HIGH); //off
    digitalWrite(ledColumn_Computer_1, LOW); //on
  }else if(led1==2){
    digitalWrite(ledColumn_Player_1, LOW); //on
    digitalWrite(ledColumn_Computer_1, HIGH); //off
  }else{
    digitalWrite(ledColumn_Player_1, HIGH); //off
    digitalWrite(ledColumn_Computer_1, HIGH); //off
  }
  
  if(led2==1){
    digitalWrite(ledColumn_Player_2, HIGH); //off
    digitalWrite(ledColumn_Computer_2, LOW); //on
  }else if(led2==2){
    digitalWrite(ledColumn_Player_2, LOW); //on
    digitalWrite(ledColumn_Computer_2, HIGH); //off
  }else{
    digitalWrite(ledColumn_Player_2, HIGH); //off
    digitalWrite(ledColumn_Computer_2, HIGH); //off
  }
  delay(10);
  
}
}



}


void setup() {
  pinMode(buttonPin_0, INPUT);
  pinMode(buttonPin_1, INPUT);
  pinMode(buttonPin_2, INPUT);
  pinMode(buttonPin_3, INPUT);
  pinMode(buttonPin_4, INPUT);
  pinMode(buttonPin_5, INPUT);
  pinMode(buttonPin_6, INPUT);
  pinMode(buttonPin_7, INPUT);
  pinMode(buttonPin_8, INPUT);
  
  pinMode(ledRow_0, OUTPUT);
  pinMode(ledRow_1, OUTPUT);
  pinMode(ledRow_2, OUTPUT);

  pinMode(ledColumn_Player_0, OUTPUT);
  pinMode(ledColumn_Computer_0, OUTPUT);
  pinMode(ledColumn_Player_1, OUTPUT);
  pinMode(ledColumn_Computer_1, OUTPUT);
  pinMode(ledColumn_Player_2, OUTPUT);
  pinMode(ledColumn_Computer_2, OUTPUT);
  

  Serial.begin(9600);
  tic::time_waiting = millis();
}


void loop() {
  tic::check_buttons();
  tic::state();
  tic::update_leds();
  //debounce(buttonPin_1, &lastDebounceTime, debounceDelay, &buttonState_1, &ledState_1, &lastButtonState_1, ledPin_1, turn_on_led);
  //debounce(buttonPin_2, &lastDebounceTime, debounceDelay, &buttonState_2, &ledState_2, &lastButtonState_2, ledPin_2, turn_on_led);

}



