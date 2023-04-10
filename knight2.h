#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

// #define DEBUG
enum ItemType {
    ANTIDOTE = 0,
    PHOENIXDOWNI,
    PHOENIXDOWNII,
    PHOENIXDOWNIII,
    PHOENIXDOWNIV,
};
enum KnightType { PALADIN = 0,
                  LANCELOT,
                  DRAGON,
                  NORMAL };

string Convert(int x) {
    string ans = "";
    while (x > 0) {
        ans = char(x % 10 + '0') + ans;
        x /= 10;
    }
    return ans;
}
bool checkPaladin(int x) {
    for (int i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            return 0;
        }
    }
    return 1;
}
bool checkLancelot(int x) {
    return (x == 888);
}
int SQR(int x) {
    return x * x;
}
bool checkDragon(int x) {
    int arr[3] = {0, 0, 0};
    int cnt = 0;
    while (x > 0) {
        arr[cnt++] = x % 10;
        x /= 10;
    }
    if (cnt < 3) {
        return 0;
    }
    if (SQR(arr[0]) + SQR(arr[1]) == SQR(arr[2])) {
        return 1;
    }
    if (SQR(arr[1]) + SQR(arr[2]) == SQR(arr[0])) {
        return 1;
    }
    if (SQR(arr[0]) + SQR(arr[2]) == SQR(arr[1])) {
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------------------------//
class BaseItem {
protected:
    ItemType type;

public:
    BaseItem(ItemType type) {
        this->type = type;
    };
    ItemType GetType() {
        return this->type;
    }
    // virtual bool canUse(BaseKnight *knight) {
    //     return 0;
    // }
    // virtual void use(BaseKnight *knight) {
    //     return;
    // }
};
// ---------------------------------------------------------------------------------------------//

const string medicine[] = {"Antidote", "PhoenixDownI", "PhoenixDownII", "PhoenixDownIII", "PhoenixDownIV"};

class BaseBag {

public:
    virtual bool insertFirst(BaseItem *item) = 0;
    virtual BaseItem *get(ItemType itemType) = 0;
    virtual string toString() const = 0;
    virtual ~BaseBag(){};
    virtual void Drop() = 0;
};
class CustomBag : public BaseBag {

private:
    int n = 0, maxItem = 0;
    struct Node {
        BaseItem *item;
        Node *next;
        Node(BaseItem *item) : item(item), next(nullptr) {}
    };
    Node *head = nullptr;

public:
    CustomBag(int phoenixdown, int antidote, int maxItem) {
        this->maxItem = maxItem;
        for (int i = 0; i < phoenixdown; ++i) {
            this->insertFirst(new BaseItem(PHOENIXDOWNI));
        }
        for (int i = 0; i < antidote; ++i) {
            this->insertFirst(new BaseItem(ANTIDOTE));
        }
    };
    ~CustomBag() {
        Node *ptr = head;
        while (ptr != nullptr) {
            Node *next = ptr->next;
            delete ptr->item;
            delete ptr;
            ptr = next;
        }
    };
    bool insertFirst(BaseItem *item) override {
        if (n >= maxItem) {
            return false;
        }
        Node *NewItem = new Node(item);
        if (!NewItem) {
            return false;
        }
        NewItem->next = head;
        head = NewItem;
        n++;
        return true;
    };
    BaseItem *get(ItemType itemType) override {
        Node *ptr = this->head;
        while (ptr != nullptr) {
            if (itemType == ptr->item->GetType()) {
                break;
            }
            ptr = ptr->next;
        }
        if (ptr == nullptr) {
            return nullptr;
        }
        Node *ans = ptr;

        Node *prev = this->head;
        while (prev != nullptr && prev->next != ptr) {
            prev = prev->next;
        }
        if (prev == nullptr) {
            return nullptr;
        }
        Node *tmp = this->head->next;
        this->head->next = ptr->next;
        prev->next = this->head;
        this->head = tmp;
        n--;

        return ans->item;
    };
    void Drop() {
        for (int i = 0; i < 3; ++i) {
            if (this->head != nullptr) {
                this->get(this->head->item->GetType());
            }
        }
    }
    string toString() const override {
        Node *ptr = head;
        string ans = "Bag[count=" + Convert(n) + ";";
        while (ptr != nullptr) {
            ans += medicine[ptr->item->GetType()];
            ptr = ptr->next;
            if (ptr != nullptr) {
                ans += ",";
            }
        }
        ans += "]";
        return ans;
    }
};

int BaseDamage[] = {10, 15, 45, 75, 95};
int Gil[] = {100, 150, 450, 750, 800};

class BaseOpponent {
protected:
    int eventID, i;

public:
    BaseOpponent(int eventID, int i) {
        this->eventID = eventID;
        this->i = i;
    }
    int getEventID() {
        return this->eventID;
    }
    int getI() {
        return this->i;
    }
    virtual bool fight(ArmyKnights *a) = 0;
};
class Monster : public BaseOpponent { // use for event numbered 1 to 5
public:
    bool fight(ArmyKnights *a) override {
        BaseKnight *knight = a->lastKnight();
        if (knight == nullptr) {
            return false;
        }

        bool type = knight->GetKnightType();
        int levelO = (this->getI() + this->getEventID()) % 10 + 1;

        if (type == PALADIN || type == LANCELOT || levelO <= knight->getLevel()) {
            int gil = Gil[this->getEventID() - 1];
            for (int i = a->count(); i >= 1; --i) {
                int current_gil = a->GetbyID(i)->getGil();
                int update_gil = min(999, current_gil + gil);
                int taken = update_gil - current_gil;
                gil -= taken;
                a->GetbyID(i)->updateGil(update_gil);
            }
            return 1;
        }
        int damage = (levelO - knight->getLevel()) * BaseDamage[this->getEventID() - 1];
        knight->updateHP(knight->getHP() - damage);
        if (knight->Reborn() || knight->RebornUsingGil()) {
            return 1;
        }
    };
};

class BaseKnight {
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    BaseBag *bag;
    KnightType knightType;

public:
    static BaseKnight *create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
        return new BaseKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    };
    ~BaseKnight() {
        delete bag;
    }
    bool Reborn() {
        if (this->hp > 0) {
            return 1;
        }
        if (this->hp <= 0 && this->getBag(PHOENIXDOWNI)) {
            this->hp = this->maxhp;
        } else if (this->getBag(PHOENIXDOWNII) && this->hp < this->maxhp / 4) {
            this->hp = this->maxhp;
        } else if (this->getBag(PHOENIXDOWNIII) && this->hp < this->maxhp / 3) {
            if (this->hp <= 0) {
                this->hp = this->maxhp;
            } else {
                this->hp += this->maxhp / 4;
            }
        } else if (this->getBag(PHOENIXDOWNIV) && this->hp < this->maxhp / 2) {
            if (this->hp <= 0) {
                this->hp = this->maxhp / 2;
            } else {
                this->hp += this->maxhp / 5;
            }
        }
        return (this->hp > 0);
    }
    bool RebornUsingGil() {
        if (this->hp > 0) {
            return 1;
        }
        if (this->gil >= 100) {
            this->gil -= 100;
            this->hp = this->maxhp;
            return 1;
        }
        return 0;
    }
    bool getBag(ItemType type) {
        if (this->bag->get(type) == nullptr) {
            return 0;
        }
        return 1;
    }
    void updateHP(int HP) {
        this->hp = HP;
    }
    void updateLevel(int level) {
        this->level = level;
    }
    void updateGil(int gil) {
        this->gil = gil;
    }
    void reduceGilByHalf() {
        this->gil /= 2;
    }
    int getHP() {
        return this->hp;
    }
    void incLevel() {
        this->level++;
    }
    void drop() {
        this->bag->Drop();
    }
    int getGil() {
        return this->gil;
    }
    int getMaxHP() {
        return this->maxhp;
    }
    bool usePhoenixDownI() {
        if (this->bag->get(PHOENIXDOWNI) == nullptr) {
            return false;
        }
        return true;
    }
    bool useAntidote() {
        if (this->bag->get(ANTIDOTE) == nullptr) {
            return false;
        }
        return true;
    }
    bool updateHPandUsePhoenixI(int damage) {
        this->hp -= damage;
        if (this->hp <= 0) {
            BaseItem *ptr = this->bag->get(PHOENIXDOWNI);
            if (ptr == nullptr) {
                this->hp = this->maxhp;
                return false;
            }
        }
        return true;
    };
    int getLevel() {
        return this->level;
    }
    int GetKnightType() {
        return this->knightType;
    };
    BaseKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
        this->id = id;
        this->hp = maxhp;
        this->maxhp = maxhp;
        this->level = level;
        this->gil = gil;

        // cerr << "starting1....\n";
        int maxItem = (checkDragon(maxhp) ? 14 : (checkLancelot(maxhp) ? 16 : 19));
        if (checkPaladin(maxhp)) {
            maxItem = 1e9;
        }
        this->bag = new CustomBag(phoenixdownI, antidote, maxItem);
        if (checkDragon(maxhp)) {
            this->knightType = DRAGON;
        } else if (checkLancelot(maxhp)) {
            this->knightType = LANCELOT;
        } else if (checkPaladin(maxhp)) {
            this->knightType = PALADIN;
        } else {
            this->knightType = NORMAL;
        }
    };

    int GetType() {
        return this->knightType;
    };

    string toString() const {
        string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
        string s("");
        s += "[Knight:id:" + to_string(id) + ",hp:" + to_string(hp) + ",maxhp:" + to_string(maxhp) + ",level:" + to_string(level) + ",gil:" + to_string(gil) + "," + bag->toString() + ",knight_type:" + typeString[knightType] + "]";
        return s;
    };
};
class Events {
private:
    int *event = nullptr;
    int *n = nullptr;

public:
    Events(const string &file_events) {

        ifstream cin(file_events);
        this->n = new int;
        cin >> *this->n;
        event = new int[*this->n + 5];
        for (int i = 1; i <= *this->n; ++i) {
            cin >> this->event[i];
        }
        cin.close();
    };
    ~Events() {
        delete event;
        delete n;
    }
    int count() const {
        return *this->n;
    };
    int get(int i) const {
        return this->event[i];
    };
};

