// STUDENT: Paulo André Ribeiro Said
//          CC6M

#include <stdio.h>
#include <stdlib.h>

#define MAX 3  // MAX KEYS
#define MIN (MAX + 1) / 2  // MIN KEYS

// B+ Tree Node structure
typedef struct BPTreeNode {
    int keys[MAX + 1];
    struct BPTreeNode *children[MAX + 1];
    struct BPTreeNode *next;
    int count;
    int isLeaf;
} BPTreeNode;

BPTreeNode *root = NULL;

// Function to create a new node
BPTreeNode *createNode(int isLeaf) {
    BPTreeNode *node = (BPTreeNode *)malloc(sizeof(BPTreeNode));
    if (!node) {
        printf("Memory allocation error.\n");
        exit(1);
    }
    node->isLeaf = isLeaf;
    node->count = 0;
    node->next = NULL;
    for (int i = 0; i <= MAX; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// Function to split a child node when it's full
void splitChild(BPTreeNode *parent, int index, BPTreeNode *child) {
    int mid = MIN - 1;
    BPTreeNode *newChild = createNode(child->isLeaf);
    newChild->count = MIN - 1;

    // Copying half the keys to the new node
    for (int j = 0; j < MIN - 1; j++)
        newChild->keys[j] = child->keys[j + mid + 1];

    if (!child->isLeaf) {
        for (int j = 0; j < MIN; j++)
            newChild->children[j] = child->children[j + mid + 1];
    }

    child->count = mid;
    for (int j = parent->count; j >= index + 1; j--)
        parent->children[j + 1] = parent->children[j];
    parent->children[index + 1] = newChild;

    for (int j = parent->count - 1; j >= index; j--)
        parent->keys[j + 1] = parent->keys[j];
    parent->keys[index] = child->keys[mid];
    parent->count++;
}

// Insert into a non-full node
void insertNonFull(BPTreeNode *node, int key) {
    int i = node->count - 1;

    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->count++;
    } else {
        while (i >= 0 && key < node->keys[i])
            i--;
        i++;
        if (node->children[i]->count == MAX) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i])
                i++;
        }
        insertNonFull(node->children[i], key);
    }
}

// Main function to insert a key into the tree
void insert(int key) {
    if (root == NULL) {
        root = createNode(1);
        root->keys[0] = key;
        root->count = 1;
    } else {
        if (root->count == MAX) {
            BPTreeNode *newRoot = createNode(0);
            newRoot->children[0] = root;
            splitChild(newRoot, 0, root);
            int i = (newRoot->keys[0] < key) ? 1 : 0;
            insertNonFull(newRoot->children[i], key);
            root = newRoot;
        } else {
            insertNonFull(root, key);
        }
    }
}

// Search function
void search(BPTreeNode *node, int key) {
    int i = 0;
    while (i < node->count && key > node->keys[i])
        i++;
    if (i < node->count && key == node->keys[i]) {
        printf("Key %d found.\n", key);
        return;
    }
    if (node->isLeaf) {
        printf("Key %d not found.\n", key);
        return;
    }
    search(node->children[i], key);
}

// Function to free memory of a node recursively
void freeNode(BPTreeNode *node) {
    if (node) {
        if (!node->isLeaf) {
            for (int i = 0; i <= node->count; i++) {
                freeNode(node->children[i]);
            }
        }
        free(node);
    }
}

// Function to merge two nodes
void mergeNodes(BPTreeNode *node, int idx) {
    BPTreeNode *child = node->children[idx];
    BPTreeNode *sibling = node->children[idx + 1];

    child->keys[MIN - 1] = node->keys[idx];
    for (int i = 0; i < sibling->count; ++i)
        child->keys[i + MIN] = sibling->keys[i];

    if (!child->isLeaf) {
        for (int i = 0; i <= sibling->count; ++i)
            child->children[i + MIN] = sibling->children[i];
    }

    for (int i = idx + 1; i < node->count; ++i)
        node->keys[i - 1] = node->keys[i];
    for (int i = idx + 2; i <= node->count; ++i)
        node->children[i - 1] = node->children[i];

    child->count += sibling->count + 1;
    node->count--;
    free(sibling);
}

// Function to display the tree
// OBS.: Not working properly sometimes, but it helps to display the tree.
void display(BPTreeNode *node, int level) {
    if (node != NULL) {
        printf("Level %d: ", level);
        for (int i = 0; i < node->count; i++)
            printf("%d ", node->keys[i]);
        printf("\n");
        for (int i = 0; i <= node->count; i++)
            display(node->children[i], level + 1);
    }
}

int main() {
    int choice, key;
    while (1) {
        printf("\n1. Insert\n2. Search\n3. Display\n4. Exit\nChoice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter key to insert: ");
                scanf("%d", &key);
                insert(key);
                break;
            case 2:
                printf("Enter key to search: ");
                scanf("%d", &key);
                search(root, key);
                break;
            case 3:
                display(root, 0);
                break;
            case 4:
                freeNode(root);
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}