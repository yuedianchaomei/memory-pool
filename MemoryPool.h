#pragma once

#include <cstddef>
#include <cstdlib>
#include <stdexcept>

class MemoryPool
{
private:
    size_t mBlockNums;
    size_t mBlockSize;
    size_t mRemainNums;
    char* mPool;
    void* mFreePtr;

public:
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    MemoryPool(size_t, size_t);
    ~MemoryPool();

    void* allocate();
    void deallocate(void*);

    size_t getFreeBlockNums() const;

private:
    void initPool();
};

MemoryPool::MemoryPool(size_t blockNums, size_t blockSize) : mBlockNums(blockNums), mBlockSize(blockSize), mRemainNums(0), mPool(nullptr), mFreePtr(nullptr)
{
    if (blockNums == 0 || blockSize == 0) {
        throw std::invalid_argument("块数量和块大小必须大于0");
    }

    if (mBlockSize < sizeof(void*))
    {
        mBlockSize = sizeof(void*);
    }

    mPool = static_cast<char*>(malloc(mBlockNums * mBlockSize));
    if (mPool == nullptr)
    {
        throw std::bad_alloc();
    }

    mRemainNums = mBlockNums;

    initPool(); 
}

MemoryPool::~MemoryPool()
{
    free(mPool);
}

void MemoryPool::initPool()
{
    char* cur = mPool;

    for(size_t i = 0; i < mBlockNums - 1; ++i)
    {
        *(reinterpret_cast<char**>(cur)) = cur + mBlockSize;
        cur += mBlockSize;
    }
    *(reinterpret_cast<char**>(cur)) = nullptr;

    mFreePtr = mPool;
}

void* MemoryPool::allocate()
{
    if (mFreePtr == nullptr)
    {
        return nullptr;
    }

    void* block = mFreePtr;
    mFreePtr = *(reinterpret_cast<char**>(mFreePtr));
    --mRemainNums;

    return block;
}

void MemoryPool::deallocate(void* block)
{
    if (block == nullptr) {
        throw std::invalid_argument("无法释放空指针");
    }

    if (block < mPool || block >= mPool + mBlockNums * mBlockSize)
    {
        throw std::invalid_argument("无法归还非内存池中的内存");;
    }

    *(reinterpret_cast<void**>(block)) = mFreePtr;
    mFreePtr = block;
    ++mRemainNums;
}

size_t MemoryPool::getFreeBlockNums() const
{
    return mRemainNums;
}