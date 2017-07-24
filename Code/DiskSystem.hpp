#pragma once

#include <array>
#include <memory>
#include <unordered_map>

#include "DiskPartition.hpp"

const size_t diskSize = 128;

const int partitionSize = 4;

const std::array<std::string, 10> partitionName = {
    "C:", "D:", "E:", "F:", "G", "H", "I", "J", "K", "L"
};

const std::string initPartition = "C:";

class DiskSystem {
public:
    static DiskSystem* getInstance();
    std::string getCurrentPath();
    std::shared_ptr<NTree<File>> getCurrentPartition();
    void setCurrentNode(std::shared_ptr<NTree<File>> node);
    std::shared_ptr<NTree<File>> getCurrentNode();
    std::shared_ptr<NTree<File>> findNode();

private:
    DiskSystem();
    void calculateCurrentPath();
    static DiskSystem* m_instance;
    std::unordered_map<std::string, DiskPartition> m_partitions;
    std::string m_currentPath;
    std::shared_ptr<NTree<File>> m_currentPartition;
    std::shared_ptr<NTree<File>> m_currentNode;

};

DiskSystem* DiskSystem::m_instance = nullptr;

DiskSystem* DiskSystem::getInstance() {
    if(m_instance == nullptr) {
        m_instance = new DiskSystem();
    }
    return m_instance;
}

std::shared_ptr<NTree<File>> DiskSystem::getCurrentPartition() {
    return m_currentPartition;
}

std::string DiskSystem::getCurrentPath() {
    return m_currentPath;
}

void DiskSystem::setCurrentNode(std::shared_ptr<NTree<File>> node) {
    m_currentNode = node;
    calculateCurrentPath();
}

std::shared_ptr<NTree<File>> DiskSystem::getCurrentNode() {
    return m_currentNode;
}

DiskSystem::DiskSystem() {
    for(auto index = 0; index != partitionSize; index++) {
        void* startPtr = malloc(diskSize);
        m_partitions.emplace(partitionName[index], DiskPartition(partitionName[index], startPtr));
    }
    m_currentNode = m_partitions[initPartition].getNTree();
    m_currentPartition = m_currentNode;
    calculateCurrentPath();
}

void DiskSystem::calculateCurrentPath() {
    NTree<File>* tree = m_currentNode.get();
    do {
        m_currentPath = tree->getNTreeInfo()->getFileName() + m_currentPath;
        for(auto value : tree->getNTreeValue()) {
            m_currentPath = value->getFileName() + " " + m_currentPath;
        }
        tree = tree->getParent();
    } while(tree != nullptr && tree->getNTreeInfo()->getFileName() != std::string("root:"));
}
