Controls:
Adding controls here to avoid having to draw it in SFML when it is easier to have this here.

C : Will toggle the Cost Value for the tiles.
V : Will toggle the Vector(Direction) field for the tiles.

Left Mouse : Will Set the tile you click on to the Start Position.
Right Mouse : Will set the tile you click on to the End/Goal position.
Middle Mouse : Will toggle a tile to be an obstacle and entity and flowfield will path around it.

Extra Content:

Added an entity that will traverse the grid laid out automatically and gradually using the flowfield. Because of how flowfield works we only ever recalculate the flow when the End goal changes. The only thing i ask for leniency on is the snapping to a new tile. When you give it a new Start Position i warp it to that new place. Thats it, just mild suspension of disbelief.

Added The Heat Map but made it dynamic, as you move the start position it will update the local grid. 50x50 seems performant but may run into mild issues on larger tileSets.

^ I had planned to look into this further however, there is a lot on the coming week and this weekend. I sadly do not have time to dedicate to researching an Assignment when there is projects coming.