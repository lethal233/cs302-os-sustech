1. unordered_map：用来存储<进程号, 进程资源信息的结构体的指针>，使用原因：方便快速查找到是否有对应的pid的进程 `O(logn)`
2. vector：动态分配空间，用于存储资源信息
3. struct Node，如下所示，记录每个进程对应的allocate, max, need, 是否释放资源的信息
```c
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
```
最优吗？
可能是最优的，删除map里的进程的时候需要`O(logn)`，如果采用链表可以降到`O(1)`，但是每个结构体内需要额外记录两个指针，是空间换时间的结果。