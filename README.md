# UltimateTicTacToe

https://en.wikipedia.org/wiki/Ultimate_tic-tac-toe

My variant is a bit different, as you can still play in a local board that has been won.

- When you start the game you are player X and the computer is player O.
- the region you can play in is indicated by the given letter and number coordinates
- if you win a local board, the computer prints this
- sometimes if you have nowhere u can move, the completelyStuck function activates and moves you

## AI
- my AI simply does a recursive search down a tree of board combinations, and evaluates how many 3 in a rows of both local and global boards there are, and also 2 in a rows if there are no 3 in a rows.
- the AI is okay, but could easily be improved with a different AI algorithm, such as monte carlo tree search

## To Improve
- more tests
- simplify the code further
- apply a proper GUI

## -- HOW TO USE THE PROGRAM --
- typing (./UltimateTicTacToe n) where 0<n<10 will create a instance of the game with a diffculty proportional to the depth of the tree search.
- typing (./UltimateTicTacToe) on its own will start the testing function, where the computer plays against the computer!!! Very Cool!!!! 
- PS don't go above 4 for the difficulty/depth, as it takes too long to process!!
