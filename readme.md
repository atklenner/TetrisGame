# Andrew Klenner's Tetris 2.0

### The project

  I chose tetris because making games seems like a fun thing to do and
I never really got around to actually doing it. There were other project
ideas that I had that would have been more technical but I decided not to
jump into the deep end too early and use Qt.

  Much of this project was following the tutorial [here](https://doc.qt.io/qt-5/qtwidgets-widgets-tetrix-example.html).
Though the first thing I did was finally get around to completing 
assignment 2 so I could get a hang of Qt. Before I could get 
started properly I had to reinitlize git because it kept 
adding additional files from past projects. Also from time
to time git will not commit anything to github because it claims my 
commit message is empty when it is not. I had to find the solution
on stackexchange and use git commit -am "message". 

  After solving my git problems I took to reading and completing the tutorial. The game
consists of a window class that handles the ui elements, a tetris
piece class that only knows what shape it is and where it is located
on the board, and a board class that knows what kind of shape is
located in each square. Both the ui and tetris piece classes were
easy to follow, understanding the game logic of the tetris board class
took some time to wrap my head around. Figuring this out did give me
some insight on how arrays can be used for things like games. Things
like the tetris board are not immediately seen as a one dimensional
array, but now that I see how it is done I could definitely apply it
to different types of games. 

  After reading the tutorial and typing  up the code, the game ran alomst immediately. 
This seemed way too easy, so I decided to modify the game that the tutorial 
provides with features I have seen in other tetris games. The main
modification I thought of was to use Qt Designer to set up the ui
instead of the GridLayout that they use in the tutorial. I also wanted
a hold piece function, this is very useful if you get a line piece too
early and would like to save it for later. I have made note of all the 
changes I made to the game to make it easier to see, instead of comparing
it to the version from the tutorial.

### The Modifications

  This is the list of modifications I made for the game, some of these
were fairly easy to implement, some not so much.

-Hold a piece

  The first mod I made was to include a hold piece function. I've seen this
in other tetris games and wanted it in mine. It lets you press the "h" key
and take the currently falling piece and put it away. You can then use this
held piece whenever you want, this is useful if you get a line piece early
and want to keep it for later. The actual implementation of this took some
thinking to do. Getting it to display on the ui was easy and is essentially
identical to the QLabel that shows the next piece. However getting the game
to store a piece or switch the current piece with one that had already been
stored took some thinking.

-Remove quit button

  Next I removed the quit button, this was obviously just deleting code, but
I didn't see a need for two exit buttons so I got rid of it.

-Pause shortcut

  Then I made the pause a little easier to access by making it a keyboard
shortcut as well as a button in the window. This took a bit of thinking
to implement. It was easy to make it pause when the game was playing but
un-pausing the game when it was paused was hard. Though looking back the 
solution is pretty straight foreward, all I did was add an if and switch
statement to the function that handled the keyboard input.

-Difficulty slider

  I wanted to add a difficulty slider to the game. This would increase the
speed of the game while also increasing the number of points you would
earn whenever you completed a line or dropped a piece early. This required
writing a new slot and signal to the window and gameboard respectively and
adding an int to multiply the score increases and decrease the amound of
time in between when the game updates itself.

-Directions

  I added directions to the game. Before you start the game the directions
are presented to the player and they disappear when you start the game. 
This was easy to implement, all I had to do was add text to the QFrame
that would only show before the game had started.

-Change the ui

  The next modification I made was to rewrite the ui part of the game and
use Qt Designer. This is easily the most difficult modification I made. 
Though the most difficult part was figuring out why I couldn't add a ui
object to my game class. Eventually I figured out that the ui form was
creating a class of a different name and so of course the solution was
incredibly simple, which was just change the name of the ui class to
Ui::Form. After that had been resolved I just recreated the ui using 
Qt Designer and deleted a bunch of code that was redundant. The last
thing I added to the ui was the GameBoard, which is the class that 
the actual game happens in. This required me to add a QFrame and then
promote it to a GameBoard widget, after that it worked perfectly.

-Message to the player

  The last thing I did was make it so that when you got a tetris, that is,
complete four lines at once, the game would congratulate you and say
TETRIS! This was simple and all I did was add a QLabel and a new signal
that would be triggered when you completed four lines.
