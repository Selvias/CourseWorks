#pragma once
#include "avl/avltree.hpp"

  ////////////////////////////////////////////////
 //////////////////////NODE//////////////////////
////////////////////////////////////////////////
struct radnode {
    int value;
    //Хранилище частей ключа и указателей на узлы radix tree
    avl *tree;
    //Переменная-флаг, сигнализирующая о том, что узел radix tree является концом строки некоторого ключа
    bool isLeaf;
};
  ////////////////////////////////////////////////
 ////////////ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ/////////////
////////////////////////////////////////////////
//Функция, возвращающая часть строки после найденного префикса
char *turnPostfix(char *s1, int n);
//Перевод числового значения префикса в строку
char *prefncpy(char *s1, int n);
//Функция поиска префикса в числовом эквиваленте (сколько первых символов совпало)
int findpref2(char *str1, char *str2);
//Создание дубликата авл-узла с новым split-ключом (Например, если был slowER, станет er)
avl *avlcpy(avl *src);
//Поиск ребра, имеющего общий префикс с ключом
avl *findEdge(avl *tree, char *key);
//Первой при конкатенации идёт строка 1, затем строка 2
char *strconcat(char *str1, char *str2);
  ////////////////////////////////////////////////
 ////////////////ОСНОВНЫЕ ФУНКЦИИ////////////////
////////////////////////////////////////////////
//Создание узла дерева Radix
radnode *create();
//Вставка в дерево Radix
radnode *insertRadix(radnode *root, char *key, int value, int keypos = 0);
//Удаление из Radix Tree
radnode *removeRadix(radnode *root, char *key);
  ////////////////////////////////////////////////
 //////////////////////ПОИСК/////////////////////
////////////////////////////////////////////////
radnode *lookupRadix(radnode *root, char *key, int keypos = 0);

char *findminRadix(radnode *root);
char *findmaxRadix(radnode *root);

radnode *findminRadixNode(radnode *root);
radnode *findmaxRadixNode(radnode *root);