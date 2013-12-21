# Right now these are declared statically so that we can specify
# which objects and headers should be included in each target.
HEADERS := Armor.h Character.h Consumable.h Creature.h Game.h GenericFactory.h DungeonLevel.h Entity.h Equipment.h functions.h Item.h Player.h Random.h Rect.h Tile.h Weapon.h XMLSerializable.h
OBJECTS := Armor.o Character.o Consumable.o Creature.o Game.o GenericFactory.o DungeonLevel.o Entity.o Equipment.o functions.o Item.o Player.o Random.o Tile.o Weapon.o

CCFLAGS := -g -lncurses

INC := inc/
SRC := src/
OBJ := obj/
BIN := bin/
RES := res/

HEADERS := $(addprefix $(INC),$(HEADERS))
OBJECTS := $(addprefix $(OBJ),$(OBJECTS))

bin/game: $(OBJECTS) obj/main.o | $(BIN)
	g++ $^ $(CCFLAGS) -o $@

bin/prog1: $(OBJECTS) obj/prog1.o | $(BIN)
	g++ $^ $(CCFLAGS) -o $@

bin/la5: $(OBJECTS) obj/la5.o | $(BIN)
	g++ $^ $(CCFLAGS) -o $@

bin/test-DungeonLevel: $(OBJECTS) obj/test-DungeonLevel.o | $(BIN)
	g++ $^ $(CCFLAGS) -o $@

bin/generate-DungeonLevel: $(OBJECTS) obj/generate-DungeonLevel.o | $(BIN)
	g++ $^ $(CCFLAGS) -o $@

obj/%.o: src/%.cpp $(HEADERS) | $(OBJ)
	g++ --std=c++0x -c $< $(CCFLAGS) -o $@

$(OBJ):
	mkdir $@

$(BIN):
	mkdir $@
	ln -s ../$(RES) $(BIN)

clean:
	rm -rf obj/ bin/
