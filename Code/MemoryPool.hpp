#pragma once

#include <memory>
#include "File.hpp"

const size_t memoryPoolSize = 128;

class MemoryPool {
public:
    /**
     * get memory pool instance
     * @return instance
     */
    static MemoryPool* getInstance();

    /**
     * destory memory pool instance
     */
    static void destoryInstance();

private:
    /**
     * private constructor for singleton pattern
     * allocate memory pool
     */
    MemoryPool();

    /**
     * private destructor for singleton pattern
     * release memory pool
     */
    ~MemoryPool();

    static MemoryPool* m_instance;
    std::allocator<File> m_allocator;
    void* m_startPtr = nullptr;
};

MemoryPool* MemoryPool::m_instance = nullptr;

MemoryPool* MemoryPool::getInstance() {
    if(m_instance == nullptr) {
        m_instance = new MemoryPool();
    }
    return m_instance;
}

void MemoryPool::destoryInstance() {
    if(m_instance != nullptr) {
        delete m_instance;
        m_instance = nullptr;
    }
}

MemoryPool::MemoryPool() {
    m_startPtr = m_allocator.allocate(memoryPoolSize);
}

MemoryPool::~MemoryPool() {

}
