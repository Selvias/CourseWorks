#include "radixtree.h"
#include <ctime>
#include <fstream>
#include <iomanip>

const int MAX{240000};
const int delay{20000};
//Множитель для увеличения времени поиска в n раз
const int mult{1000000};

int main() {

    radnode *root = create();
    // insertRadix(root, (char*)"slow", 1);
    // //Case 1
    // insertRadix(root, (char*)"slower", 2);
    // //Case 2
    // insertRadix(root, (char*)"slo", 3);

    // //Normal insertion
    // insertRadix(root, (char*)"water", 10);

    // //Case 3
    // insertRadix(root, (char*)"waste", 11);

    // //Mixed Case
    // insertRadix(root, (char*)"watch", 21);

    // insertRadix(root, (char*)"pisos", 22);
    // insertRadix(root, (char*)"portal", 23);
    // insertRadix(root, (char*)"water", 21);
    // insertRadix(root, (char*)"tututu", 24);
    // insertRadix(root, (char*)"abc", 24);

    std::ifstream in("source.txt");
    for (int i{1}; i < MAX; i++)
    {
        char *s = new char[70];
        in.getline(s, 70);
        insertRadix(root, s, i);

        if (!(i%delay))
        {
            radnode *look = nullptr;
            clock_t start = clock();
            for (int j{0}; j < mult; j++)
                look = lookupRadix(root, s);
            clock_t end = clock();
            std::cout << std::fixed << std::setprecision(8); 
            double t = double((double)(end - start) / CLOCKS_PER_SEC);
            std::cout << "Radix Time (i = " << i << "," << s << ") : " << t << " ";

            if (look)
                std::cout << "Key : " << look->value << std::endl;
            else
                std::cout << "NOT FOUND\n";
        }
        delete [] s;
    }

    in.close();

    // radnode *look = lookupRadix3(root, (char*)"slow");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;
    
    // look = lookupRadix3(root, (char*)"slo");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;
    // look = lookupRadix3(root, (char*)"slower");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;
    // look = lookupRadix3(root, (char*)"watch");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;
    // look = lookupRadix3(root, (char*)"waste");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;
    // look = lookupRadix3(root, (char*)"water");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;
    // look = lookupRadix3(root, (char*)"pisos");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;
    // look = lookupRadix3(root, (char*)"portal");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;    
    removeRadix(root, (char*)"the");
    // removeRadix(root, (char*)"watch");
    radnode *look = lookupRadix(root, (char*)"the");
    if (look)
        std::cout << "Lookup Result : " << look->value << std::endl;
    else
        std::cout << "NOT FOUND\n";
    // look = lookupRadix3(root, (char*)"watch");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;
    // else
    //     std::cout << "NOT FOUND\n";
    // look = lookupRadix3(root, (char*)"water");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;
    // look = lookupRadix3(root, (char*)"waste");
    // if (look)
    //     std::cout << "Lookup Result : " << look->value << std::endl;
    std::cout << "MAX : ";
    char *findrstr = findmaxRadix(root);
    if (findrstr)
        std::cout << findrstr;
    radnode *findrstr2 = findmaxRadixNode(root);
    if (findrstr)
        std::cout << "; val : " << findrstr2->value << std::endl;

    std::cout << "MIN : ";
    char *findrstr31 = findminRadix(root);
    if (findrstr)
        std::cout << findrstr31;
    radnode *findrstr3 = findminRadixNode(root);
    if (findrstr)
        std::cout << "; val : " << findrstr3->value << std::endl;    
    return 0;
}