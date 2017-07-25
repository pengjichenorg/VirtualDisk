#pragma once

#include <memory>
#include <cstdlib>

#include "NTree.hpp"
#include "File.hpp"

class DiskPartition {
public:
    /*
     * empty constructor
     */
    DiskPartition();

    /*
     * custom constructor
     */
    DiskPartition(NTree<File>* node, void* startPtr);

    /**
     * destructor
     */
    ~DiskPartition();

    NTree<File>* getNTree();

    std::string getFullPath(NTree<File>* node);

    std::string getPartitionName();

    void traversalTest() {
        traversalFunc(m_partitionNode);
    }

    // test
    // traversal function
    void traversalFunc(NTree<File>* ntree) {
        std::cout << ntree->getNTreeValue() << std::endl;
        if(ntree->getNTreeChildren().empty()) {
            return;
        }
        for(auto child = ntree->getNTreeChildren().begin(); child != ntree->getNTreeChildren().end(); child++) {
            traversalFunc(child->second);
        }
    };

private:
    NTree<File>* m_partitionNode = nullptr;
    void* m_startPtr = nullptr;
    std::string m_partitionName;

};

DiskPartition::DiskPartition() {

}

DiskPartition::DiskPartition(NTree<File>* node, void* startPtr) {
    m_partitionNode = node;
    m_startPtr = startPtr;
    m_partitionName = m_partitionNode->getNTreeValue().getFileName();
}

DiskPartition::~DiskPartition() {
    free(m_startPtr);
}

NTree<File>* DiskPartition::getNTree() {
    return m_partitionNode;
}

std::string DiskPartition::getFullPath(NTree<File>* node) {
    std::string path;
    while(node != m_partitionNode) {
        path = node->getNTreeValue().getFileName() + path;
        node = node->getNTreeParent();
    };
    // for parition node
    path = node->getNTreeValue().getFileName() + path;
    return path;
}

std::string DiskPartition::getPartitionName() {
    return m_partitionName;
}
