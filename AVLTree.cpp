#include "AVLTree.h"
#include <algorithm>

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    destroyTree(root);
}

void AVLTree::destroyTree(Node* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

int AVLTree::getHeight(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

int AVLTree::getBalanceFactor(Node* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Node* AVLTree::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node* AVLTree::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

Node* AVLTree::insertNode(Node* node, StockData data) {
    // Standard BST insert
    if (node == nullptr)
        return new Node(data);

    // We'll use ticker+date as the key for comparison
    std::string key = data.ticker + data.date;
    std::string nodeKey = node->data.ticker + node->data.date;
    
    if (key < nodeKey)
        node->left = insertNode(node->left, data);
    else if (key > nodeKey)
        node->right = insertNode(node->right, data);
    else // Equal keys not allowed
        return node;

    // Update height of current node
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // Get balance factor
    int balance = getBalanceFactor(node);

    // Left Left Case
    if (balance > 1 && (data.ticker + data.date) < (node->left->data.ticker + node->left->data.date))
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && (data.ticker + data.date) > (node->right->data.ticker + node->right->data.date))
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && (data.ticker + data.date) > (node->left->data.ticker + node->left->data.date)) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && (data.ticker + data.date) < (node->right->data.ticker + node->right->data.date)) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node* AVLTree::minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

Node* AVLTree::deleteNode(Node* root, std::string ticker, std::string date) {
    if (root == nullptr)
        return root;

    std::string key = ticker + date;
    std::string rootKey = root->data.ticker + root->data.date;
    
    // Standard BST delete
    if (key < rootKey)
        root->left = deleteNode(root->left, ticker, date);
    else if (key > rootKey)
        root->right = deleteNode(root->right, ticker, date);
    else {
        // Node with only one child or no child
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Node* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else // One child case
                *root = *temp; // Copy the contents of the non-empty child
            
            delete temp;
        } else {
            // Node with two children
            Node* temp = minValueNode(root->right);
            
            // Copy the inorder successor's data to this node
            root->data = temp->data;
            
            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->data.ticker, temp->data.date);
        }
    }

    // If the tree had only one node then return
    if (root == nullptr)
        return root;

    // Update height of the current node
    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));

    // Get the balance factor
    int balance = getBalanceFactor(root);

    // Left Left Case
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node* AVLTree::searchNode(Node* root, std::string ticker, std::string date) {
    if (root == nullptr)
        return nullptr;
        
    std::string key = ticker + date;
    std::string rootKey = root->data.ticker + root->data.date;
    
    if (key < rootKey)
        return searchNode(root->left, ticker, date);
    else if (key > rootKey)
        return searchNode(root->right, ticker, date);
    else
        return root;
}

void AVLTree::inOrderTraversal(Node* root, std::vector<StockData>& result) {
    if (root) {
        inOrderTraversal(root->left, result);
        result.push_back(root->data);
        inOrderTraversal(root->right, result);
    }
}

// Public CRUD Operations

void AVLTree::insert(StockData data) {
    root = insertNode(root, data);
}

StockData* AVLTree::search(std::string ticker, std::string date) {
    Node* result = searchNode(root, ticker, date);
    return result ? &(result->data) : nullptr;
}

bool AVLTree::update(StockData newData) {
    // First search for the node
    Node* node = searchNode(root, newData.ticker, newData.date);
    if (node == nullptr)
        return false;
        
    // Update the node data
    node->data = newData;
    return true;
}

bool AVLTree::remove(std::string ticker, std::string date) {
    // Check if the node exists
    if (searchNode(root, ticker, date) == nullptr)
        return false;
        
    root = deleteNode(root, ticker, date);
    return true;
}

std::vector<StockData> AVLTree::getAllStocks() {
    std::vector<StockData> result;
    inOrderTraversal(root, result);
    return result;
}

std::vector<StockData> AVLTree::getStocksByTicker(std::string ticker) {
    std::vector<StockData> allStocks = getAllStocks();
    std::vector<StockData> result;
    
    for (const auto& stock : allStocks) {
        if (stock.ticker == ticker) {
            result.push_back(stock);
        }
    }
    
    return result;
}

std::vector<StockData> AVLTree::getStocksByDateRange(std::string ticker, 
                                                   std::string startDate, 
                                                   std::string endDate) {
    std::vector<StockData> tickerStocks = getStocksByTicker(ticker);
    std::vector<StockData> result;
    
    for (const auto& stock : tickerStocks) {
        if (stock.date >= startDate && stock.date <= endDate) {
            result.push_back(stock);
        }
    }
    
    return result;
}
