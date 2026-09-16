#include <iostream>

// 树节点结构体
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    int height;

    TreeNode(int x) :
        val(x), left(NULL), right(NULL), height(1) {
    }
};

// 获取节点高度
int height(TreeNode *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

// 获取平衡因子
int balanceFactor(TreeNode *node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

// 更新节点高度
void updateHeight(TreeNode *node) {
    node->height = std::max(height(node->left), height(node->right)) + 1;
}

// 向右旋转
TreeNode *rotateRight(TreeNode *y) {
    TreeNode *x = y->left;
    TreeNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

// 向左旋转
TreeNode *rotateLeft(TreeNode *x) {
    TreeNode *y = x->right;
    TreeNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// 在AVL树中插入节点，并返回根节点
TreeNode *insert(TreeNode *root, int key) {
    if (root == NULL) {
        return new TreeNode(key);
    }

    if (key < root->val) {
        root->left = insert(root->left, key);
    } else if (key > root->val) {
        root->right = insert(root->right, key);
    } else {
        return root; // 不允许插入重复值
    }

    updateHeight(root);

    int balance = balanceFactor(root);

    // 左左不平衡，执行右旋
    if (balance > 1 && key < root->left->val) {
        return rotateRight(root);
    }
    // 右右不平衡，执行左旋
    if (balance < -1 && key > root->right->val) {
        return rotateLeft(root);
    }
    // 左右不平衡，先左旋再右旋
    if (balance > 1 && key > root->left->val) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    // 右左不平衡，先右旋再左旋
    if (balance < -1 && key < root->right->val) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// 中序遍历AVL树并输出
void inorderTraversal(TreeNode *root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        std::cout << root->val << " ";
        inorderTraversal(root->right);
    }
}

int main() {
    TreeNode *root = NULL;

    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 15);
    root = insert(root, 25);

    std::cout << "inorderTraversal:";
    inorderTraversal(root);
    std::cout << std::endl;

    return 0;
}
