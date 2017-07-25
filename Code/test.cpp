// #include "DiskSystem.hpp"

#include "NTree.hpp"
#include "File.hpp"
#include "VDos.hpp"

#include <cassert>
#include <cstdlib>
#include <functional>

void testFileInNTree() {
    // root:
    NTree<File>* dirRootNTree = new NTree<File>(File("root:", FileType::directoryFile));

    // C:
    NTree<File>* dirCNTree = new NTree<File>(File("C:", FileType::directoryFile));
    dirRootNTree->addNTreeChild(dirCNTree);

    // C:\A
    NTree<File>* dirC_ANTree = new NTree<File>(File("\\A", FileType::directoryFile));
    dirCNTree->addNTreeChild(dirC_ANTree);

    // C:1.txt
    NTree<File>* dirC_1TXTNode = new NTree<File>(File("1.txt"));
    dirCNTree->addNTreeChild(dirC_1TXTNode);

    // C:\B
    NTree<File>* dirC_BNTree = new NTree<File>(File("\\B", FileType::directoryFile));
    dirCNTree->addNTreeChild(dirC_BNTree);

    // D:
    NTree<File>* dirDNTree = new NTree<File>(File("D:", FileType::directoryFile));
    dirRootNTree->addNTreeChild(dirDNTree);

    // D:\C
    NTree<File>* dirD_CNTree = new NTree<File>(File("\\C", FileType::directoryFile));
    dirDNTree->addNTreeChild(dirD_CNTree);

    // D:\D
    NTree<File>* dirD_DNTree = new NTree<File>(File("\\D", FileType::directoryFile));
    dirDNTree->addNTreeChild(dirD_DNTree);

    // D:\E
    NTree<File>* dirD_ENTree = new NTree<File>(File("\\E", FileType::directoryFile));
    dirDNTree->addNTreeChild(dirD_ENTree);

    // E:
    NTree<File>* dirENTree = new NTree<File>(File("E:", FileType::directoryFile));
    dirRootNTree->addNTreeChild(dirENTree);

    // E:2.txt
    NTree<File>* dirE_2TXTNode = new NTree<File>(File("2.txt"));
    dirENTree->addNTreeChild(dirE_2TXTNode);

    // E:3.txt
    NTree<File>* dirE_3TXTNode = new NTree<File>(File("3.txt"));
    dirENTree->addNTreeChild(dirE_3TXTNode);

    // F:
    NTree<File>* dirFNTree = new NTree<File>(File("F:", FileType::directoryFile));
    dirRootNTree->addNTreeChild(dirFNTree);

    // traversal function
    std::function<void(NTree<File>*)> traversal = [&traversal](NTree<File>* ntree) {
        std::cout << ntree->getNTreeValue() << std::endl;
        if(ntree->getNTreeChildren().empty()) {
            return;
        }
        for(auto child = ntree->getNTreeChildren().begin(); child != ntree->getNTreeChildren().end(); child++) {
            // std::cout << child->first << std::endl;
            traversal(child->second);
        }
    };
    // traversal from root
    traversal(dirRootNTree);

    // get full path from D:\E
    std::string path;
    auto node = dirD_ENTree;
    while(node != nullptr) {
        path = node->getNTreeValue().getFileName() + path;
        node = node->getNTreeParent();
    };
    std::cout << path << std::endl;

    // delete test
    delete dirRootNTree;
}

int main(int argc, char const *argv[]) {
    VDos vdos;
    vdos.run();

    // std::string cmd1 = "cmd f1\\f2";
    // std::string cmd2 = "cmd .\\f1\\f2";
    // std::string cmd3 = "cmd ..\\f1\\f2";
    // std::string cmd4 = "cmd \\f1\\f2";
    // std::cout << cmd1.find("\\") << std::endl;
    // std::cout << cmd2.find(".") << std::endl;
    // std::cout << cmd3.find("..") << std::endl;
    // std::cout << cmd4.find("\\") << std::endl;

    return 0;
}
