#include <iostream>

// 颜色枚举
enum Color {
    RED,
    BLACK
};

// 树节点结构体
template <typename T>
struct TreeNode {
    T data;
    TreeNode<T> *left;
    TreeNode<T> *right;
    TreeNode<T> *parent;
    Color color;

    // 构造函数
    TreeNode(T value, Color nodeColor = RED, TreeNode<T> *parentPtr = nullptr) {
        data = value;
        color = nodeColor;
        left = nullptr;
        right = nullptr;
        parent = parentPtr;
    }

    // 析构函数
    ~TreeNode() {
        delete left;
        delete right;
    }
};

// 红黑树类
template <typename T>
class RedBlackTree {
private:
    TreeNode<T> *root;

public:
    // 构造函数
    RedBlackTree() {
        root = nullptr;
    }

    // 析构函数
    ~RedBlackTree() {
        delete root;
    }

    // 中序遍历
    // inorderTraversal is a recursive function used to traverse a binary tree
    // in an inorder manner
    // Inorder traversal means traversing the left subtree first, then the root
    // node, and finally the right subtree. This approach ensures that the nodes
    // are traversed in ascending order of their values.
    void inorderTraversal(TreeNode<T> *node) {
        if (node != nullptr) {
            inorderTraversal(node->left);
            std::cout << node->data << " ";
            inorderTraversal(node->right);
        }
    }

    // 排序显示（使用中序遍历）
    void sortAndDisplay() {
        inorderTraversal(root);
        std::cout << std::endl;
    }

    // 搜索节点
    TreeNode<T> *search(T value) {
        return search(root, value);
    }

    // 搜索节点（递归函数）
    TreeNode<T> *search(TreeNode<T> *node, T value) {
        if (node == nullptr || node->data == value) {
            return node;
        }

        if (value < node->data) {
            return search(node->left, value);
        }

        return search(node->right, value);
    }

    // 插入节点
    void insert(T value) {
        TreeNode<T> *newNode = new TreeNode<T>(value);
        if (root == nullptr) {
            root = newNode;
            root->color = BLACK;
        } else {
            TreeNode<T> *current = root;
            TreeNode<T> *parent = nullptr;
            while (current != nullptr) {
                parent = current;
                if (value < current->data) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }

            if (value < parent->data) {
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
            newNode->parent = parent;

            fixInsert(newNode);
        }
    }

    // 插入修复
    void fixInsert(TreeNode<T> *newNode) {
        while (newNode != root && newNode->parent->color == RED) {
            TreeNode<T> *parent = newNode->parent;
            TreeNode<T> *grandparent = parent->parent;

            if (parent == grandparent->left) {
                TreeNode<T> *uncle = grandparent->right;
                if (uncle != nullptr && uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    newNode = grandparent;
                } else {
                    if (newNode == parent->right) {
                        newNode = parent;
                        leftRotate(newNode);
                    }
                    parent->color = BLACK;
                    grandparent->color = RED;
                    rightRotate(grandparent);
                }
            } else {
                TreeNode<T> *uncle = grandparent->left;
                if (uncle != nullptr && uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    newNode = grandparent;
                } else {
                    if (newNode == parent->left) {
                        newNode = parent;
                        rightRotate(newNode);
                    }
                    parent->color = BLACK;
                    grandparent->color = RED;
                    leftRotate(grandparent);
                }
            }
        }
        root->color = BLACK;
    }

    // 左旋
    void leftRotate(TreeNode<T> *node) {
        TreeNode<T> *rightChild = node->right;
        node->right = rightChild->left;

        if (rightChild->left != nullptr) {
            rightChild->left->parent = node;
        }

        rightChild->parent = node->parent;

        if (node->parent == nullptr) {
            root = rightChild;
        } else if (node == node->parent->left) {
            node->parent->left = rightChild;
        } else {
            node->parent->right = rightChild;
        }

        rightChild->left = node;
        node->parent = rightChild;
    }

    // 右旋
    void rightRotate(TreeNode<T> *node) {
        TreeNode<T> *leftChild = node->left;
        node->left = leftChild->right;

        if (leftChild->right != nullptr) {
            leftChild->right->parent = node;
        }

        leftChild->parent = node->parent;

        if (node->parent == nullptr) {
            root = leftChild;
        } else if (node == node->parent->right) {
            node->parent->right = leftChild;
        } else {
            node->parent->left = leftChild;
        }

        leftChild->right = node;
        node->parent = leftChild;
    }

    // 删除节点
    void deleteNode(T value) {
        TreeNode<T> *nodeToDelete = search(value);
        if (nodeToDelete != nullptr) {
            deleteNode(nodeToDelete);
        }
    }

    // 删除节点（内部递归函数）
    void deleteNode(TreeNode<T> *node) {
        TreeNode<T> *child;

        if (node->left != nullptr && node->right != nullptr) {
            TreeNode<T> *successor = minimumNode(node->right);
            node->data = successor->data;
            deleteNode(successor);
            return;
        }

        if (node->left != nullptr) {
            child = node->left;
        } else {
            child = node->right;
        }

        if (child != nullptr) {
            child->parent = node->parent;
            if (node->parent == nullptr) {
                root = child;
            } else if (node == node->parent->left) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }

            if (node->color == BLACK) {
                fixDelete(child);
            }
            delete node;
        } else {
            if (node->color == BLACK) {
                fixDelete(node);
            }
            if (node->parent != nullptr) {
                if (node->parent->left == node) {
                    node->parent->left = nullptr;
                } else {
                    node->parent->right = nullptr;
                }
            }
            delete node;
        }
    }

