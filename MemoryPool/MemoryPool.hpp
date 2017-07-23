#pragma once

#include <memory>

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

}

MemoryPool::~MemoryPool() {

}
