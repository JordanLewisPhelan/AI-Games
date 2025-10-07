

This is a shorthand addition to the existing project.

Goals;

1. Add in Formation construction and movement.  - That is it but this must be togglable.
	1. Togglability based on input key chosen.
	2. Attach these NPCs movement to a formation - Behaviour? so they have a movement vector. Using the player as a Leader will suffice.
	3. NPCs should follow their formation slots and anchor point(player), but, ONLY WHEN REQUESTED TO. This will mean that movement should be a behaviour and applied as a force to align with the Slot position the NPC is tracking and following.
	4. Gradual changes as the player turns.
	   
Additions to help;
1. Supplemental classes, e.g. Transform or Slot, e.g. Transform could be the point the 'Slot' is at for a given NPC, they follow that and its direction, reasonably reappliable.
2. Structs to hold Followers - Will determine NPC tracking a point, i.e. the Transform and the behaviour it should do.
   e.g. 
   NPC = Entity, 
   Transform = Position-Direction and movement speed we are moving by - this is for updating Position by movement speed,
   Behaviour = Maybe unnecessary but would allow trace-ability & ability to mediate to the Transform position in the most efficient way should the player take off(likely overkill for this project even if awesome).


