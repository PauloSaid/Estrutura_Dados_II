// ALUNO: PAULO ANDRÉ RIBEIRO SAID
//        CC6M


#include <stdio.h>
#include <stdlib.h>

#define MAX 3  // Order of the B-tree. Adjust as needed.
#define MIN 2  // Minimum degree, half of MAX (for simplicity).

// Node structure for the B-tree
struct BTreeNode {
    int items[MAX];
    struct BTreeNode *children[MAX + 1];
    int n; // Number of keys in the node
    int leaf; // 1 if leaf node, 0 otherwise
};

struct BTreeNode *root = NULL;

// Function to create a new node
struct BTreeNode* createNode(int leaf) {
    struct BTreeNode* node = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    node->leaf = leaf;
    node->n = 0;
    for (int i = 0; i < MAX + 1; i++)
        node->children[i] = NULL;
    return node;
}

// Function to split child nodes
void splitChild(struct BTreeNode* parent, int i, struct BTreeNode* y) {
    struct BTreeNode* z = createNode(y->leaf);
    z->n = MIN - 1;

    for (int j = 0; j < MIN - 1; j++)
        z->items[j] = y->items[j + MIN];

    if (!y->leaf) {
        for (int j = 0; j < MIN; j++)
            z->children[j] = y->children[j + MIN];
    }

    y->n = MIN - 1;

    for (int j = parent->n; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[i + 1] = z;

    for (int j = parent->n - 1; j >= i; j--)
        parent->items[j + 1] = parent->items[j];

    parent->items[i] = y->items[MIN - 1];
    parent->n += 1;
}

// Insert non-full node
void insertNonFull(struct BTreeNode* node, int key) {
    int i = node->n - 1;

    if (node->leaf) {
        while (i >= 0 && key < node->items[i]) {
            node->items[i + 1] = node->items[i];
            i--;
        }
        node->items[i + 1] = key;
        node->n += 1;
    } else {
        while (i >= 0 && key < node->items[i])
            i--;

        if (node->children[i + 1]->n == MAX) {
            splitChild(node, i + 1, node->children[i + 1]);

            if (key > node->items[i + 1])
                i++;
        }
        insertNonFull(node->children[i + 1], key);
    }
}

// Insert a new key
void insert(int key) {
    if (root == NULL) {
        root = createNode(1);
        root->items[0] = key;
        root->n = 1;
    } else {
        if (root->n == MAX) {
            struct BTreeNode* newRoot = createNode(0);
            newRoot->children[0] = root;
            splitChild(newRoot, 0, root);

            int i = 0;
            if (newRoot->items[0] < key)
                i++;
            insertNonFull(newRoot->children[i], key);
            root = newRoot;
        } else {
            insertNonFull(root, key);
        }
    }
}

// Search a key in the B-tree
struct BTreeNode* search(struct BTreeNode* node, int key) {
    int i = 0;
    while (i < node->n && key > node->items[i])
        i++;

    if (i < node->n && key == node->items[i])
        return node;

    if (node->leaf)
        return NULL;

    return search(node->children[i], key);
}

// Display the B-tree
void display(struct BTreeNode* node, int level) {
    if (node != NULL) {
        int i;
        for (i = 0; i < node->n; i++) {
            if (!node->leaf)
                display(node->children[i], level + 1);
            printf("Level %d: %d\n", level, node->items[i]);
        }
        if (!node->leaf)
            display(node->children[i], level + 1);
    }
}

int main() {
    int choice, key;
    while (1) {
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Display\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
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
                if (search(root, key) != NULL)
                    printf("Key %d found in the B-tree.\n", key);
                else
                    printf("Key %d not found in the B-tree.\n", key);
                break;
            case 3:
                printf("B-tree:\n");
                display(root, 0);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}