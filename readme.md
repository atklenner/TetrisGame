# Andrew Klenner's Tetris 2.0

### The project

I chose tetris because making games seems like a fun thing to do and
I never really got around to actually doing it. There were other project
ideas that I had that would have been more technical but with my other
obligations (Math 110B and 120A) I decided to take a simpler route and
use Qt.

Much of this project was following the tutorial [here](https://doc.qt.io/qt-5/qtwidgets-widgets-tetrix-example.html).
Though the first thing I did was finally get around to completing 
assignment 2 so I could get a hang of Qt. Before I could get 
started properly I had to reinitlize git because it kept 
adding additional files from past projects. Also from time
to time git will not commit anything to github because it claims my 
commit message is empty when it is not. I had to find the solution
on stackexchange and use git commit -am "message".
(This is not finished, I have to say how following the tutorial went)

(I want to explain the process of getting the game running/reading the tutorial (and how easy/hard this was),
the the motivation for all my modifications, and how those work/how difficult were they to implement)

### The Modifications

I decided that the game I ended up with following the tutorial isn't as
fully featured as I could make it so I modified it a bit, some of these
were fairly easy to implement, some not so much.

The first mod I made was to include a hold piece function. I've seen this
in other tetris games and wanted it in mine. It lets you press the "h" key
and take the currently falling piece and put it away. You can then use this
held piece whenever you want, this is useful if you get a line piece early
and want to keep it for later. The actual implementation of this was really
easy and worked almost identically to the show next piece function.

Next I removed the quit button, this was obviously just deleting code, but
I didn't see a need for two exit buttons so I got rid of it.

Then I made the pause a little easier to access by making it a keyboard
shortcut as well as a button in the window. This took a bit of thinking
to implement. It was easy to make it pause when the game was playing but
un-pausing the game when it was paused was hard. Though looking back the 
solution is pretty straight foreward, all I did was add an if and switch
statement to the function that handled the keyboard input.

I wanted to add a difficulty slider to the game. This would increase the
speed of the game while also increasing the number of points you would
earn whenever you completed a line or dropped a piece early. This required
writing a new slot and signal to the window and gameboard respectively and
adding an int to multiply the score increases and decrease the amound of
time in between when the game updates itself.

(changing colors of the window and other things)

I added directions to the game. Before you start the game the directions
are presented to the player and they disappear when you start the game. 
This was easy to implement, all I had to do was add text to the QFrame
that would only show before the game had started.

(redo ui elements, i.e. use Qt designer)
This is easily the most difficult modification I made. 

(tetris special effect, when you get a tetris it looks cool)