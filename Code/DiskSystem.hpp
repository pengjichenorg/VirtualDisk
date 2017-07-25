#pragma once

#include <array>
#include <memory>
#include <unordered_map>

#include "NTree.hpp"
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
    static void destoryInstance();
    std::string getCurrentPath();
    void setCurrentNode(NTree<File>* node);
    NTree<File>* getCurrentNode();
    DiskPartition* getCurrentPartition();
    void setCurrentPartition(std::string partition);

    NTree<File>* findDirectory(std::string fileName);

private:
    DiskSystem();
    static DiskSystem* m_instance;
    std::unordered_map<std::string, DiskPartition> m_partitions;
    DiskPartition* m_currentPartition = nullptr;
    NTree<File>* m_currentNode = nullptr;
    NTree<File>* m_rootNode = nullptr;
    std::string m_currentPath;

};

DiskSystem* DiskSystem::m_instance = nullptr;

DiskSystem* DiskSystem::getInstance() {
    if(m_instance == nullptr) {
        m_instance = new DiskSystem();
    }
    return m_instance;
}

void DiskSystem::destoryInstance() {

}

DiskSystem::DiskSystem() {
    // create NTree node for root
    m_rootNode = new NTree<File>(File("root:", FileType::directoryFile));
    for(auto index = 0; index != partitionSize; index++) {
        // create NTree node for parition
        NTree<File>* paritionNode = new NTree<File>(File(partitionName[index], FileType::directoryFile));
        m_rootNode->addNTreeChild(paritionNode);
        // allocate memory
        void* startPtr = malloc(diskSize);
        m_partitions.emplace(partitionName[index], DiskPartition(paritionNode, startPtr));
    }
    m_currentPartition = &m_partitions[initPartition];
    m_currentNode = m_partitions[initPartition].getNTree();
    m_currentPath = m_currentPartition->getFullPath(m_currentNode);
}

std::string DiskSystem::getCurrentPath() {
    m_currentPartition->getFullPath(m_currentNode);
    return m_currentPath;
}

void DiskSystem::setCurrentNode(NTree<File>* node) {
    m_currentNode = node;
}

NTree<File>* DiskSystem::getCurrentNode() {
    return m_currentNode;
}

DiskPartition* DiskSystem::getCurrentPartition() {
    return m_currentPartition;
}

void DiskSystem::setCurrentPartition(std::string partition = initPartition) {
    m_currentPartition = &m_partitions[partition];
}

NTree<File>* DiskSystem::findDirectory(std::string fileName) {

    // if() {

    // }
}
