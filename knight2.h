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
ItemType ArrType[] = {ANTIDOTE, PHOENIXDOWNI, PHOENIXDOWNII, PHOENIXDOWNIII, PHOENIXDOWNIV};
enum KnightType { PALADIN = 0,
                  LANCELOT,
                  DRAGON,
                  NORMAL };
double knightBaseDamge[] = {0.05, 0.06, 0.075};

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
    for (int i = 0; i < 3; ++i) {
        if (arr[i] == 0) {
            return 0;
        }
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
    }
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

const string medicine[] = {"Antidote", "PhoenixI", "PhoenixII", "PhoenixIII", "PhoenixIV"};

class BaseBag {

public:
    virtual bool insertFirst(BaseItem *item) = 0;
    virtual BaseItem *get(ItemType itemType) = 0;
    virtual string toString() const = 0;
    virtual ~BaseBag(){};
    virtual void Drop() = 0;
    virtual void GoFind(int &HP, int &maxHP) = 0;
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
    int getFirst() {
        return this->head->item->GetType();
    }
    ~CustomBag() {
        Node *ptr = head;
        while (ptr != nullptr) {
            Node *next = ptr->next;
            delete ptr->item;
            delete ptr;
            ptr = next;
        }
    };
    bool checkUse(ItemType type, int &HP, int &maxHP) {
        if (type == PHOENIXDOWNI) {
            if (HP <= 0) {
                HP = maxHP;
                return 1;
            }
        } else if (type == PHOENIXDOWNII) {
            if (HP < maxHP / 4) {
                HP = maxHP;
                return 1;
            }
        } else if (type == PHOENIXDOWNIII) {
            if (HP < maxHP / 3) {
                if (HP <= 0) {
                    HP = maxHP / 3;
                } else {
                    HP += maxHP / 4;
                }
                return 1;
            }
        } else if (type == PHOENIXDOWNIV) {
            if (HP < maxHP / 2) {
                if (HP <= 0) {
                    HP = maxHP / 2;
                } else {
                    HP += maxHP / 5;
                }
                return 1;
            }
        }
        return 0;
    }
    void GoFind(int &HP, int &maxHP) override {
        ItemType arr[n];
        int cnt = 0;
        while (head != nullptr) {
            arr[cnt++] = head->item->GetType();
            Node *ptr = head;
            head = head->next;
            delete ptr;
        }

        int l = 0;
        for (int i = 0; i < n; ++i) {
            if (checkUse(arr[i], HP, maxHP)) {
                if (i == l) {
                    l++;
                } else {
                    swap(arr[i], arr[l]);
                    l++;
                }
                break;
            }
        }
        this->n = 0;
        for (int i = cnt - 1; i >= l; --i) {
            this->insertFirst(new BaseItem(arr[i]));
        }
    }
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
        ItemType arr[n];
        int cnt = 0;
        Node *ptr = head;
        while (ptr != nullptr) {
            arr[cnt++] = ptr->item->GetType();
            ptr = ptr->next;
        }

