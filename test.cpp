#include <bits/stdc++.h>

using namespace std;

#define task "sol"
#define lb lower_bound
#define ub upper_bound
#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define zs(v) ((int)(v).size())
#define BIT(x, i) (((x) >> (i)) & 1)
#define CNTBIT __builtin_popcountll
#define ALL(v) (v).begin(), (v).end()
#define endl '\n'

typedef long double ld;
typedef long long ll;
typedef pair<int, int> pii;

const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int dxx[8] = {-1, -1, 0, 1, 1, 1, 0, -1}, dyy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll mod = 1000000007; /// 998244353
const ll base = 331;
class ArmyKnights {
private:
    bool shield = 0, spear = 0, hair = 0, sword = 0;
    int n = 0;
    int maxn = 0;
    // BaseKnight **aKnight = nullptr;

public:
    ArmyKnights(const string &file_armyknights) {
        this->shield = 0;
        this->spear = 0;
        this->hair = 0;
        this->sword = 0;
        this->n = 0;
        this->maxn = 0;
    };
};

class KnightAdventure {
private:
    ArmyKnights *armyKnights;

public:
    KnightAdventure() {
        armyKnights = nullptr;
    };
    ~KnightAdventure() {
        delete armyKnights;
    }; // TODO:

    void loadArmyKnights(const string &file_army) {
        this->armyKnights = new ArmyKnights(file_army);
        cerr << "ending reading armyknights...\n";
    };
    void run() {
        cout << "success";
    };
};
void gogo() {
    KnightAdventure *knightAdventure;
    knightAdventure = new KnightAdventure();
    knightAdventure->loadArmyKnights("ahihi");
    knightAdventure->run();
    delete knightAdventure;
}
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    if (fopen(task ".inp", "r")) {
        freopen(task ".inp", "r", stdin);
        freopen(task ".out", "w", stdout);
    }
    gogo();
}