    // 删除修复
    void fixDelete(TreeNode<T> *node) {
        while (node != root && node->color == BLACK) {
            if (node == node->parent->left) {
                TreeNode<T> *sibling = node->parent->right;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    leftRotate(node->parent);
                    sibling = node->parent->right;
                }
                if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (sibling->right->color == BLACK) {
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        rightRotate(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->right->color = BLACK;
                    leftRotate(node->parent);
                    node = root;
                }
            } else {
                TreeNode<T> *sibling = node->parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rightRotate(node->parent);
                    sibling = node->parent->left;
                }
                if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (sibling->left->color == BLACK) {
                        sibling->right->color = BLACK;
                        sibling->color = RED;
                        leftRotate(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->left->color = BLACK;
                    rightRotate(node->parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }

    // 获取根节点
    TreeNode<T> *getRoot() {
        return root;
    }

    // 获取最小节点
    TreeNode<T> *minimumNode(TreeNode<T> *node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // 获取最大节点
    TreeNode<T> *maximumNode(TreeNode<T> *node) {
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }

    // 后继节点
    TreeNode<T> *successor(TreeNode<T> *node) {
        if (node->right != nullptr) {
            return minimumNode(node->right);
        }

        TreeNode<T> *parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }

        return parent;
    }

    // 前驱节点
    TreeNode<T> *predecessor(TreeNode<T> *node) {
        if (node->left != nullptr) {
            return maximumNode(node->left);
        }

        TreeNode<T> *parent = node->parent;
        while (parent != nullptr && node == parent->left) {
            node = parent;
            parent = parent->parent;
        }

        return parent;
    }
};

int main() {
    RedBlackTree<int> tree;

    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);

    std::cout << "Inorder traversal of the Red-Black Tree:" << std::endl;
    tree.sortAndDisplay();

    TreeNode<int> *foundNode = tree.search(20);
    if (foundNode != nullptr) {
        std::cout << "Value 20 found in the tree." << std::endl;
    } else {
        std::cout << "Value 20 not found in the tree." << std::endl;
    }

    tree.deleteNode(20);

    std::cout << "Delete value 20:" << std::endl;
    std::cout << "Inorder traversal of the Red-Black Tree after deletion:" << std::endl;
    tree.sortAndDisplay();

    return 0;
}