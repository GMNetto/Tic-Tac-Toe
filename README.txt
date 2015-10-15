# Tic-Tac-Toe

The project consists of a Tic-Tac-Toe that a player compete against a computer.

Pins:

The inputs are 11 buttons and consume the same number of pins: 9 positions on the board, a button to select who begins (computer or person) and the reset is already present in the board. The action fired by all buttons (except the reset) happen when the button is released. There are 10 outputs and they are all LED’s: 1 to indicate next player, and 9 to indicate the board.

General program structure:

The general structure tries to emulate a state machine. So the loop function always execute 2 tasks: a state task and a LED update task. 
The state function is the current state of the game. And each state is responsible to change the function stored in state if a transition happened. In this way the state machine that would model the game machine is clear in the code.

These states are: waiting order, person turn, computer turn and game over.

Waiting Order – Runs a timer and the person has this time to select if she wants to start. 

Person turn – In this state a movement is read from the player if it is valid then the proper transition is execute on the other hand the state is kept.

Computer turn – In this state the next movement is searched and then the proper transition is executed.

The search for the new movement uses Min-Max with alpha-beta pruning. So the movement chosen is always valid and the computer will never loose. 

The update is never done directly in the pins, first they are written in a matrix and just in the task update_leds that the lights will reflect the game. 

//TODO – write about game over state and how the LED’s are updated
 
