# FARKLE

## Farkle Game Logic

1. Player Round Start
2. Check Dice for Farkle.
   1. If Farkle, Round over, track the farkle in score history
3. Player's turn:
   1. HOLD scoring dice
   2. ROLL dice if holding at least 1 scoring die
   3. Can only BANK if points > 0
   4. If succesful roll, goto 2.
   5. If Bank, add score to history, turn round over to next player

====

1. Computer Round Start
2. Check Dice for Farkle.
   1. If Farkle, Round over, track the farkle in score history
3. Computer's turn:
   1. If score is 0
      1. Decide which dice to hold and roll
   2. If score > 0, figure out how to decide hold/roll or bank
   3. If succesful roll, goto 2.
   4. If Bank, add score to history, turn round over to next player