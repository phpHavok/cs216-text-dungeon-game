Welcome to "Super Awesome (Just Kidding) Game" by Jacob Chappell.

# Controls

* W,A,S,D moves the player.
* > goes down a staircase (must be standing on a down staircase first).
* < goes up a staircase (must be standing on an up staircase first).
* P picks up an item and places it into your inventory.
* O drops an item from your inventory. If equipped, it unequips it.
* E equips an equippable item from your inventory. If another item is equipped, it unequips it.
* U uses a consumable item in your inventory.
* I views your inventory.
* Q quits the game.

# Game Decisions

## Prelude

Unfortunately, I've been very busy lately and didn't get much time to add "extra" features.
I also had to cop out on some design decisions in order to actually get things done.
That being said, I think my DungeonLevel generation is the strongest part of my game. My AI
is also pretty good, all things being said. I will now walk through each of the important
game decisions for discussion/extra credit.

## Choice of Coordinates

I chose to place X,Y coordinates in each Entity as well as a Character* in each Tile. Doing it both ways
permits very convenient access in various circumstances. The down side is, it is another layer of complexity
to manage. If time permitted, I would have liked to explore some encapsulation ideas such that there would
only be one "path" to update the player's position, and that single path should update the X,Y as well as
Character*.

## Dungeon Levels

DungeonLevels are randomly generated as needed. That means they (theoretically) go on FOREVER. The only real
problem with this is that I am allocating monsters that have specific stats rather than generating dynamic stats.
That means, eventually, you run out of good battles because you're on Dungeon Level 50 and still getting level
10 monters (since I haven't made monsters all the way up to level 50). However, I doubt anyone is going to play
this game for more than 5 minutes anyway, so I'm not worried about it too much :)

The general idea is, go as deep as you can collecting points, then return to the top to escape and get your final score.
The more risky you are, the better your score will be, but also the higher chance you have of dying and losing it all.

## AI

My creatures each have "line's of sight" or a minimum distance you must be before they notice you. When you are outside
of a creature's line of sight, it will just move around the level minding its own business. If you approach it and enter
its line of sight, it will start chasing you with the goal of ending your life. Be careful not to get mobbed into a corner
if a bunch of them start chasing you :).

Passive creatures will never attempt to chase or attack you. They always mind their own business.

## Battles

A battle phase takes place whenever a creature attacks you or you attack a creature. There is a simple algorithm in game
that basically sums up base defense/attacks of creatures plus attack/defense bonuses due to weapons/armor to find out
how much damage should be inflicted. That being said, there is a minimum damage of 1 hit point. That way you can't armor
the hell up and escape attack entirely.

## Experience/Levels

Each monster has a certain amount of experience it gives you upon death. A simple algorithm calculates how much experience
you need to gain a level, and this amount goes up after each level. Leveling up will add (randomly) either 1 to your base
attack or 1 to your base defense. You will also always get +10 max health, and your health will be restored to max.
Your experience is your score at the end of the game.

## Items

### Armor

Armor appears as [Armor] in your inventory. You can equip these items and they add to your base defense.

### Weapon

Weapon appears as [Weapon] in your inventory. You can equip these items and they add to your base attack.

### Consumable

Consumable appears as [Consumable] in your inventory. You can use these and they increase your health.

### Other

Other items have no real use.

## Inventory

In the spirit of infinity, you have unlimited inventory space and no weight restrictions. Have fun :)

## Timed Events

Every so often, the player will regenerate health and there is a chance that monsters will repopulate the level.

## Lighting

There is basic lighting in the game, such that the player can only see around him/herself within a certain circular radius.

## Epilogue

All other requirements are implemented and are self explanatory.
