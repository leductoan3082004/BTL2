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

struct SCP {
    int id;
    int objClass;
    string speConProcedures;
    string description;
    string *addendums;
    int numAddendums;
};
SCP *cloneSCP(SCP *original) {
    SCP a = *original;
    SCP *b = new SCP(a);
    b->addendums = new string[b->numAddendums];
    for (int i = 0; i < b->numAddendums; ++i) {
        b->addendums[i] = original->addendums[i];
    }
    return b;
}

void gogo() {
    string *addendums = new string[2];
    addendums[0] = "Document #055-1: An Analysis of SCP-055\nThe author puts forward the hypothesis that SCP-055 was never formally acquired.";
    addendums[1] = "Document #055-2: Report of Dr. John Marachek\nSurvey team #19-055-127BXE was successfully able to enter SCP-055's container and ascertain the appearance.";

    SCP *obj = new SCP{55, 2, "Object is kept within a five (5) by five (5) by two point five (2.5) meter square room.", "All known information about the object is that it is not round.", addendums, 2};

    SCP *newObj = cloneSCP(obj);

    cout << (newObj == obj) << endl;

    obj->id = 1;
    cout << obj->id << " " << newObj->id << "\n";

    delete[] obj->addendums;
    delete obj;

    delete[] newObj->addendums;
    delete newObj;
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
