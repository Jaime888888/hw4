#include "equal-paths.h"

using namespace std;

// Helper function to check the depth of all paths
bool checkEqualPaths(Node* node, int depth, int& pathLength) {
    if (node == nullptr) {
        return true;
    }

    // If leaf node
    if (node->left == nullptr && node->right == nullptr) {
        if (pathLength == -1) {
            pathLength = depth;
            return true;
        }
        return pathLength == depth;
    }

    // Recursively check left and right subtrees
    bool leftCheck = checkEqualPaths(node->left, depth + 1, pathLength);
    bool rightCheck = checkEqualPaths(node->right, depth + 1, pathLength);

    return leftCheck && rightCheck;
}

bool equalPaths(Node * root)
{
    int pathLength = -1;
    return checkEqualPaths(root, 0, pathLength);
}
