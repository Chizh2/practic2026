#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

/* Цвет узла */
typedef enum {
    RED,
    BLACK
} Color;

/* Структура узла */
typedef struct Node {
    int key;
    Color color;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

/* Глобальный фиктивный узел */
Node* NIL;

/* Создание фиктивного узла */
void initNIL() {
    NIL = (Node*)malloc(sizeof(Node));

    NIL->color = BLACK;
    NIL->left = NIL;
    NIL->right = NIL;
    NIL->parent = NIL;
}

/* Создание нового узла */
Node* createNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));

    node->key = key;
    node->color = RED;

    node->left = NIL;
    node->right = NIL;
    node->parent = NIL;

    return node;
}

/* Левый поворот */
void leftRotate(Node** root, Node* x) {
    Node* y = x->right;

    x->right = y->left;

    if (y->left != NIL) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NIL) {
        *root = y;
    }
    else {
        if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
    }

    y->left = x;
    x->parent = y;
}

/* Правый поворот */
void rightRotate(Node** root, Node* y) {
    Node* x = y->left;

    y->left = x->right;

    if (x->right != NIL) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == NIL) {
        *root = x;
    }
    else {
        if (y == y->parent->left) {
            y->parent->left = x;
        }
        else {
            y->parent->right = x;
        }
    }

    x->right = y;
    y->parent = x;
}

/* Поиск элемента */
Node* search(Node* root, int key) {
    while (root != NIL && key != root->key) {
        if (key < root->key) {
            root = root->left;
        }
        else {
            root = root->right;
        }
    }

    return root;
}

/* Поиск минимального элемента */
Node* treeMinimum(Node* node) {
    while (node->left != NIL) {
        node = node->left;
    }

    return node;
}

/* Замена одного поддерева другим */
void transplant(Node** root, Node* u, Node* v) {
    if (u->parent == NIL) {
        *root = v;
    }
    else {
        if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
    }

    v->parent = u->parent;
}

/* Восстановление свойств дерева после вставки */
void fixInsert(Node** root, Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;

            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                rightRotate(root, z->parent->parent);
            }
        }
        else {
            Node* y = z->parent->parent->left;

            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                leftRotate(root, z->parent->parent);
            }
        }
    }

    (*root)->color = BLACK;
}

/* Вставка нового узла */
void insert(Node** root, int key) {
    Node* node = createNode(key);

    Node* parent = NIL;
    Node* current = *root;

    while (current != NIL) {
        parent = current;

        if (node->key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    node->parent = parent;

    if (parent == NIL) {
        *root = node;
    }
    else {
        if (node->key < parent->key) {
            parent->left = node;
        }
        else {
            parent->right = node;
        }
    }

    node->left = NIL;
    node->right = NIL;
    node->color = RED;

    fixInsert(root, node);
}

/* Восстановление свойств дерева после удаления */
void fixDelete(Node** root, Node* x) {
    while (x != *root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;

                leftRotate(root, x->parent);

                w = x->parent->right;
            }

            if (w->left->color == BLACK &&
                w->right->color == BLACK) {

                w->color = RED;

                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;

                    rightRotate(root, w);

                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;

                leftRotate(root, x->parent);

                x = *root;
            }
        }
        else {
            Node* w = x->parent->left;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;

                rightRotate(root, x->parent);

                w = x->parent->left;
            }

            if (w->right->color == BLACK &&
                w->left->color == BLACK) {

                w->color = RED;

                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;

                    leftRotate(root, w);

                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;

                rightRotate(root, x->parent);

                x = *root;
            }
        }
    }

    x->color = BLACK;
}

/* Удаление узла */
void deleteNode(Node** root, int key) {
    Node* z = search(*root, key);

    if (z == NIL) {
        printf("Элемент не найден!\n");
        return;
    }

    Node* y = z;
    Node* x;

    Color originalColor = y->color;

    if (z->left == NIL) {
        x = z->right;
        transplant(root, z, z->right);
    }
    else {
        if (z->right == NIL) {
            x = z->left;
            transplant(root, z, z->left);
        }
        else {
            y = treeMinimum(z->right);
            originalColor = y->color;

            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            }
            else {
                transplant(root, y, y->right);

                y->right = z->right;
                y->right->parent = y;
            }

            transplant(root, z, y);

            y->left = z->left;
            y->left->parent = y;

            y->color = z->color;
        }
    }

    free(z);

    if (originalColor == BLACK) {
        fixDelete(root, x);
    }
}

/* Вывод дерева */
void printTree(Node* root, int space) {
    int i;

    if (root == NIL) {
        return;
    }

    space += 8;

    printTree(root->right, space);

    printf("\n");

    for (i = 8; i < space; i++) {
        printf(" ");
    }

    if (root->color == RED) {
        printf("%d(R)\n", root->key);
    }
    else {
        printf("%d(B)\n", root->key);
    }

    printTree(root->left, space);
}

int main() {
    setlocale(LC_CTYPE, "RUSSIAN");
    Node* root;

    int choice;
    int key;

    initNIL();
    root = NIL;

    do {
        printf("Красно-черное дерево\n");
        printf("1. Добавить элемент\n");
        printf("2. Найти элемент\n");
        printf("3. Удалить элемент\n");
        printf("4. Показать дерево\n");
        printf("0. Выход\n");

        printf("Выберите действие: ");
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            printf("Введите ключ: ");
            scanf_s("%d", &key);

            insert(&root, key);

            printf("Элемент добавлен.\n");
            break;

        case 2:
            printf("Введите ключ: ");
            scanf_s("%d", &key);

            if (search(root, key) != NIL) {
                printf("Элемент найден.\n");
            }
            else {
                printf("Элемент не найден.\n");
            }

            break;

        case 3:
            printf("Введите ключ: ");
            scanf_s("%d", &key);

            deleteNode(&root, key);

            break;

        case 4:
            if (root == NIL) {
                printf("Дерево пустое.\n");
            }
            else {
                printTree(root, 0);
            }

            break;

        case 0:
            printf("Работа программы завершена.\n");
            break;

        default:
            printf("Неверный пункт меню.\n");
        }

    } while (choice != 0);

    return 0;
}
