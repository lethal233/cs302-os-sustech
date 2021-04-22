#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

inline int read()
{
    char ch = getchar();
    int x = 0, f = 1;
    while (ch < '0' || '9' < ch)
    {
        if (ch == '-')
            f = -1;
        ch = getchar();
    }
    while ('0' <= ch && ch <= '9')
    {
        x = x * 10 + ch - 48;
        ch = getchar();
    }
    return x * f;
}

struct lru
{
    int page;
    lru *prev;
    lru *next;

    lru(int page)
    {
        this->page = page;
    }
};

void fifo_();

void lru_();

void min_();

void clock_();

int N;
int hit;
int K;

int main()
{
    K = read();     // cache size
    int A = read(); // algorithm index
    N = read();
    if (A == 0)
    {
        fifo_();
        //fifo
    }
    else if (A == 1)
    {
        lru_();
        //lru
    }
    else if (A == 2)
    {
        //min
    }
    else if (A == 3)
    {
        //clock
    }
    printf("Hit ratio = %.2f%%", hit * 100.0 / N);
    return 0;
}

void fifo_()
{
    hit = 0;
    queue<int> q;
    unordered_map<int, int> mp;
    for (int i = 0; i < N; ++i)
    {
        int current_page = read();
        if (mp.find(current_page) == mp.end())
        { // 不存在
            if (q.size() < K)
            { // 队列未满
                q.push(current_page);
                mp[current_page] = 1;
            }
            else
            {
                q.pop();
                mp.erase(current_page);
            }
        }
        else
        {
            ++hit;
        }
    }
}

void lru_()
{
    hit = 0;
    unordered_map<int, lru *> mp;
    lru *head = new lru(-1);
    lru *tail = new lru(-2);
    head->next = tail;
    tail->prev = head;
    for (int i = 0; i < N; ++i)
    {
        int current_page = read();
        if (mp.find(current_page) == mp.end())
        {
            if (mp.size() < K)
            {
                lru *now = new lru(current_page);
                mp[current_page] = now;
                // 放在最前面
                now->next = head->next;
                head->next->prev = now;
                now->prev = head;
                head->next = now;
            }
            else
            {
                // 移除最后一个元素
                lru *to_remove = tail->prev;
                to_remove->prev->next = tail;
                tail->prev = to_remove->prev;
                mp.erase(to_remove->page);
                delete to_remove;
            }
        }
        else
        {
            ++hit;
            lru *now = mp[current_page];
            now->prev->next = now->next;
            now->next->prev = now->prev;
            now->prev = head;
            now->next = head->next;
            head->next = now;
            head->next->prev = now;
        }
    }
    delete head;
    delete tail;
}