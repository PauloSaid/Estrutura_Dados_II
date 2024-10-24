// ALUNO: PAULO ANDRÉ RIBEIRO SAID
//        CC6M

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node {
  int key;    // Key of the node (value in the treap)
  int prior;  // Priority assigned to the node
  struct Node *left;   // Pointer to the left child
  struct Node *right;  // Pointer to the right child
};

// Creates a new node with the given key
struct Node *CreateNode(int key) {
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));

  if (node != NULL) {
    node->key = key;
    node->prior = rand() % 100;  // Assigns a random priority between 0 and 99
    node->left = NULL;
    node->right = NULL;
  }

  return node;
}

// Performs a left rotation on the node
struct Node *LeftRotation(struct Node *node) {
    struct Node *aux = node->right; // aux points to the right child (higher priority)
    struct Node *aux2 = aux->left;  // aux2 points to the left subtree of aux
    
    aux->left = node;    // Rotates the node to the left
    node->right = aux2;  // Updates right child of node
    
    return aux;          // aux becomes the new root
}

// Performs a right rotation on the node
struct Node *RightRotation(struct Node *node) {
    struct Node *aux = node->left;  // aux points to the left child (higher priority)
    struct Node *aux2 = aux->right; // aux2 points to the right subtree of aux
    
    aux->right = node;   // Rotates the node to the right
    node->left = aux2;   // Updates left child of node
    
    return aux;          // aux becomes the new root
}

// Inserts a new node with the given key in the treap
struct Node *InsertTreap(struct Node *root, int key) {
  if (root == NULL) {
    return CreateNode(key);  // Creates a new node if the tree is empty
  }

  // Inserting in the left subtree if the key is smaller
  if (root->key > key) {
    root->left = InsertTreap(root->left, key);
    // Rotates if the left child has higher priority
    if (root->left != NULL && root->left->prior > root->prior)
        root = RightRotation(root);
  }
  // Inserting in the right subtree if the key is greater
  else if (root->key < key) {
    root->right = InsertTreap(root->right, key);
    // Rotates if the right child has higher priority
    if (root->right != NULL && root->right->prior > root->prior)
        root = LeftRotation(root);
  }

  return root;  // Returns the updated root
}

// Removes a node from the treap and maintains heap property
struct Node *RemoveNode(struct Node *node) {
  struct Node *aux;
  
  // If the node has no left child, return the right child
  if (node->left == NULL)
    aux = node->right;
  // If the node has no right child, return the left child
  else if (node->right == NULL)
    aux = node->left;
  else {
    // Rotates based on the priority and recursively removes the node
    if (node->left->prior > node->right->prior) {
        node = RightRotation(node);
        node->right = RemoveNode(node->right);
    } else {
        node = LeftRotation(node);
        node->left = RemoveNode(node->left);
    }
    return node;
  }

  free(node);  // Frees the memory of the node
  return aux;  // Returns the new subtree root
}

// Removes a node with the given key from the treap
struct Node *RemoveTreap(struct Node *root, int key) {
    if (root == NULL) return root;  // If the tree is empty, return NULL
    
    if (root->key > key)   // Search in the left subtree
        root->left = RemoveTreap(root->left, key);
    else if (root->key < key)  // Search in the right subtree
        root->right = RemoveTreap(root->right, key);
    else
        return RemoveNode(root);  // If the key matches, remove the node
    
    return root;  // Returns the updated root
}

// Prints the treap in a structured format (in-order)
void PrintTree(struct Node *node, int lvl) {
    if (node != NULL) {
        // Indents according to the tree level
        for (int i = 0; i < lvl; i++)
            printf("  ");
        
        // Prints the key and priority of the node
        printf("%d(%d)\n ", node->key, node->prior);
        
        // Recursively prints the left and right children
        PrintTree(node->left, lvl + 1);
        PrintTree(node->right, lvl + 1);
    }
}

int main()
{
    srand(time(NULL));  // Seed for random number generator
    
    struct Node *root = NULL;  // Initializes an empty treap
    
    int v[6] = {10, 20, 5, 30, 36, 40};  // Values to insert into the treap
    
    // Inserts values into the treap
    for (int i = 0; i < 6; i++) {
        root = InsertTreap(root, v[i]);
    }
    
    // Prints the treap structure
    PrintTree(root, 0);
    
    // Removes the node with key 5
    root = RemoveTreap(root, 5);
  
    // Prints the updated treap structure
    PrintTree(root, 0);

    return 0;
}