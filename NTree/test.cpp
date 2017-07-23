#include "NTree.hpp"
#include "../Timer.hpp"
#include <iostream>
#include <cassert>
#include <typeinfo>

/**
 * test NTree as like as follow:
 * /
 * C: D: E: F:
 * C: A/ 1.txt B/
 * D: C/ D/
 * D:C/: E/
 * E: 2.txt 3.txt
 * F:
 */

int main(int argc, char* argv[])
{
    // /
    std::shared_ptr<int> treeInfo_Root(new int(0));
    std::shared_ptr<NTree<int>> ntree(new NTree<int>(treeInfo_Root));
    ntree->setNTreeInfo(treeInfo_Root);

    // C:
    std::shared_ptr<int> treeInfo_C(new int(1));
    std::shared_ptr<NTree<int>> ntree_C(new NTree<int>(treeInfo_C));
    ntree_C->setNTreeInfo(treeInfo_C);
    // C:A/
    std::shared_ptr<int> treeInfo_C_A(new int(11));
    std::shared_ptr<NTree<int>> ntree_C_A(new NTree<int>(treeInfo_C_A));
    ntree_C->addNTreeChild(ntree_C_A);
    // C:1.txt
    std::shared_ptr<int> treeInfo_C_1TXT(new int(-1));
    ntree_C->addNTreeValue(treeInfo_C_1TXT);
    // C:B/
    std::shared_ptr<int> treeInfo_C_B(new int(12));
    std::shared_ptr<NTree<int>> ntree_C_B(new NTree<int>(treeInfo_C_B));
    ntree_C->addNTreeChild(ntree_C_B);

    ntree->addNTreeChild(ntree_C);

    // D:
    std::shared_ptr<int> treeInfo_D(new int(2));
    std::shared_ptr<NTree<int>> ntree_D(new NTree<int>(treeInfo_D));
    ntree_D->setNTreeInfo(treeInfo_D);
    // D:C/
    std::shared_ptr<int> treeInfo_D_C(new int(21));
    std::shared_ptr<NTree<int>> ntree_D_C(new NTree<int>(treeInfo_D_C));
    ntree_D->addNTreeChild(ntree_D_C);
    // D:E/
    std::shared_ptr<int> treeInfo_D_E(new int(22));
    std::shared_ptr<NTree<int>> ntree_D_E(new NTree<int>(treeInfo_D_E));
    ntree_D->addNTreeChild(ntree_D_E);

    ntree->addNTreeChild(ntree_D);

    // E:
    std::shared_ptr<int> treeInfo_E(new int(3));
    std::shared_ptr<NTree<int>> ntree_E(new NTree<int>(treeInfo_E));
    ntree_E->setNTreeInfo(treeInfo_E);
    // E:2.txt
    std::shared_ptr<int> treeInfo_E_2TXT(new int(-2));
    ntree_E->addNTreeValue(treeInfo_E_2TXT);
    // E:3.txt
    std::shared_ptr<int> treeInfo_E_3TXT(new int(-3));
    ntree_E->addNTreeValue(treeInfo_E_3TXT);

    ntree->addNTreeChild(ntree_E);

    // F:
    std::shared_ptr<int> treeInfo_F(new int(3));
    std::shared_ptr<NTree<int>> ntree_F(new NTree<int>(treeInfo_F));
    ntree_F->setNTreeInfo(treeInfo_F);

    ntree->addNTreeChild(ntree_F);

    traversalNTree(ntree);

    NTree<int>* tree = ntree_D_C.get();
    std::cout << "traversal from D:C/" << std::endl;
    do {
        std::cout << *(tree->getNTreeInfo()) << std::endl;
        for(auto value : tree->getNTreeValue()) {
            std::cout << *value << std::endl;
        }
        tree = tree->getParent();
    } while(*(tree->getNTreeInfo()) != 0);
    return 0;
}
