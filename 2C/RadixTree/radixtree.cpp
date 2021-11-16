#include "radixtree.h"

int lenn;

  ////////////////////////////////////////////////
 ////////////ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ/////////////
////////////////////////////////////////////////
//Функция, возвращающая часть строки после найденного префикса
char *turnPostfix(char *s1, int n) {
    char *newstr = new char [strlen(s1) - n + 1];
    int j{0};
    for (; s1[n] != '\0'; j++)
    {
        newstr[j] = s1[n];
        n++;
    }
    newstr[j] = '\0';
    return newstr;
}
//Перевод числового значения префикса в строку
char *prefncpy(char *s1, int n) {
    if (!n)
        return nullptr;
    char *newstr = new char [n + 1];
    if (!newstr)
        return nullptr;
    for (int i{0}; i < n; i++)
        newstr[i] = s1[i];
    newstr[n] = '\0';
    return newstr;
}
//Функция поиска префикса в числовом эквиваленте (сколько первых символов совпало)
int findpref2(char *str1, char *str2) {
    if (*str1 != *str2)
        return 0;
    int i = 0;
    while (str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0')
        i++;
    
    return (lenn = i);
}
//Создание дубликата авл-узла с новым split-ключом (Например, если был slowER, станет er)
avl *avlcpy(avl *src) {
    avl *dest = new avl;
    dest->height = 0;
    dest->key = turnPostfix(src->key, lenn);
    dest->left = nullptr;
    dest->right = nullptr;
    dest->value = src->value;
    return dest;
}
//Поиск ребра, имеющего общий префикс с ключом
avl *findEdge(avl *tree, char *key) {
    while (tree)
    {
        int length = findpref2(key, tree->key);
        if (!length)
        {
            if (strcmp(tree->key, key) > 0)
                tree = tree->left;
            else
                tree = tree->right;
        }
        else
            return tree;
    }
    return tree;
}

//Первой при конкатенации идёт строка 1, затем строка 2
char *strconcat(char *str1, char *str2) {
    int length = strlen(str1) + strlen(str2) + 1;
    char *united = new char [length];
    int i;

    for (i = 0; str1[i] != '\0'; i++)
        united[i] = str1[i];

    for (int j{0}; str2[j] != '\0'; j++) {
        united[i] = str2[j];
        i++;
    }
    united[i] = '\0';
    return united;
}

  ////////////////////////////////////////////////
 ////////////////ОСНОВНЫЕ ФУНКЦИИ////////////////
////////////////////////////////////////////////
radnode *create() {
    radnode *node = new radnode;
    if (!node)
        return nullptr;
    node->value = 0;
    node->tree = nullptr;
    node->isLeaf = false;
    return node;
}

radnode *insertRadix(radnode *root, char *key, int value, int keypos) {
    radnode *node = root, *list;
    if (*key != '\0')
    {
        avl *anode = findEdge(root->tree, key);
        if (!anode)
        {
            node->tree = avltree_add(node->tree, key, list = create());
            node = list;
            node->isLeaf = true;
            node->value = value;
            lenn = 0;
        }
        else 
        {
            //Создание текстовой версии префикса
            char *pref = prefncpy(key, lenn);
            //Обработка случая, когда ключ и связка совпадают
            if (strcmp(key, anode->key) == 0)
            {
                node = (radnode*)anode->value;
                if (!(node->isLeaf))
                {
                    node->isLeaf = true;
                    node->value = value;
                    lenn = 0;
                }
                return node;
            }
            //Case 1. Вставка длиннее префикса, но префикс полностью соответствует 
            //первой части вставки.
            else if (strcmp(pref, anode->key) == 0)
            {
                node = (radnode*)anode->value;
                keypos = keypos + lenn;
                lenn = 0;
                insertRadix(node, key + keypos, value);
            }
            //Case 2. Префикс длиннее вставки, но вставка полностью совпадает с 
            //первой частью префикса
            else if (strcmp(pref, key) == 0)
            {
                avl *temp = avlcpy(anode);
                node->tree = delnode(node->tree, anode->key);
                node->tree = avltree_add(node->tree, key, list = create());
                node = list;
                node->isLeaf = true;
                node->value = value;
                node->tree = temp;
                lenn = 0;
            }
            //Case 3. У вставки и связки есть общий префикс, но полного совпадения нет.
            else
            {
                avl *temp = avlcpy(anode);
                node->tree = delnode(node->tree, anode->key);
                node->tree = avltree_add(node->tree, pref, list = create());
                node = list;
                node->tree = temp;
                keypos = keypos + lenn;
                lenn = 0;
                node->tree = avltree_add(node->tree, key + keypos, list = create());
                node = list;
                node->isLeaf = true;
                node->value = value;
            }
        }
    }

    return root;
}

radnode *removeRadix(radnode *root, char *key) {
    radnode *node = root;

    if (*key == '\0')
    {
        //Если удаляемый узел найден
        if (node->isLeaf)
        {
            node->isLeaf = false;
            lenn = 0;
            return node;
        }
        //Если ключ закончился, но префикс не является ключевым
        //т.е. такого ключа в дереве нет.
        else 
            return nullptr;
    }
    else if (*key != '\0')
    {
        avl *anode = findEdge(node->tree, key);
        if (!anode)
            return nullptr;
        else
        {
            key = turnPostfix(key, lenn);
            lenn = 0;
            radnode *next = removeRadix((radnode*)anode->value, key);
            //Если найден узел, который нужно удалить
            if (next)
            {
                if (!next->isLeaf)
                {
                    //Вернувшийся узел содержит только одно ребро
                    if (next->tree && !next->tree->left && !next->tree->right)
                    {
                        char *cnct;
                        //Создаётся новый узел АВЛ дерева, в который записывается
                        //объединение префиксов radix tree. (схлопывание узлов)
                        node->tree = avltree_add(node->tree, cnct = strconcat(anode->key, next->tree->key), next->tree->value);
                        node->tree = delnode(node->tree, anode->key);
                        delnode(next->tree, next->tree->key);
                        delete next;
                    }
                    //Удаляемый узел - листовой
                    else if (!next->tree)
                    {
                        node->tree = delnode(node->tree, anode->key);
                        delete next;
                        lenn = 0;
                        return node;
                    }
                }
            }
            else
            //Если удалённый узел не найден
                return nullptr;
        }
    }
    lenn = 0;
    return node;
}

  ////////////////////////////////////////////////
 //////////////////////ПОИСК/////////////////////
////////////////////////////////////////////////
radnode *lookupRadix(radnode *root, char *key, int keypos) {
    if (*key == '\0')
    {
        if (root->isLeaf)
            return root;
        else
            return nullptr;
    }
    else
    {
        avl *anode = findEdge(root->tree, key);
        if (!anode)
            return nullptr;
        keypos = keypos + lenn;
        lenn = 0;
        return lookupRadix((radnode*)anode->value, key+keypos);
    }
}

radnode *findminRadixNode(radnode *root) {
    if (!root)
        return nullptr;
    if (!root->tree)
        return root;
    avl *minstr = min(root->tree);
    return findminRadixNode((radnode*)minstr->value);
}

radnode *findmaxRadixNode(radnode *root) {
    if (!root)
        return nullptr;
    if (!root->tree)
        return root;
    avl *minstr = max(root->tree);
    return findmaxRadixNode((radnode*)minstr->value);
}

char *findminRadix(radnode *root) {
    if (!root)
        return nullptr;
    if (!root->tree)
        return (char*)"";
    avl *minstr = min(root->tree);
    return strconcat(minstr->key, findminRadix((radnode*)minstr->value));
}

char *findmaxRadix(radnode *root) {
    if (!root)
        return nullptr;
    if (!root->tree)
        return (char*)"";
    avl *minstr = max(root->tree);
    return strconcat(minstr->key, findmaxRadix((radnode*)minstr->value));
}