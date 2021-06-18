#include <bits/stdc++.h>

using namespace std;

inline int read() {
    char ch = getchar();
    int x = 0, f = 1;
    while (ch < '0' || '9' < ch) {
        if (ch == '-')
            f = -1;
        ch = getchar();
    }
    while ('0' <= ch && ch <= '9') {
        x = x * 10 + ch - 48;
        ch = getchar();
    }
    return x * f;
}

int S, M, N, moving_distance = 0;
vector<int> l, h;

int main() {
    S = read();
    M = read();
    N = read();
    int now;
    for (int i = 0; i < N; ++i) {
        now = read();
        now <= S ? l.push_back(now) : h.push_back(now);
    }
    sort(l.begin(), l.end());
    sort(h.begin(), h.end());
    printf("%d ", S);
    for (auto i = l.rbegin(); i != l.rend(); ++i) {
        printf("%d ", *i);
    }
    for (auto i = h.rbegin(); i != h.rend(); ++i) {
        printf("%d ", *i);
    }
    printf("\n");
    if (l.empty() && !h.empty()) {
        moving_distance = *h.rbegin() * 2 - S - *h.begin();
    } else if (!l.empty() && h.empty()) {
        moving_distance = S - *l.begin();
    } else {
        moving_distance = S - *l.begin() * 2 + *h.rbegin() * 2 - *h.begin();
    }
    printf("%d\n", moving_distance);
    return 0;
}