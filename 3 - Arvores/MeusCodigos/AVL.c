// ALUNO: Paulo André Ribeiro Said
//        CC6M

#include <stdio.h>
#include <stdlib.h>

// Structure for an AVL tree node
typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// Helper function to get the height of a node
int height(Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Quick function to return the bigger of two numbers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Make a new node with the given value
Node* newNode(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // New node, so height is 1
    return node;
}

// Rotate right around a node (used to balance the tree)
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Do the rotation
    x->right = y;
    y->left = T2;

    // Update heights after rotation
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return the new root
    return x;
}

// Rotate left around a node (another balancing trick)
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform the rotation
    y->left = x;
    x->right = T2;

    // Update the heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return the new root
    return y;
}

// Get the balance factor of a node (difference in height between left and right)
int getBalance(Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Insert a value into the AVL tree
Node* insert(Node* node, int value) {
    // Basic BST insertion
    if (node == NULL)
        return newNode(value);

    // Go left or right depending on the value
    if (value < node->value)
        node->left = insert(node->left, value);
    else if (value > node->value)
        node->right = insert(node->right, value);
    else  // Duplicates not allowed
        return node;

    // Update the height of the current node
    node->height = 1 + max(height(node->left), height(node->right));

    // Get the balance factor
    int balance = getBalance(node);

    // Fix the tree if it's unbalanced

    // Left-heavy (rotate right)
    if (balance > 1 && value < node->left->value)
        return rightRotate(node);

    // Right-heavy (rotate left)
    if (balance < -1 && value > node->right->value)
        return leftRotate(node);

    // Left-right case (rotate left, then right)
    if (balance > 1 && value > node->left->value) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right-left case (rotate right, then left)
    if (balance < -1 && value < node->right->value) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the unchanged node pointer
    return node;
}

// Print the tree in-order (smallest to largest)
void inOrder(Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->value);
        inOrder(root->right);
    }
}

int main(int argc, char const *argv[]) {
    Node* root = NULL;

    // Insert some numbers into the tree
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);

    // Print the tree in-order
    printf("\nAVL tree in-order: ");
    inOrder(root);
    printf("\n");

    return 0;
}
