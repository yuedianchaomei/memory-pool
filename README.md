# memory-pool
一个简易内存池

用法：

```C++
MemoryPool pool(BLOCK_NUMS, BLOCK_SIZE);
void* p1 = pool.allocate();
pool.deallocate(p1);
```