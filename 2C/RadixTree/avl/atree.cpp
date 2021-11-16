#include "avltree.hpp"

  ////////////////////////////////////////////////
 ///////////ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ//////////////
////////////////////////////////////////////////
int height(avl *tree) {
    return tree ? tree->height : -1;
}

void fixheight(avl *tree) {
    int h1 = height(tree->left);
    int h2 = height(tree->right);
    tree->height = (h1 > h2 ? h1 : h2) + 1;
}

int bfactor(avl *tree) {
    return height(tree->left) - height(tree->right);
}

avl *fixbalance(avl *tree) {
    fixheight(tree);
    //Перекос влево
    if (bfactor(tree) == 2)
    {
        if (bfactor(tree->left) == -1)
            return avltree_lr_rotate(tree);
        return avltree_right_rotate(tree);
    }
    //Перекос вправо
    if (bfactor(tree) == -2)
    {
        if (bfactor(tree->right) == 1)
            return avltree_rl_rotate(tree);
        return avltree_left_rotate(tree);
    }
    return tree;
}
  ///////////////////////////////////////////////
 //////////////////ПОВОРОТЫ/////////////////////
///////////////////////////////////////////////
avl *avltree_rl_rotate(avl *P) {
    P->right = avltree_right_rotate(P->right);
    return avltree_left_rotate(P);
}

avl *avltree_lr_rotate(avl *P) {
    P->left = avltree_left_rotate(P->left);
    return avltree_right_rotate(P);
}

avl *avltree_left_rotate(avl *tree) {
    avl *right = tree->right;
    tree->right = right->left;
    right->left = tree;
    fixheight(tree);
    fixheight(right);
    return right;
}

avl *avltree_right_rotate(avl *tree) {
    avl *left = tree->left;
    tree->left = left->right;
    left->right = tree;
    fixheight(tree);
    fixheight(left);
    return left;
}

/////////////////////////////////////////////////
/////СОЗДАНИЕ И ДОБАВЛЕНИЕ УЗЛА AVL ДЕРЕВА///////
/////////////////////////////////////////////////

avl *create_tree(char *key, void *value) {
    avl *node = new avl;
    if (node != nullptr)
    {
        if (key == nullptr)
            return nullptr;
        
        node->key = new char [strlen(key) + 1];
        strcpy(node->key, key);
        node->value = value;
        node->left = nullptr;
        node->right = nullptr;
        node->height = 0;
    }
    return node;
}

avl *avltree_add(avl *tree, char *key, void *value) {
    
    if (tree == nullptr)
        return create_tree(key, value);

    if (strcmp(tree->key, key) > 0)
    {
        tree->left = avltree_add(tree->left, key, value);
        if (bfactor(tree) == 2)
        {
            if (strcmp(tree->left->key, key) > 0)
                tree = avltree_right_rotate(tree);
            else
                tree = avltree_lr_rotate(tree);
        }
    }
    else if (strcmp(tree->key, key) < 0)
    {
        tree->right = avltree_add(tree->right, key, value);
        if (bfactor(tree) == -2)
        {
            if (strcmp(tree->right->key, key) < 0)
                tree = avltree_left_rotate(tree);
            else
                tree = avltree_rl_rotate(tree);
        }
    }
    fixheight(tree);
    return tree;
}

 /////////////////////////////////////////////////
 ///////////////ПОИСК В AVL ДЕРЕВЕ///////////////
////////////////////////////////////////////////
avl *lookup(avl *tree, char *key) {
    while (tree != nullptr)
    {
        if (strcmp(tree->key, key) == 0)
            return tree;
        else if (strcmp(tree->key, key) > 0)
            tree = tree->left;
        else
            tree = tree->right;
    }
    return tree;
}

avl* min(avl* tree) {
	return tree->left ? min(tree->left) : tree;
}

avl* max(avl* tree) {
	return tree->right ? max(tree->right) : tree;
}

  /////////////////////////////////////////////////
 //////////////////Удаление узла//////////////////
/////////////////////////////////////////////////

//Возвращает минимальный узел для правого поддерева, для последующего удаления
avl *mindel(avl *tree) {
    //Если справа от мина есть элемент (1 элемент)
    if (tree->left == nullptr)
        return tree->right;
    tree->left = mindel(tree->left);
    return fixbalance(tree);
}

avl *delnode(avl *tree, char *key) {
    if (tree != nullptr)
    {
        if (strcmp(tree->key, key) < 0)
            tree->right = delnode(tree->right, key);
        else if (strcmp(tree->key, key) > 0)
            tree->left = delnode(tree->left, key);
        else if (strcmp(tree->key, key) == 0)
        {
                avl * left = tree->left;
                avl *right = tree->right;
                delete tree;
                
                //Если правого поддерева нет
                if (right == nullptr)
                    return left;
                //Если есть правое поддерево (необходимо найти в нём минимальный элемент)
                avl *rightmin = min(right);
                rightmin->right = mindel(right);
                rightmin->left = left;
                return fixbalance(rightmin);
        }
        return fixbalance(tree);
    }
    return tree;
}

//Полное удаление АВЛ дерева
void free_avl_tree(avl *tree) {
    if (tree == nullptr)
        return;
    
    free_avl_tree(tree->left);
    free_avl_tree(tree->right);
    delete [] tree->key;
    delete tree;
}

  ////////////////////////////////////////////////
 ////////////////ВЫВОД AVL ДЕРЕВА////////////////
////////////////////////////////////////////////

void tree_out(avl* tree,int level)
{
    int i{0};
    if(tree != nullptr)
    {
        tree_out(tree->right,level+3);
        for(;i < level;i++)
        {
            std::cout << "  ";

        }
        std::cout << tree->key << "(" << level << ")\n";
        tree_out(tree->left, level+3);
    }
}
