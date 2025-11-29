
#### ***Update 1 : 28/11/25***
Done a new project as a Test to ensure that sfml sockets were indeed functional, what I would need to use them and testing that there was something I could understand and use here.

The above was achieved and was able to use TCP sockets, directly connected to another instance of the project running on 2 different devices at home and on the same Desktop Computer in College. This was a stellar first step.

Following this I have began slotting a NetworkManager class into my full game so that there is a hub area, we arent just working with main() anymore so we have to sure we can structure this soundly. Deadlines are no reason to get sloppy.

===============================================================

#### ***Update 2 : 28/11/25***
Got some basic integration into the game, there is 1 flaw i need to work out, the game does not launch the user into a lobby until another party has actually engaged and hit join/host, so it looks like the game is froze. I will have to amend that, it even tricked me and Im working on it.

===============================================================

#### ***Update 3 : 29/11/25***
Fixed the Lobby screen updating and allowing users to join it before the other party connect. 
Got data sending and recieving, both players see the same board and added a simple turn system that only works during networking to ensure players cannot place each others pieces. 
Added IP influence, users are asked to get the ip of the partner and then input that to join each other. Very trusting bond that would be :). 