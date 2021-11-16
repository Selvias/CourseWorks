#include "avltree.hpp"
#include <ctime>
#include <iomanip>

const unsigned int count = 5;
const unsigned int delay = 10000;

int main()
{
    // srand(time(NULL));

    char *str = (char *)"string";
    avl *root = create_tree(rand() % 1000, str);

    for (int i = 1; i < int(count); i++)
    {
        root = avltree_add(root, i, str);

        if (i % delay == 0)
        {
            clock_t start = clock();
            lookup(root, rand() % i - 1);
            clock_t end = clock();
            std::cout << std::fixed << std::setprecision(8); 
            double t = double((double)(end - start) / CLOCKS_PER_SEC);
            std::cout << "Time (i = " << i << ") : " << t << std::endl;
            std::cout << "HEIGHT : " << root->height << " KEY : " << (min(root))->key << std::endl;
            std::cout << "MIN : " << (min(root))->key << std::endl;
            std::cout << "MAX : " << (max(root))->key << std::endl;

            std::cout << "ROOT HEIGHT : " << root->height << std::endl;
        }

    }   

    tree_out(root, 0);
    std::cout << "______________________________\n" << std::endl;

    // delnode(root, 2);
    root = delnode(root, 1);
    root = delnode(root, 3);
    root = delnode(root, 4);

    tree_out(root, 0);

    return 0;
}