class ArmyKnights {
private:
    bool shield = 0, spear = 0, hair = 0, sword = 0;
    int n = 0;
    int maxn = 0;
    BaseKnight **aKnight = nullptr;

public:
    ArmyKnights(const string &file_armyknights) {
        this->shield = 0;
        this->spear = 0;
        this->hair = 0;
        this->sword = 0;
        ifstream cin(file_armyknights);

        cin >> this->n;
        this->maxn = this->n;
        this->aKnight = new BaseKnight *[this->n + 5];

        for (int i = 1; i <= this->n; ++i) {
            int hp, level, gil, antidote, phoenixdowni;
            cin >> hp >> level >> gil >> antidote >> phoenixdowni;
            this->aKnight[i] = new BaseKnight(i, hp, level, gil, antidote, phoenixdowni);
        }

        cin.close();
    };
    BaseKnight *GetbyID(int i) {
        if (i > this->n) {
            return nullptr;
        }
        return this->aKnight[i];
    }
    void giveGilRemaining(int gil) {
        if (n <= 0) {
            return;
        }
    }
    bool fight(BaseOpponent *opponent) {
        BaseKnight *knight = this->lastKnight();
        if (knight == nullptr) {
            return false;
        }

        bool type = knight->GetKnightType();
        int levelO = (opponent->getI() + opponent->getEventID()) % 10 + 1;

        return opponent->fight(this);
    };

