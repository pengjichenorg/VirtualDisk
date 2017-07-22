#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cassert>

template <typename T>
class NTree /* : public std::enable_shared_from_this<NTree<T>> */ {
public:
    /**
     * empty constructor
     */
    NTree(std::shared_ptr<T> info);

    /**
     * set this tree's info
     * @param info read
     */
    void setNTreeInfo(std::shared_ptr<T> info);

    /**
     * get this tree's info
     * @return read
     */
    const std::shared_ptr<T> getNTreeInfo() const;

    /**
     * set this tree's parent
     * @param parent read
     */
    // TODO: use weak_ptr
    void setNTreeParent(NTree* parent);

    /**
     * get this tree's parent
     * @param value read
     */
    const NTree* getParent() const;

    /**
     * add this tree's value
     * @param value read
     */
    void addNTreeValue(std::shared_ptr<T> value);

    /**
     * remove this tree's value
     * @param value read
     */
    void removeNTreeValue(std::shared_ptr<T> value);

    /**
     * get tree value vector
     */
    const std::vector<std::shared_ptr<T>>& getNTreeValue() const;

    /**
     * add this tree's child
     * @param child read
     */
    void addNTreeChild(std::shared_ptr<NTree> child);

    /**
     * remove this tree's child
     * @param child read
     */
    void removeNTreeChild(std::shared_ptr<NTree> child);

    /**
     * get children vector
     *
     */
    const std::vector<std::shared_ptr<NTree>>& getChildren() const;

    /**
     * operator== with const lvalue
     * need type T provide operator== function
     */
    bool operator== (const NTree& ntree);

    /**
     * operator== with const shared_ptr
     * need type T provide operator== function
     */
    bool operator== (const std::shared_ptr<NTree> ntree);

private:
    std::shared_ptr<T> m_treeInfo = nullptr;
    NTree* m_parent = nullptr;
    std::vector<std::shared_ptr<T>> m_treeValue;
    std::vector<std::shared_ptr<NTree>> m_children;

};

template <typename T>
NTree<T>::NTree(std::shared_ptr<T> treeInfo) {
    m_treeInfo = treeInfo;
}

template <typename T>
void NTree<T>::setNTreeInfo(std::shared_ptr<T> treeInfo) {
    m_treeInfo = treeInfo;
}

template <typename T>
const std::shared_ptr<T> NTree<T>::getNTreeInfo() const {
    return m_treeInfo;
}

template <typename T>
void NTree<T>::setNTreeParent(NTree* parent) {
    m_parent = parent;
}

template <typename T>
const NTree<T>* NTree<T>::getParent() const {
    return m_parent;
}

template <typename T>
void NTree<T>::addNTreeValue(std::shared_ptr<T> value) {
    m_treeValue.push_back(value);
}

template <typename T>
void NTree<T>::removeNTreeValue(std::shared_ptr<T> value) {
    m_treeValue.erase(std::find(m_treeValue.begin(), m_treeValue.end(), value));
}

template <typename T>
const std::vector<std::shared_ptr<T>>& NTree<T>::getNTreeValue() const {
    return m_treeValue;
}

template <typename T>
void NTree<T>::addNTreeChild(std::shared_ptr<NTree> child) {
    child->setNTreeParent(this);
    m_children.push_back(child);
}

template <typename T>
void NTree<T>::removeNTreeChild(std::shared_ptr<NTree<T>> child) {
    m_children.erase(std::find(m_children.begin(), m_children.end(), child));
}

template <typename T>
const std::vector<std::shared_ptr<NTree<T>>>& NTree<T>::getChildren() const {
    return m_children;
}

template <typename T>
bool NTree<T>::operator==(const NTree<T>& ntree) {
    return *m_treeInfo == *ntree.m_treeInfo;
}

template <typename T>
bool NTree<T>::operator== (const std::shared_ptr<NTree<T>> ntree) {
    return *m_treeInfo == *(ntree->m_treeInfo);
}

template <typename T>
void traversalNTree(std::shared_ptr<NTree<T>> ntree) {
    std::cout << "tree info:" << std::endl;
    auto ntreeInfoSptr = ntree->getNTreeInfo();
    assert(ntreeInfoSptr != nullptr);
    std::cout << *ntreeInfoSptr << std::endl;
    std::cout << "tree value:" << std::endl;
    for(auto value : ntree->getNTreeValue()) {
        std::cout << *value << std::endl;
    }
    if(ntree->getChildren().empty()) {
        return;
    }
    for(auto child : ntree->getChildren()) {
        traversalNTree(child);
    }
}

// QUESTION: 1
