// ALUNO: Paulo André Ribeiro Said
//        CC6M

#include <stdio.h>
#include <stdlib.h>

// Defining a node structure
struct NODE {
    int info;
    struct NODE *left;
    struct NODE *right;
};

// Defining BinTree as a pointer to NODE
typedef struct NODE *BinTree;

// Function to create an empty binary tree
BinTree *create_BinTree() {
    BinTree *root = (BinTree *)malloc(sizeof(BinTree));

    if (root != NULL) {
        *root = NULL; // Initialize the tree as empty
    }

    return root;
}

// Function to insert a value into the binary tree
int insert_BinTree(BinTree *root, int value) {
    if (root == NULL) {
        return 0; // The tree isn't created yet
    }

    struct NODE *new = (struct NODE *)malloc(sizeof(struct NODE));
    if (new == NULL) {
        return 0; // Memory allocation failed
    }

    new->info = value;
    new->right = NULL;
    new->left = NULL;

    if (*root == NULL) {
        *root = new; // If the tree is empty, the new node becomes the root
    } else {
        struct NODE *current = *root;
        struct NODE *previous = NULL;

        while (current != NULL) {
            previous = current;
            if (value == current->info) {
                free(new);
                return 0; // Duplicate values are not allowed
            }

            if (value > current->info) {
                current = current->right;
            } else {
                current = current->left;
            }
        }

        if (value > previous->info) {
            previous->right = new;
        } else {
            previous->left = new;
        }
    }

    return 1;
}

// Function to search for a value in the binary tree
struct NODE* search_BinTree(BinTree *root, int value) {
    if (root == NULL || *root == NULL) {
        return NULL; // Tree is empty or not created
    }

    struct NODE *current = *root;

    while (current != NULL) {
        if (value == current->info) {
            return current; // Value found
        } else if (value > current->info) {
            current = current->right;
        } else {
            current = current->left;
        }
    }

    return NULL; // Value not found
}

// Function to delete a value from the binary tree
void delete_value(int value, BinTree *root) {
    if (root == NULL || *root == NULL) {
        return;
    }

    struct NODE *current = *root;
    struct NODE *parent = NULL;

    while (current != NULL && current->info != value) {
        parent = current;
        if (value < current->info) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (current == NULL) {
        printf("Value %d not found in the tree.\n", value);
        return;
    }

    // Case 1: Node to be deleted has no children (it's a leaf)
    if (current->left == NULL && current->right == NULL) {
        if (parent == NULL) {
            *root = NULL; // Tree had only one node
        } else if (parent->left == current) {
            parent->left = NULL;
        } else {
            parent->right = NULL;
        }
        free(current);
    }
    // Case 2: Node to be deleted has two children
    else if (current->left != NULL && current->right != NULL) {
        struct NODE *successor = current->left;
        struct NODE *successorParent = current;

        while (successor->right != NULL) {
            successorParent = successor;
            successor = successor->right;
        }

        current->info = successor->info;

        if (successorParent->left == successor) {
            successorParent->left = successor->left;
        } else {
            successorParent->right = successor->left;
        }

        free(successor);
    }
    // Case 3: Node to be deleted has only one child
    else {
        struct NODE *child;
        if (current->left != NULL) {
            child = current->left;
        } else {
            child = current->right;
        }

        if (parent == NULL) {
            *root = child; // Deleting the root node with one child
        } else if (parent->left == current) {
            parent->left = child;
        } else {
            parent->right = child; // By Paulo Said 
        }
        free(current);
    }

    printf("The value %d has been deleted.\n", value);
}

// Function to calculate the height of the binary tree
int height(BinTree *root) {
    if (root == NULL)
        return 0;
    if (*root == NULL)
        return -1;

    int height_left = height(&((*root)->left));
    int height_right = height(&((*root)->right));

    if (height_left > height_right)
        return (height_left + 1);
    else
        return (height_right + 1);
}

// Function to count the total elements in the binary tree
int totalElem(BinTree *root) {
    if (root == NULL)
        return 0;
    else if (*root == NULL)
        return 0;

    int total_left = totalElem(&((*root)->left));
    int total_right = totalElem(&((*root)->right));

    return ((total_left + total_right) + 1);
}

// Function to count the number of leaves in the binary tree
int countLeaves(struct NODE *root) {
    if (root == NULL)
        return 0;
    else if (root->left == NULL && root->right == NULL)
        return 1;

    int leftLeaves = countLeaves(root->left);
    int rightLeaves = countLeaves(root->right);

    return (leftLeaves + rightLeaves);
}

// Function to find the maximum value in the binary tree
int maior(struct NODE *node) {
    if (node == NULL)
        return 0;

    if (node->left == NULL && node->right == NULL)
        return node->info;

    int left = maior(node->left);
    int right = maior(node->right);

    if (node->info > left && node->info > right)
        return node->info;
    else if (left > node->info && left > right)
        return left;
    else if (right > node->info && right > left)
        return right;

    return 1;
}

// Main function to demonstrate the binary tree operations
int main(void) {
    BinTree *root = create_BinTree();
    int N = 8, data[8] = {50, 30, 20, 40, 45, 2000, 35, 57};
    for (int i = 0; i < N; i++) {
        insert_BinTree(root, data[i]);
    }

    printf("Maximum value: %d\n", maior(*root));
    
    struct NODE *found = search_BinTree(root, 40);
    if (found) {
        printf("Value 40 found in the tree.\n");
    } else {
        printf("Value 40 not found in the tree.\n");
    }

    delete_value(20, root);

    int height_of_tree = height(root);
    int total_elements = totalElem(root);
    int leaves = countLeaves(*root);
    printf("Height: %i\nTotal elements: %i\nLeaves: %i\n", height_of_tree, total_elements, leaves);

    return 0;
}