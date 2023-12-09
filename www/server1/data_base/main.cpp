#include <iostream>
#include <vector>
// #include <cstring>  
#include <string.h>
#include <unordered_map>
#include <stack>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

void BFSTree(TreeNode *root) {

    if (root == NULL) {
        return ;
    }

    if (root->left) {
        BFSTree(root->left);
    }
    std::cout << root->val << " ";
    if (root->right) {
        BFSTree(root->right);
    }
}

class Solution {
    int countHeight(TreeNode* root) {
        if (root == NULL) {
            return (0);
        }
        int countLeft = countHeight(root->left);
        int countRight = countHeight(root->right);
        std::cout << "countLeft = " <<countLeft << std::endl;
        std::cout << "countRight = " <<countRight << std::endl;
        if (countLeft == -1
            || countRight == -1
            || abs(countLeft - countRight) > 1) {
            return -1;
        } else if (countLeft < countRight) {
            return (countRight + 1);
        }
        return (countLeft + 1);
    }
public:
    bool isBalanced(TreeNode* root) {
        int a = countHeight(root);
        std::cout << "a = " << a << std::endl;
        return (a != -1);
    }
};

int main() {
    TreeNode tree1(0);
    tree1.left = new TreeNode(-3);
    tree1.left->left = new TreeNode(-10);
    tree1.right = new TreeNode(5);
    tree1.right->right = new TreeNode(9);
    tree1.right->right->right = new TreeNode(9);
    tree1.right->right->right->right = new TreeNode(9);
    tree1.right->right->right->right->right = new TreeNode(9);
    Solution a;
    std::vector<int> vec = {-10, -3, 0, 5, 9};
    std::cout << a.isBalanced(&tree1) << std::endl;
    // std::cout << a.isSameTree(&tree1, &tree2) << std::endl;
    // BFSTree(root);
    // BFSTree(&tree1);

}

