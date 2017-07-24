#include "DiskSystem.hpp"

#include "NTree.hpp"
#include "File.hpp"

#include "VDos.hpp"

#include <cstdlib>

void testFileInNTree() {
    // /
    std::shared_ptr<File> treeInfo_Root(new File("root:", FileType::directoryFile));
    std::shared_ptr<NTree<File>> ntree(new NTree<File>(treeInfo_Root));

    // C:
    std::shared_ptr<File> treeInfo_C(new File("C:", FileType::directoryFile));
    std::shared_ptr<NTree<File>> ntree_C(new NTree<File>(treeInfo_C));

    // C:\A
    std::shared_ptr<File> treeInfo_C_A(new File("\\A", FileType::directoryFile));
    std::shared_ptr<NTree<File>> ntree_C_A(new NTree<File>(treeInfo_C_A));
    ntree_C->addNTreeChild(ntree_C_A);
    // C:1.txt
    std::shared_ptr<File> treeInfo_C_1TXT(new File("1.txt"));
    ntree_C->addNTreeValue(treeInfo_C_1TXT);
    // C:\B
    std::shared_ptr<File> treeInfo_C_B(new File("\\B", FileType::directoryFile));
    std::shared_ptr<NTree<File>> ntree_C_B(new NTree<File>(treeInfo_C_B));
    ntree_C->addNTreeChild(ntree_C_B);

    ntree->addNTreeChild(ntree_C);

    // D:
    std::shared_ptr<File> treeInfo_D(new File("D:", FileType::directoryFile));
    std::shared_ptr<NTree<File>> ntree_D(new NTree<File>(treeInfo_D));

    // D:\C
    std::shared_ptr<File> treeInfo_D_C(new File("\\C", FileType::directoryFile));
    std::shared_ptr<NTree<File>> ntree_D_C(new NTree<File>(treeInfo_D_C));
    ntree_D->addNTreeChild(ntree_D_C);
    // D:\E
    std::shared_ptr<File> treeInfo_D_E(new File("\\E", FileType::directoryFile));
    std::shared_ptr<NTree<File>> ntree_D_E(new NTree<File>(treeInfo_D_E));
    ntree_D->addNTreeChild(ntree_D_E);

    ntree->addNTreeChild(ntree_D);

    // E:
    std::shared_ptr<File> treeInfo_E(new File("E:", FileType::directoryFile));
    std::shared_ptr<NTree<File>> ntree_E(new NTree<File>(treeInfo_E));

    // E:2.txt
    std::shared_ptr<File> treeInfo_E_2TXT(new File("2.txt"));
    ntree_E->addNTreeValue(treeInfo_E_2TXT);
    // E:3.txt
    std::shared_ptr<File> treeInfo_E_3TXT(new File("3.txt"));
    ntree_E->addNTreeValue(treeInfo_E_3TXT);

    ntree->addNTreeChild(ntree_E);

    // F:
    std::shared_ptr<File> treeInfo_F(new File("F:", FileType::directoryFile));
    std::shared_ptr<NTree<File>> ntree_F(new NTree<File>(treeInfo_F));

    ntree->addNTreeChild(ntree_F);

    traversalNTree(ntree);

    NTree<File>* tree = ntree_D_C.get();
    std::cout << "traversal from D:\\C" << std::endl;
    std::string path;
    do {
        path = tree->getNTreeInfo()->getFileName() + path;
        for(auto value : tree->getNTreeValue()) {
            path = value->getFileName() + " " + path;
        }
        tree = tree->getParent();
    } while(tree->getNTreeInfo()->getFileName() != std::string("root:"));
    std::cout << path << std::endl;
}

int main(int argc, char const *argv[]) {
    VDos vdos;
    vdos.run();
    return 0;
}