    ~ArmyKnights() {
        delete aKnight;
    };
    int count() const {
        return this->n;
    };
    BaseKnight *lastKnight() const {
        if (this->n == 0) {
            return nullptr;
        }
        return aKnight[n];
    };
    void takeShield() {
        this->shield = 1;
    }
    void takeSpear() {
        this->spear = 1;
    }
    void takeGunevereHair() {
        this->hair = 1;
    }
    bool hasPaladinShield() const {
        return this->shield;
    };
    bool hasLancelotSpear() const {
        return this->spear;
    };
    bool hasGuinevereHair() const {
        return this->hair;
    };
    bool hasExcaliburSword() const {
        return this->sword;
    };

    void printInfo() const {
        cout << "No. knights: " << this->count();
        if (this->count() > 0) {
            BaseKnight *lknight = lastKnight(); // last knight
            cout << ";" << lknight->toString();
        }
        cout << ";PaladinShield:" << this->hasPaladinShield()
             << ";LancelotSpear:" << this->hasLancelotSpear()
             << ";GuinevereHair:" << this->hasGuinevereHair()
             << ";ExcaliburSword:" << this->hasExcaliburSword()
             << endl
             << string(50, '-') << endl;
    };
    void printResult(bool win) const {
        cout << (win ? "WIN" : "LOSE") << '\n';
    };

    bool adventure(Events *events) {
        return false;
    };
};

class KnightAdventure {
private:
    ArmyKnights *armyKnights;
    Events *events;

public:
    KnightAdventure() {
        armyKnights = nullptr;
        events = nullptr;
    };
    ~KnightAdventure() {
        delete armyKnights;
        delete events;
    }; // TODO:

    void loadArmyKnights(const string &file_army) {
        this->armyKnights = new ArmyKnights(file_army);
        // cerr << "ending reading armyknights...\n";
    };
    void loadEvents(const string &file_event) {
        this->events = new Events(file_event);
    };
    void run() {
        cout << "ahihi";
    };
};

#endif // __KNIGHT2_H__