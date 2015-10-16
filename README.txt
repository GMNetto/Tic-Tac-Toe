####################################################################################
# Tic-Tac-Toe								           #
#										   #
# Gustavo Marques Netto								   #
# Lucas Salla Pagnan								   #
#										   #
# The project consists of a Tic-Tac-Toe game played against the computer	   #
####################################################################################
Pins:

The inputs are 11 buttons: 9 positions on the board, a button to select who begins (computer or person) and the reset is already present in the board. The action fired by all buttons (except the reset) happens when the button is released. There are 10 outputs and they are all LEDs: 1 to indicate next player, and 9 to indicate the board.

The following is the table of used pins:

led_computer_turn - 0
Choose order switch - 1
Board switches - 2 through 10
Led rows - 11, 12 and 13
Led columns - A0 through A5 (used as digital functions)

LEDs:

In this project, a 3x3 RGB LED matrix was used, it was divided into three rows, each containing 3 LEDs sharing the Arduino pin to the common cathode. 
Each column also has 3 LEDs, which share the same anode with each other, each LED has three anodes, two of which are used for this program (one color for the computer and one for the player).
The anodes of the same color and the same column are connected together, totalling 6 ports for the Arduino, there is also a resistor at each of theses pins to limit the current.
The green light indicates the human player, and the blue light the computer.

General program structure:

The general structure tries to emulate a state machine. So the loop function always execute 2 tasks: a state task and a LED update task. 
The state function is the current state of the game. And each state is responsible to change the function stored in state if a transition happened. In this way the state machine that would model the game machine is clear in the code.

These states are: waiting order, person turn, computer turn and game over.

Waiting Order – Runs a timer and the person has this time to select if she wants to start. A red led will be lit while the person can choose to start by pushing the order button, it turns off with the timeout and then the computer starts.

Person turn – In this state a movement is read from the player if it is valid then the proper transition is execute on the other hand the state is kept.

Computer turn – In this state the next movement is searched and then the proper transition is executed. A red LED will be on for less than a second to indicate the computer has played.

The search for the new movement uses Min-Max with alpha-beta pruning. So the movement chosen is always valid and the computer will never loose. 

The update is never done directly in the pins, first they are written in a matrix and just in the task update_leds that the lights will reflect the game. 
The update_leds function works by implementing a counter that resets approximately every milisecond. Each time it resets it has the function of updating the next row of LEDs. The function has a small timer because it must be as fast as possible so the user can not perceive the blinking pattern.

Game over - if a game over by player loss is detected, the program will enter in game over state. In this state the LEDs of the current board will blink. The game can be restarted using the arduino reset button.