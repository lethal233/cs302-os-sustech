#include <bits/stdc++.h>

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

    explicit lru(int page)
    {
        this->page = page;
        this->prev = nullptr;
        this->next = nullptr;
    }
};

struct clock_node
{
    int page;
    clock_node *prev, *next;
    int count;

    explicit clock_node(int page)
    {
        this->page = page;
        prev = next = nullptr;
        count = 0;
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
        min_();
        //min
    }
    else if (A == 3)
    {
        //clock
        clock_();
    }
    printf("Hit ratio = %.2f%%\n", hit * 100.0 / N);
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
            q.push(current_page);
            mp[current_page] = 1;
            if (q.size() >= K)
            { // 队列已满
                int page = q.front();
                mp.erase(page);
                q.pop();
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
            lru *now = new lru(current_page);
            mp[current_page] = now;
            now->next = head->next;
            head->next->prev = now;
            now->prev = head;
            head->next = now;
            if (mp.size() >= K)
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
            if (mp.size() == 1)
                continue;
            lru *now = mp[current_page];
            lru *p = now->prev;
            lru *n = now->next;
            p->next = n;
            n->prev = p;
            lru *hn = head->next;
            now->prev = head;
            now->next = hn;
            hn->prev = now;
            head->next = now;

            //            now->prev->next = now->next;
            //            now->next->prev = now->prev;
            //            now->prev = head;
            //            now->next = head->next;
            //            head->next = now;
            //            head->next->prev = now;
        }
    }
    delete head;
    delete tail;
}

void min_()
{
    hit = 0;
    priority_queue<pair<int, int>> q;
    unordered_map<int, int> mp;
    vector<int> pages;
    vector<int> next(N, 0x7ffffff);
    for (int i = 0; i < N; ++i)
    {
        pages.push_back(read());
    }
    for (int i = N - 1; i >= 0; --i)
    {
        if (mp.find(pages[i]) != mp.end())
        { // 有entry，要更新
            next[i] = mp[pages[i]];
        }
        mp[pages[i]] = i;
    }
    mp.clear();
    for (int i = 0; i < N; ++i)
    {
        if (mp.find(pages[i]) == mp.end())
        {
            if (q.size() >= K)
            {
                auto t = q.top();
                q.pop();
                mp.erase(t.second);
            }
            mp[pages[i]] = 1;
        }
        else
        { //TODO: 需要判断条件吗?
            ++hit;
        }
        q.push(make_pair(next[i], pages[i]));
    }
}

void clock_()
{
    hit = 0;
    clock_node *pointer;
    auto *head = new clock_node(-1);
    auto *tail = new clock_node(-2);
    head->next = tail;
    tail->prev = head;
    unordered_map<int, clock_node *> mp;
    for (int i = 0; i < N; ++i)
    {
        int current_page = read();
        auto *cur = new clock_node(current_page);
        cur->count = 1;
        if (mp.find(current_page) == mp.end())
        {
            if (mp.size() >= K)
            {
                while (true)
                {
                    if (pointer->count == 0)
                    {
                        auto *b = pointer->prev;
                        auto *c = pointer->next;
                        mp.erase(pointer->page);
                        delete pointer;
                        cur->prev = b;
                        b->next = cur;
                        cur->next = c;
                        c->prev = cur;
                        pointer = cur;
                        mp[current_page] = cur;
                        break;
                    }
                    else
                    {
                        pointer->count = 0;
                    }
                }
            }
            else
            {
                auto *tmp = tail->prev;
                tmp->next = cur;
                cur->next = tail;
                tail->prev = cur;
                cur->prev = tmp;
                mp[current_page] = cur;
                if (mp.size() == K)
                {
                    tmp = head->next;
                    cur->next = tmp;
                    tmp->prev = cur;
                    pointer = tmp;
                }
            }
        }
        else
        {
            ++hit;
            mp[current_page]->count = 1;
        }
    }
}