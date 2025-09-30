
Short and Simple(in theory) assignment;
Goals:

1. Create Swarming behaviour using the "Lenard Jones Potential Function/Equation".
	 1a. We should only be adjusting the formula based on its A, B, n and m variables to control intensity, rest should remain the same according to the formula.
	 1b. We have this behaviour and simply apply it to an SwarmerGroup of NPCs list - This will separate concerns while not discarding the effects both have.
	 
2. The swarm should have a scalable amount of entities that are swarming and remain grouped, i.e. 50 entities should swarm the same as 100 entities just over a larger area.

3.  Entities should pick a random direction to move in together.
	 3a. Should entities follow a core leader or a direction?
	 Asking as teleporting by bounds may move or displace the entities.
	 3b. Direction: Will disregard distance to core entity(leader) and may have adverse effects.    Follow Leader: Will need to ensure the swarm knows that teleporting will be quicker to reach the Leader again than running to the other end of the screen. 
	
4.  For performance we should inspect larger groupings versus smaller inner groupers of the swarm, which is less intensive to use while still retaining the effect of not disregarding one another.



Checklist:

	1. Create New NPC List that will be designed for swarmers: Will include their movement behaviour, their internal grouping movement and a direction to go, this will separate them from base NPCs - Just for clarity, kind of obvious..
	- Refactored & Completed -
	   
	2. Design a system to take in a Behaviour and an amount of entities to be manipulated or that should care about one another in a smaller group, SHOULD reduce Lenard Jones algorithm overhead.
	   - Refactored & Completed -
	   
	3. Make the Lenard Jones algorithm a behaviour, this will make applying it to an NPC more managable
	   - Refactored & Completed -
	   ----------------------------------------------------------------------------
	3Alt. Should the whole swarm be new and not derived from NPC due to different behaviour? Could we make Lenard Jones Algorithm applyable only to these dedicated NPCs? This would be rigid but would allow for more control and better performance in theory.
		- Scrapped -
	
	4. Make a struct for passing movement and other concerns to the steering calculation for Lenard Jones - We may be able to trim off a lot of the performance heavy calculations by mini groups and only comparing to groups or select entities in nearby groups rather than the whole list of Entities which will be Log(n^2)(Must compare to every single entity for each entity.)
	   - Refactored & Completed -


**Review Stage**

Due to part 3 changing - Could not make it a behaviour, part 4 had to be redesigned and modified. What was done? We made a new class solely for Lennard Jones Potential as working with it I discovered several things, the foremost being this is not per entity. There is per entity calculation but this is designed not as a steering behaviour but a Bundle. Cannot apply struct movement behaviours as we done before.

Above was the big change hence it coming first;

But part 1 had to be reviewed, we could use NPCs but not our old ones, made a struct for a Swarmer type NPC, made a list of those.
Part 2 was achieved as we cut down some of the Lennard Jones overhead but not by grouping, we just checked around for neighbours inside of a cutoff range, this effectively done the same thing by ignoring anything further and not bothering doing a full calculation.