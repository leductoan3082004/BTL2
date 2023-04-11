#include "knight2.h"

int main(int argc, char **argv) {
    string file_armyknights, file_events;
    if (argc == 1) {
        file_armyknights = "tc1_armyknights"; // hard-code
        file_events = "tc1_events";
    } else if (argc == 3) {
        file_armyknights = argv[1];
        file_events = argv[2];
    } else {
        cout << "Error number of arguments" << endl;
        exit(1);
    }

    CustomBag *bag = new CustomBag(1, 1, 10);
    bag->insertFirst(new BaseItem(PHOENIXDOWNIII));
    bag->insertFirst(new BaseItem(PHOENIXDOWNII));
    bag->insertFirst(new BaseItem(PHOENIXDOWNIV));

    cout << bag->toString() << endl;
    int hp = 0, maxhp = 999;
    bag->GoFind(hp, maxhp);
    cout << bag->toString();
    cout << endl;
    cout << hp << ' ' << maxhp << endl;

    delete bag;
    // KnightAdventure *knightAdventure = new KnightAdventure();
    // knightAdventure->loadArmyKnights(file_armyknights);
    // knightAdventure->loadEvents(file_events);
    // knightAdventure->run();

    // delete knightAdventure;

    return 0;
}