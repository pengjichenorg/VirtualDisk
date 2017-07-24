#pragma once

#include <map>
#include <string>
#include <iostream>

template <typename T>
class NTree {
public:
    NTree();
    NTree(T value);
    void setNTreeValue(T value);
    T getNTreeValue();
    void setNTreeParent(NTree<T>* ntree);
    NTree<T>* getNTreeParent();
    void addNTreeChild(NTree<T>* ntree);
    void removeNTreeChild(std::string child);
    std::map<std::string, NTree<T>*> getNTreeChildren();

private:
    T m_value;
    NTree<T>* m_parent;
    std::map<std::string, NTree<T>*> m_children;

};

template <typename T>
NTree<T>::NTree() {

}

template <typename T>
NTree<T>::NTree(T value) {
    m_value = value;
}

template <typename T>
void NTree<T>::setNTreeValue(T value) {
    m_value = value;
}

template <typename T>
T NTree<T>::getNTreeValue() {
    return m_value;
}

template <typename T>
void NTree<T>::setNTreeParent(NTree<T>* ntree) {
    m_parent = ntree;
}

template <typename T>
NTree<T>* NTree<T>::getNTreeParent() {
    return m_parent;
}

template <typename T>
void NTree<T>::addNTreeChild(NTree<T>* ntree) {
    ntree->setNTreeParent(this);
    auto key = ntree->getNTreeValue()->getFileName();
    m_children.insert(std::pair<std::string, NTree<T>*>(key, ntree));
}

template <typename T>
std::map<std::string, NTree<T>*> NTree<T>::getNTreeChildren() {
    return m_children;
}
