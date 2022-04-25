#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    //new->lower_than = lower_than;
    return NULL;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * newNode = createTreeNode(key, value);
    Pair * p = searchTreeMap(tree, key);
    TreeNode * aux = tree->current;

    if (p != NULL)return;
    if(tree->lower_than(newNode->pair->key, aux->pair->key)){
        tree->current->left = newNode;
        newNode->parent = tree->current;
        tree->current = newNode;
    }
    else{ 
        tree->current->right = newNode;
        newNode->parent = tree->current;
        tree->current = newNode;
    }
}

TreeNode * minimum(TreeNode * x){
    while(x != NULL)
    {
        if(x->left == NULL)return x;
        x = x->left;
    }
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {

    if(node == NULL)return;

    if(node->left == NULL && node->right == NULL){
        if(node == node->parent->right){
            node->parent->right = NULL;
            free(node);
        }
        else{
            node->parent->left = NULL;
            free(node);
        }
    }
    else if(node->right != NULL && node->left != NULL){
        TreeNode * aux = minimum(node->right);
        node->pair->key = aux->pair->key;
        node->pair->value = aux->pair->value;
        removeNode(tree,aux);
    }
    else{
        if (node == node->parent->right){
            if(node->right != NULL){
                node->right->parent = node->parent;
                node->parent->right = node->right;
                free(node);
            }
            else{
                node->left->parent = node->parent;
                node->parent->right = node->left;
                free(node);
            }
        }
        else{
            if(node->right != NULL){
                node->right->parent = node->parent;
                node->parent->left = node->right;
                free(node);
            }
            else{
                node->left->parent = node->parent;
                node->parent->left = node->left;
                free(node);
            }
        }
    }  
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    while(aux != NULL)
    {
        tree->current = aux;
        if(tree->lower_than(key, aux->pair->key))
            aux = aux->left;
        else if (tree->lower_than(aux->pair->key, key))
            aux = aux->right;
        else
        {
            tree->current = aux;
            return aux->pair;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if(tree != NULL)
    {
        if (tree->root != NULL)
        {
            TreeNode * aux = minimum(tree->root);
            searchTreeMap(tree,aux->pair->key);
            return aux->pair;
        }
    }
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    if(tree->current == NULL)return NULL;

    TreeNode * aux = tree->current->parent;
    TreeNode * actual = tree->current;

    if(actual->right != NULL){
        aux = actual->right;
        aux = minimum(aux);
        tree->current = aux;
        return aux->pair;
    }

    while(aux != NULL /*&& aux != tree->root*/)
    {
        if(tree->lower_than(actual->pair->key,aux->pair->key) == 1){
            if(aux->right == NULL){
                tree->current = aux;
                return aux->pair;
            }
            else{
                aux = aux->right;
                aux = minimum(aux);
                tree->current = aux;
                return aux->pair;
            }
        }
        else aux = aux->parent;
    }
    return NULL;
}

/*
Pair * nextTreeMap(TreeMap * tree) {
  TreeNode *aux = tree->current;
  if(aux->right != NULL){
    aux = aux->right;
    aux = minimum(aux);
    tree->current = aux;
    return aux->pair;
  }

  if(aux->right == NULL){
    while(aux != NULL){
      if(aux->parent == NULL)return NULL;
      if(aux == tree->root)return NULL;
      //printf("key: %d\n", (int)aux->pair->key );
      if(tree->lower_than(aux->pair->key, aux->parent->pair->key) == 1){
        aux = aux->parent;
        tree->current = aux;
        return aux->pair;
      }
      //else if(tree->lower_than(aux->pair->key, aux->parent->pair->key) == 0){
        if(aux == tree->root)return NULL;
        tree->current = aux;
        return aux->pair;
      }//
      else{
        //printf("a");
        aux = aux->parent;
        //if(aux == tree->root)return NULL;
      }
    }
  }
  printf("a");
  return NULL;
}
*/