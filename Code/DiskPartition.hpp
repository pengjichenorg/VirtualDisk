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
    DiskPartition(std::string root, void* startPtr);

    /**
     * destructor
     */
    ~DiskPartition();

    std::shared_ptr<NTree<File>>& getNTree(std::shared_ptr<File> file);

private:
    std::shared_ptr<NTree<File>> m_ntreeRootPtr;
    void* m_startPtr = nullptr;

};

DiskPartition::DiskPartition() {

}

DiskPartition::DiskPartition(std::string root, void* startPtr) {
    std::shared_ptr<File> treeInfo(new File(root, FileType::directoryFile));
    m_ntreeRootPtr = std::make_shared<NTree<File>>(treeInfo);
    m_ntreeRootPtr->setNTreeInfo(treeInfo);

    m_startPtr = startPtr;
}

DiskPartition::~DiskPartition() {
    free(m_startPtr);
}

std::shared_ptr<NTree<File>>& DiskPartition::getNTree(std::shared_ptr<File> file = nullptr) {
    // if(file == nullptr) {
        return m_ntreeRootPtr;
    // }
    // for(auto ntree : )
    // return nullptr;
}
