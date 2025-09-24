1. NPC must now contain a movement Behaviour attached to them, this will guide them.

		1.1. One Wander Behaviour; Will ***patrol around in a range, must not patrol behind itself*** as we want to ensure the movement is not snappy and is smooth. -COMPLETED AND FUNCTIONAL-
		1.2  One Seek Behaviour; Will **seek *out the player and follow them*** around until reaching them. -COMPLETED AND FUNCTIONAL-
	
		1.3 Two Arrive Behaviours; Difference will be the Speed at which they move. In effect the Behaviour will make the ***NPC seek out the player, but slow down on approach,*** this ensures they wont blaze past the player. -COMPLETED AND FUNCTIONAL-
		1.4 One Pursue Behaviour; The NPC will ***predict where the player will be and aim to intercept the player*** as they move. -COMPLETED AND FUNCTIONAL-
		
2. Multiple NPCs must be Made - Used a NPCManager ***-COMPLETED AND FUNCTIONAL-***
3. NPCs will display Text next to them explaining what their behaviour pattern is. 
4. NPCs will have a vision cone system, we must also display that or atleast a debug option for this feature.
5. NPCs can be toggled via numpad 1 - 5 and text for this should be displayed. ***-COMPLETED AND FUNCTIONAL-***