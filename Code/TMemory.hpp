#pragma once

#include <cstdlib>

const size_t diskSize = 128;

class TMemory {
public:
    TMemory();
    void* m_startPtr = nullptr;
};

TMemory::TMemory() {
    m_startPtr = malloc(diskSize);
}
