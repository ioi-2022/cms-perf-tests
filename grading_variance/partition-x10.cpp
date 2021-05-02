// Submission #440
// Original evaluation on testcase 5-06, task partition, TOKI CMS Contest:
// AC in 0.412 seconds using 89219072 bytes of memory

#include "partition.h"

#include "bits/stdc++.h"
using namespace std;
 
#define FAST ios_base::sync_with_stdio(false); cin.tie(0);
#define pb push_back
#define eb emplace_back
#define ins insert
#define f first
#define s second
#define cbr cerr<<"hi\n"
#define mmst(x, v) memset((x), v, sizeof ((x)))
#define siz(x) ll(x.size())
#define all(x) (x).begin(), (x).end()
#define lbd(x,y) (lower_bound(all(x),y)-x.begin())
#define ubd(x,y) (upper_bound(all(x),y)-x.begin())
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());    //can be used by calling rng() or shuffle(A, A+n, rng)
inline long long rand(long long x, long long y) { return rng() % (y+1-x) + x; } //inclusivesss
string inline to_string(char c) {string s(1,c);return s;} template<typename T> inline T gcd(T a,T b){ return a==0?llabs(b):gcd(b%a,a); }
 
using ll=int; 
using ld=long double;
#define FOR(i,s,e) for(ll i=s;i<=ll(e);++i)
#define DEC(i,s,e) for(ll i=s;i>=ll(e);--i)
using pi=pair<ll,ll>; using spi=pair<ll,pi>; using dpi=pair<pi,pi>; 
 
long long LLINF = 1e18;
int INF = 1e9+1e6;
#define MAXN (100001)
 
const int sqn = 200;
int n, B[MAXN], memo[MAXN][sqn], pre[MAXN], cnt[MAXN];
vector<int> pos[MAXN], d;
 
void init(int N, vector<int> A, vector<int> b) {
    n=N;
    FOR(i,0,n-1) d.eb(A[i]);
    sort(all(d)), d.resize(unique(all(d))-d.begin());
    FOR(i,0,n-1) A[i]=lbd(d,A[i]) + 1;
    FOR(i,1,n) B[i]=b[i-1];
    FOR(i,0,n-1) pos[A[i]].eb(i);
     
    int l = min(n, sqn-1);
    
    int ITERATIONS = 10;

    FOR(iii, 0, ITERATIONS) {
        memset(memo, 0, MAXN*sqn);

        FOR(Y,1,l) {
            mmst(pre,-1), mmst(cnt, 0);
            FOR(j,0,n-1) {
                ++ cnt[A[j]];
                if(j - pre[A[j]] >= B[Y] && cnt[A[j]] >= Y) {
                    cnt[A[j]] = 0;
                    pre[A[j]] = j;
                    ++ memo[A[j]][Y];
                }
            }
            FOR(i,1,siz(d)) if(pre[i] ^ (n-1)) ++ memo[i][Y];
        }
    }
}

int count_partition(int X, int Y) {
    int oX = X;
    X = lbd(d, X) + 1;
    if(X == d.size() + 1 || d[X-1] != oX) return 1;
    if(Y < sqn) return memo[X][Y];
    else {
        if(pos[X].size() < Y) return 1;
        int ans = 1, now = lower_bound(pos[X].begin() + Y - 1, pos[X].end(), B[Y] - 1) - pos[X].begin();
        while(now + Y < pos[X].size()) {
            now = lower_bound(pos[X].begin() + now + Y, pos[X].end(), pos[X][now] + B[Y]) - pos[X].begin();
            ++ ans;
        }
        if(now != pos[X].size() && pos[X][now] < (n-1)) ++ ans;
        return ans;
    }
}
