#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32	  //进程名最大长度
#define MIN_SLICE 10		  //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024 //总内存大小
#define DEFAULT_MEM_START 0	  //内存开始分配时的起始地址

typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE; // the maximum mem size
bool flag = 0;					 //当内存以及被分配了之后，不允许更改总内存大小的flag
static int pid = 0;				 //global pid
int mode = 1;					 // 1: Best Fit 2: Worst Fit 3: First Fit 4: Buddy System
My_algo algo[123];

struct free_block
{ //空闲数据块
	int size;
	int start_addr;
	struct free_block *next;
};

struct allocated_block
{ //已分配的数据块
	int pid;
	int size;
	int start_addr;
	int *data;
	struct allocated_block *next;
};

free_block *free_block_head;					 //空闲数据块首指针
allocated_block *allocated_block_head = nullptr; //分配块首指针

allocated_block *find_process(int id);	   //寻找pid为id的分配块
free_block *init_free_block(int mem_size); //空闲块初始化
void display_menu();					   //显示选项菜单
void set_mem_size();					   //设置内存大小
int allocate_mem(allocated_block *ab);	   //为制定块分配内存
void rearrange();						   //对块进行重新分配
int create_new_process();				   //创建新的进程
int free_mem(allocated_block *ab);		   //释放分配块
void swap(int *p, int *q);				   //交换地址
int dispose(allocated_block *ab);		   //释放分配块结构体
void display_mem_usage();				   //显示内存情况
void kill_process();					   //杀死对应进程并释放其空间与结构体
void Usemy_algo(int id);				   //使用对应的分配算法

void insert_freeblock(free_block *);

void remove_freeblock(free_block *);

void insert_allocatedblock(allocated_block *);

int best_fit_allocation(allocated_block *);

int worst_fit_allocation(allocated_block *);

int first_fit_allocation(allocated_block *);

int buddy_allocation(allocated_block *);

int main()
{
	int op;
	pid = 0;
	free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
	for (;;)
	{
		sleep(1);
		display_menu();
		fflush(stdin);
		scanf("%d", &op);
		switch (op)
		{
		case 1:
		{
			set_mem_size();
			break;
		}
		case 2:
		{
			printf("Choose an algorithm\n");
			printf(" 1: Best Fit\n 2: Worst Fit\n 3: First Fit\n 4: Buddy System\n");
			scanf("%d", &mode);
			if (mode < 1 || mode > 4)
			{
				printf("Wrong arguments, set algorithm as the default `best fit`...");
				mode = 1;
			}
			break;
		}
		case 3:
		{
			create_new_process();
			break;
		}
		case 4:
		{
			kill_process();
			break;
		}
		case 5:
		{
			display_mem_usage();
			break;
		}
		case 233:
		{
			puts("bye....");
			sleep(1);
			return 0;
		}
		default:
			break;
		}
	}
}

allocated_block *find_process(int id)
{ //循环遍历分配块链表，寻找pid=id的进程所对应的块
	allocated_block *tmp = allocated_block_head;
	while (tmp != nullptr)
	{
		if (tmp->pid == id)
			break;
		tmp = tmp->next;
	}
	return tmp;
}

free_block *init_free_block(int mem_size)
{ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
	free_block *p;
	p = (free_block *)malloc(sizeof(free_block));
	if (p == nullptr)
	{
		puts("No memory left");
		return nullptr;
	}
	p->size = mem_size;
	p->start_addr = DEFAULT_MEM_START;
	p->next = nullptr;
	return p;
}

void display_menu()
{
	puts("\n\n******************menu*******************");
	printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
	printf("2) Set memory allocation algorithm\n");
	printf("3) Create a new process\n");
	printf("4) Kill a process\n");
	printf("5) Display memory usage\n");
	printf("233) Exit\n");
}

void set_mem_size()
{ //更改最大内存大小
	if (flag || allocated_block_head != nullptr)
		puts("Cannot set the memory");
	else
	{
		puts("Input the max memory size: ");
		int change_size;
		scanf("%d", &change_size);
		if (change_size > 0)
			free_block_head->size = mem_size = change_size;
	}
}

int allocate_mem(allocated_block *ab)
{ //为块分配内存，真正的操作系统会在这里进行置换等操作
	switch (mode)
	{
	case 2: //worst fit
	{
		return worst_fit_allocation(ab);
	}
	case 3: // first fit
	{
		return first_fit_allocation(ab);
	}
	case 4: // buddy
	{
		return buddy_allocation(ab);
	}
	default: // best fit
	{
		return best_fit_allocation(ab);
	}
	}
}

int create_new_process()
{ //创建新进程
	int mem_sz;
	printf("Please input memory size\n");
	scanf("%d", &mem_sz);
	// Write your code here
	if (free_block_head == nullptr || mem_sz <= 0 || mem_sz > mem_size)
		return -1;
	auto *new_process = new allocated_block();
	new_process->pid = ++pid;
	new_process->size = mem_sz; //TODO: remember to delete
	int code = allocate_mem(new_process);
	if (code == -1)
	{
		printf("No memory to set...");
		delete new_process;
	}
	return code;
}

void swap(int *p, int *q)
{
	int tmp = *p;
	*p = *q;
	*q = tmp;
}

void rearrange()
{ //将free块按照地址大小进行排序
	free_block *tmp, *tmpx;
	puts("Rearrange begins...");
	puts("Rearrange by address...");
	tmp = free_block_head;
	while (tmp != nullptr)
	{
		tmpx = tmp->next;
		while (tmpx != nullptr)
		{
			if (tmpx->start_addr < tmp->start_addr)
			{
				swap(&tmp->start_addr, &tmpx->start_addr);
				swap(&tmp->size, &tmpx->size);
			}
			tmpx = tmpx->next;
		}
		tmp = tmp->next;
	}
	usleep(500);
	puts("Rearrange Done.");
}

