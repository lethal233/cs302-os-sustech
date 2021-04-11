// #include <bits/stdc++.h>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
typedef vector<int> vint;
int r;

struct Node
{
    vint *allocate;
    vint *max;
    vint *need;
    bool done;
    Node(vint *vec)
    {
        this->max = vec;
        this->allocate = new vint(r, 0);
        this->need = new vint(r);
        for (int i = 0; i < r; ++i)
            this->need->at(i) = vec->at(i);
        this->done = false;
    }
    ~Node()
    {
        delete max;
        delete allocate;
        delete need;
    }
};

unordered_map<string, Node *> m;
vint *available, *all;

vint *read_vector(); // to read the vector
void discard();
void release(Node *);
bool ok_for_new(vint *);
bool ok_for_request(vint *);
bool ok_for_allocation(vint *);
void meet_request(Node *, vint *);
void undo_request(Node *, vint *);
bool ok_for_need(Node *, vint *);
bool is_safe();
void debug(vint *a)
{
    for (int i = 0; i < r; i++)
    {
        cout << a->at(i) << " ";
    }
    cout << endl;
}

void debug()
{
    unordered_map<string, Node *>::iterator iter;
    for (iter = m.begin(); iter != m.end(); iter++)
    {
        cout << "pid: here" << iter->first << " " << endl;
        for (int i = 0; i < r; i++)
        {
            if (iter->second == nullptr)
                break;
            cout << iter->second->max->at(i) << " ";
        }
        cout << endl;
    }
}

int main()
{
    string pid, status;
    cin >> r;
    all = read_vector();
    available = new vint(r);
    for (int i = 0; i < r; ++i)
        available->at(i) = all->at(i);
    Node *tmp;
    while (cin >> pid >> status)
    {
        // debug(available);
        // debug(all);
        tmp = m[pid];
        if (status == "new")
        {
            if (tmp != nullptr)
            {
                cout << "NOT OK" << endl;
                discard();
            }
            else
            {
                vint *max_ = read_vector();
                if (ok_for_new(max_))
                {
                    Node *n = new Node(max_);
                    m[pid] = n;
                    // cout << pid << "97" <<endl;
                    cout << "OK" << endl;
                }
                else
                {
                    // cout << pid << "101" <<endl;
                    cout << "NOT OK" << endl;
                }
            }
        }
        else if (status == "request")
        {
            // debug();
            // cout << pid << "110" <<endl;
            if (tmp != nullptr)
            {
                vint *request_ = read_vector();
                if (ok_for_request(request_) && ok_for_need(m[pid], request_)) // ok for request <= available && ok for request <= need
                {
                    meet_request(m[pid], request_);
                    if (is_safe())
                    {
                        cout << "OK" << endl;
                    }
                    else
                    {
                        cout << "NOT OK" << endl;
                        undo_request(m[pid], request_);
                    }
                }
                else
                {
                    cout << "NOT OK" << endl;
                }
            }
            else
            {
                // not in the map
                cout << "NOT OK" << endl;
                discard();
            }
        }
        else if (status == "terminate")
        { // release the resources
            if (tmp != nullptr)
            {
                Node *old = m[pid];
                m.erase(pid);
                release(old);
                cout << "OK" << endl;
            }
            else
            {
                // not exist
                cout << "NOT OK" << endl;
            }
        }
    }

    return 0;
}

vint *read_vector()
{
    vint *vec = new vector<int>;
    int tmp;
    for (int i = 0; i < r; ++i)
    {
        cin >> tmp;
        vec->push_back(tmp);
    }
    return vec;
}
void discard()
{
    int tmp;
    for (int i = 0; i < r; ++i)
        cin >> tmp;
}
bool ok_for_new(vint *max_need)
{
    for (int i = 0; i < r; ++i)
    {
        if (max_need->at(i) > all->at(i))
            return false;
    }
    return true;
}
bool ok_for_request(vint *request)
{
    for (int i = 0; i < r; ++i)
    {
        if (request->at(i) > available->at(i))
            return false;
    }
    return true;
}
bool ok_for_need(Node *a, vint *request)
{
    for (int i = 0; i < r; ++i)
    {
        if (request->at(i) > a->need->at(i))
            return false;
    }
    return true;
}
bool ok_for_allocation(vint *need)
{
    for (int i = 0; i < r; ++i)
    {
        if (need->at(i) > available->at(i))
            return false;
    }
    return true;
}
void meet_request(Node *a, vint *request)
{
    for (int i = 0; i < r; ++i)
    {
        available->at(i) -= request->at(i);
        a->allocate->at(i) += request->at(i);
        a->need->at(i) -= request->at(i);
    }
}
void release(Node *old)
{
    for (int i = 0; i < r; ++i)
        available->at(i) += old->allocate->at(i);
}
bool is_safe()
{
    bool flag = true;
    int *work = new int[r];
    for (int i = 0; i < r; ++i)
        work[i] = available->at(i);
    unordered_map<string, Node *>::iterator iter;
    // cout << "224" << endl;
    for (iter = m.begin(); iter != m.end();)
    {
        if (iter->second == nullptr || iter->second->done)
        {
            iter++;
            continue;
        }
        else
        {
            // cout << "true for " << iter->first << " 231" << endl;
            int j;
            for (j = 0; j < r; ++j)
            {
                if (iter->second->need->at(j) > work[j])
                    break;
            }
            // cout << "true for " << iter->first<< " 238" << endl;
            if (j == r)
            {
                // cout << "true for " << iter->first<< " 241" << endl;
                iter->second->done = true;
                for (j = 0; j < r; ++j)
                    work[j] += iter->second->allocate->at(j);
                // cout << "true for " << iter->first<< " 245" << endl;
                iter = m.begin();
                continue;
            }
            else
            {
                // cout << "true for " << iter->first<< " 250" <<endl;
                iter++;
                continue;
            }
        }
    }
    for (iter = m.begin(); iter != m.end(); iter++)
    {
        if (iter->second == nullptr)
            continue;
        if (!iter->second->done)
            flag = false;
        iter->second->done = false;
    }
    delete[] work;
    return flag;
}

void undo_request(Node *a, vint *request)
{
    for (int i = 0; i < r; ++i)
    {
        available->at(i) += request->at(i);
        a->allocate->at(i) -= request->at(i);
        a->need->at(i) += request->at(i);
    }
}