        while (head != nullptr) {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        int id = -1;
        for (int i = 0; i < this->n; ++i) {
            if (arr[i] == itemType) {
                id = i;
                break;
            }
        }
        if (id == -1) {
            this->n = 0;
            for (int i = cnt - 1; i >= 0; --i) {
                this->insertFirst(new BaseItem(arr[i]));
            }
            return nullptr;
        }
        swap(arr[0], arr[id]);

        this->n = 0;
        for (int i = cnt - 1; i >= 1; --i) {
            this->insertFirst(new BaseItem(arr[i]));
        }
        return new BaseItem(itemType);
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
        string ans = "Bag[count=" + to_string(this->n) + ";";
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
        bag->GoFind(this->hp, this->maxhp);
        this->hp = min(this->hp, this->maxhp);
        return this->hp > 0;
    }
    bool insertFirst(ItemType type) {
        return this->bag->insertFirst(new BaseItem(type));
    }
    bool RebornUsingGil() {
        if (this->hp > 0) {
            return 1;
        }
        if (this->gil >= 100) {
            this->gil -= 100;
            this->hp = this->maxhp / 2;
            return this->hp > 0;
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
        this->hp = min(this->hp, this->maxhp);
    }
    void updateLevel(int level) {
        this->level = level;
        this->level = min(this->level, 10);
    }
    void updateGil(int gil) {
        this->gil = gil;
        this->gil = min(this->gil, 999);
    }
    void reduceGilByHalf() {
        this->gil /= 2;
    }
    int getHP() {
        return this->hp;
    }
    void incLevel() {
        this->level++;
        this->level = min(this->level, 10);
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
        if (checkDragon(maxhp)) {
            this->knightType = DRAGON;
        } else if (checkLancelot(maxhp)) {
            this->knightType = LANCELOT;
        } else if (checkPaladin(maxhp)) {
            this->knightType = PALADIN;
        } else {
            this->knightType = NORMAL;
        }
        if (this->knightType == DRAGON) {
            antidote = 0;
        }
        this->bag = new CustomBag(phoenixdownI, antidote, maxItem);
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

class ArmyKnights;

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
    bool shield = 0, spear = 0, hair = 0, sword = 0, metHades = 0, metomega = 0;
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
            cin >> hp >> level >> phoenixdowni >> gil >> antidote;
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

        int type = knight->GetType();
        int levelO = (opponent->getI() + opponent->getEventID()) % 10 + 1;

        if (knight == nullptr) {
            return false;
        }

        if (opponent->getEventID() <= 5) {
            if (type == PALADIN || type == LANCELOT || levelO <= knight->getLevel()) {
                int gil = Gil[opponent->getEventID() - 1];
                for (int i = this->count(); i >= 1; --i) {
                    int current_gil = this->GetbyID(i)->getGil();
                    int update_gil = min(999, current_gil + gil);
                    int taken = update_gil - current_gil;
                    gil -= taken;
                    this->GetbyID(i)->updateGil(update_gil);
                }
                return 1;
            }
            int damage = (levelO - knight->getLevel()) * BaseDamage[opponent->getEventID() - 1];

            knight->updateHP(knight->getHP() - damage);
            // if (opponent->getI() == 5) {
            //     cerr << damage << endl;
            // }
            if (knight->Reborn() || knight->RebornUsingGil()) {
                return 1;
            }
        }

        if (opponent->getEventID() == 6) {

            if (type == DRAGON) {
                knight->incLevel();
                return 1;
            }

            if (levelO > knight->getLevel()) {
                if (knight->useAntidote()) {
                    return 1;
                }
                knight->drop();
                knight->updateHP(knight->getHP() - 10);
                return knight->Reborn() || knight->RebornUsingGil();
            }

            knight->incLevel();
            return 1;
        }

        if (opponent->getEventID() == 7) {
            if (knight->getLevel() >= levelO) {
                int newGil = min(knight->getGil() * 2, 999);
                int rem = knight->getGil() * 2 - newGil;
                knight->updateGil(newGil);

                for (int i = this->count() - 1; i >= 1 && rem > 0; --i) {
                    int gil = aKnight[i]->getGil();
                    int take = min(rem, 999 - gil);
                    gil += take;
                    rem -= take;
                    aKnight[i]->updateGil(gil);
                }
            } else {
                if (knight->GetType() != PALADIN) {
                    knight->reduceGilByHalf();
                }
            }
            return 1;
        }

        if (opponent->getEventID() == 8) {
            if (knight->GetType() == PALADIN) {
                if (knight->getHP() < knight->getMaxHP() / 3) {
                    knight->updateHP(knight->getHP() + knight->getMaxHP() / 5);
                }
            } else if (knight->getGil() >= 50) {
                if (knight->getHP() < knight->getMaxHP() / 3) {
                    knight->updateGil(knight->getGil() - 50);
                    knight->updateHP(knight->getHP() + knight->getMaxHP() / 5);
                }
            }
            return 1;
        }
        if (opponent->getEventID() == 9) {
            knight->updateHP(knight->getMaxHP());
            return 1;
        }
        if (opponent->getEventID() == 10) {
            if (this->metomega) {
                return 1;
            }
            if (knight->GetType() == DRAGON || (knight->getHP() == knight->getMaxHP() && knight->getLevel() == 10)) {
                this->metomega = 1;
                knight->updateGil(999);
                knight->updateLevel(10);
            } else {
                knight->updateHP(0);
                return knight->Reborn() || knight->RebornUsingGil();
            }
            return 1;
        }
        if (opponent->getEventID() == 11) {
            if (this->metHades) {
                return 1;
            }
            if ((knight->GetType() == PALADIN && knight->getLevel() >= 8) || (knight->getLevel() >= 10)) {
                this->takeShield();
                this->metHades = 1;
            } else {
                knight->updateHP(0);
                return knight->Reborn() || knight->RebornUsingGil();
            }
            return 1;
        }
        if (opponent->getEventID() == 95) {
            this->takeShield();
            return 1;
        }
        if (opponent->getEventID() == 96) {
            this->takeSpear();
            return 1;
        }
        if (opponent->getEventID() == 97) {
            this->takeGunevereHair();
            return 1;
        }
        if (opponent->getEventID() == 98) {
            if (this->hasPaladinShield() && this->hasGuinevereHair() && this->hasLancelotSpear()) {
                this->sword = 1;
            }
            return 1;
        }
        if (opponent->getEventID() >= 112 && opponent->getEventID() <= 114) {
            for (int i = this->count(); i >= 1; --i) {
                if (this->aKnight[i]->insertFirst(ArrType[opponent->getEventID() - 110])) {
                    break;
                }
            }
            return 1;
        }
        if (opponent->getEventID() == 99) {
            if (this->hasExcaliburSword()) {
                return 1;
            }
            if (!(this->hasGuinevereHair() && this->hasLancelotSpear() && this->hasPaladinShield())) {
                return 0;
            }
            bool del[n + 4];
            for (int i = 0; i < n + 4; ++i) {
                del[i] = 0;
            }
            int OriginalHP = 5000;

            for (int i = this->n; i >= 1; --i) {
                if (this->aKnight[i]->GetType() == NORMAL) {
                    continue;
                }
                BaseKnight *k = this->aKnight[i];
                int damage = 1.00 * knightBaseDamge[k->GetType()] * k->getHP() * k->getLevel();
                OriginalHP -= damage;
                if (OriginalHP > 0) {
                    del[i] = 1;
                } else {
                    break;
                }
            }
            if (OriginalHP > 0) {
                this->n = 0;
                return 0;
            }

            int pre_n = this->n;
            this->n = 0;
            for (int i = 1; i <= pre_n; ++i) {
                if (del[i]) {
                    continue;
                }
                aKnight[++this->n] = aKnight[i];
            }
            return 1;
        }
        return 0;
    };

    bool PopOut() {
        if (this->n > 0) {
            this->n--;
            return 1;
        }
        return 0;
    }

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
        int n = events->count();
        for (int i = 1; i <= n; ++i) {
            int id = events->get(i);
            BaseOpponent *opponent = new BaseOpponent(id, i - 1);
            while (armyKnights->fight(opponent) == 0 && armyKnights->count() > 0) {
                armyKnights->PopOut();
            }
            armyKnights->printInfo();
            if (armyKnights->count() == 0) {
                break;
            }
        }
        armyKnights->printResult(armyKnights->count() > 0 ? 1 : 0);
    };
};

#endif // __KNIGHT2_H__