int free_mem(allocated_block *ab)
{ //释放某一块的内存
	if (ab == nullptr)
		return -1;
	free_block *before = nullptr, *after = nullptr;
	for (free_block *f = free_block_head; f != nullptr; f = f->next)
	{
		if (f->start_addr + f->size == ab->start_addr)
		{
			before = f;
			break;
		}
	}
	for (free_block *f = free_block_head; f != nullptr; f = f->next)
	{
		if (ab->start_addr + ab->size == f->start_addr)
		{
			after = f;
			break;
		}
	}
	if (before != nullptr && after != nullptr)
	{
		before->size = before->size + ab->size + after->size;
		remove_freeblock(after);
	}
	else if (before != nullptr && after == nullptr)
	{
		before->size += ab->size;
	}
	else if (before == nullptr && after != nullptr)
	{
		after->size += ab->size;
		after->start_addr = ab->start_addr;
	}
	else
	{
		auto *new_free = new free_block();
		new_free->start_addr = ab->start_addr;
		new_free->size = ab->size;
		ab->next = nullptr;
		insert_freeblock(new_free);
	}
	return 1;
}

int dispose(allocated_block *fab)
{ //释放结构体所占的内存
	allocated_block *pre, *ab;
	if (fab == allocated_block_head)
	{
		allocated_block_head = allocated_block_head->next;
		free(fab);
		return 1;
	}
	pre = allocated_block_head;
	ab = allocated_block_head->next;
	while (ab != fab)
	{
		pre = ab;
		ab = ab->next;
	}
	pre->next = ab->next;
	free(ab);
	return 2;
}

void display_mem_usage()
{
	free_block *fb = free_block_head;
	allocated_block *ab = allocated_block_head;
	puts("*********************Free Memory*********************");
	printf("%20s %20s\n", "start_addr", "size");
	int cnt = 0;
	while (fb != nullptr)
	{
		cnt++;
		printf("%20d %20d\n", fb->start_addr, fb->size);
		fb = fb->next;
	}
	if (!cnt)
		puts("No Free Memory");
	else
		printf("Totaly %d free blocks\n", cnt);
	puts("");
	puts("*******************Used Memory*********************");
	printf("%10s %10s %20s\n", "PID", "start_addr", "size");
	cnt = 0;
	while (ab != nullptr)
	{
		cnt++;
		printf("%10d %10d %20d\n", ab->pid, ab->start_addr, ab->size);
		ab = ab->next;
	}
	if (!cnt)
		puts("No allocated block");
	else
		printf("Totaly %d allocated blocks\n", cnt);
}

void kill_process()
{ //杀死某个进程
	allocated_block *ab;
	int pid;
	puts("Please input the pid of Killed process");
	scanf("%d", &pid);
	ab = find_process(pid);
	if (ab != nullptr)
	{
		free_mem(ab);
		dispose(ab);
	}
}

void insert_freeblock(free_block *new_free)
{
	new_free->next = free_block_head;
	free_block_head = new_free;
}

void remove_freeblock(free_block *to_remove)
{
	if (to_remove == free_block_head)
		free_block_head = free_block_head->next;
	else
	{
		for (free_block *f = free_block_head; f != nullptr; f = f->next)
		{
			if (to_remove == f->next)
			{
				f->next = to_remove->next;
				break;
			}
		}
	}
	delete to_remove;
}

void insert_allocatedblock(allocated_block *ab)
{
	ab->next = allocated_block_head;
	allocated_block_head = ab;
}

int best_fit_allocation(allocated_block *ab)
{
	free_block *tmp = nullptr;
	for (free_block *fb = free_block_head; fb != nullptr; fb = fb->next)
	{
		if ((fb->size >= ab->size) && (tmp == nullptr || tmp->size > fb->size))
		{
			tmp = fb;
		}
	}
	if (tmp == nullptr)
		return -1;
	ab->start_addr = tmp->start_addr;
	if (tmp->size == ab->size)
		remove_freeblock(tmp);
	else
	{
		tmp->size -= ab->size;
		tmp->start_addr += ab->size;
	}
	insert_allocatedblock(ab);
	return 1;
}

int worst_fit_allocation(allocated_block *ab)
{
	int max_size = -1;
	free_block *tmp = nullptr;
	for (free_block *fb = free_block_head; fb != nullptr; fb = fb->next)
	{
		if (fb->size >= ab->size && fb->size > max_size)
		{
			tmp = fb;
			max_size = fb->size;
		}
	}
	if (tmp == nullptr)
		return -1;
	ab->start_addr = tmp->start_addr;
	if (tmp->size == ab->size)
		remove_freeblock(tmp);
	else
	{
		tmp->size -= ab->size;
		tmp->start_addr += ab->size;
	}
	insert_allocatedblock(ab);
	return 1;
}

int first_fit_allocation(allocated_block *ab)
{
	// rearrange(); //TODO: 可以更改位置, 我觉得没必要用这个方法
	free_block *tmp = nullptr;
	for (free_block *fb = free_block_head; fb != nullptr; fb = fb->next)
	{
		if (fb->size >= ab->size)
		{
			if (tmp == nullptr || fb->start_addr > tmp->start_addr)
				tmp = fb;
		}
	}
	if (tmp == nullptr)
		return -1;
	ab->start_addr = tmp->start_addr;
	if (tmp->size == ab->size)
		remove_freeblock(tmp);
	else
	{
		tmp->size -= ab->size;
		tmp->start_addr += ab->size;
	}
	insert_allocatedblock(ab);
	return 1;
}

int buddy_allocation(allocated_block *ab)
{
}