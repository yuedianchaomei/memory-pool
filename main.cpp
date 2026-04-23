#include "MemoryPool.h"
#include <iostream>
#include <vector>
#include <chrono>

const int POOLSIZE = 64;
const int POOLNUMS = 1000000;
const int TESTTIMES = 1000000;

void testUsePool()
{
    MemoryPool pool(POOLNUMS, POOLSIZE);
    std::vector<void*> ptrs(1000, nullptr);

    std::cout << "内存池块数：" << pool.getFreeBlockNums() << std::endl;
    for(int i = 0; i < 1000; ++i)
    {
        ptrs[i] = pool.allocate();
    }
    std::cout << "分配后内存池块数：" << pool.getFreeBlockNums() << std::endl;
    for(int i = 0; i < 100; ++i)
    {
        pool.deallocate(ptrs[i]);
    }
    std::cout << "归还100块后内存池块数：" << pool.getFreeBlockNums() << std::endl;
}

void testNewDeleteTime()
{
    std::vector<void*> ptrs(POOLNUMS, nullptr);

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < POOLNUMS; ++i)
    {
        ptrs[i] = new char[POOLSIZE];
    }
    
    for(int i = 0; i < POOLNUMS; ++i)
    {
        delete[] static_cast<char*>(ptrs[i]);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "new/delete耗时：" << dur.count() << "ms。" << std::endl;
}

void testPoolTime()
{
    MemoryPool pool(POOLNUMS, POOLSIZE);
    std::vector<void*> ptrs(POOLNUMS, nullptr);

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < POOLNUMS; ++i)
    {
        ptrs[i] = pool.allocate();
        if (!ptrs[i])
        {
            std::cout << "内存池不足" << std::endl;
        }
    }

    for(int i = 0; i < POOLNUMS; ++i)
    {
        pool.deallocate(ptrs[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "MemoryPool耗时：" << dur.count() << "ms。" << std::endl;
}

int main()
{
    std::cout << "===== 测试：MemoryPool的使用方法 =====" << std::endl;
    testUsePool();

    std::cout << "===== 测试：new/delete和MemoryPool耗时对比 =====" << std::endl;
    testNewDeleteTime();
    testPoolTime();

    return 0;
}