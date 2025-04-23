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
    return node ? node->height : 0;
}

int AVLTree::getBalanceFactor(Node* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

Node* AVLTree::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

Node* AVLTree::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

Node* AVLTree::insertNode(Node* node, StockData data) {
    if (!node) return new Node(data);
    
    std::string key = data.ticker + data.date;
    std::string nodeKey = node->data.ticker + node->data.date;
    
    if (key < nodeKey) node->left = insertNode(node->left, data);
    else if (key > nodeKey) node->right = insertNode(node->right, data);
    else return node;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    
    int balance = getBalanceFactor(node);
    
    // Left Left
    if (balance > 1 && key < (node->left->data.ticker + node->left->data.date))
        return rightRotate(node);
    
    // Right Right
    if (balance < -1 && key > (node->right->data.ticker + node->right->data.date))
        return leftRotate(node);
    
    // Left Right
    if (balance > 1 && key > (node->left->data.ticker + node->left->data.date)) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    // Right Left
    if (balance < -1 && key < (node->right->data.ticker + node->right->data.date)) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

Node* AVLTree::minValueNode(Node* node) {
    Node* current = node;
    while (current->left) current = current->left;
    return current;
}

Node* AVLTree::deleteNode(Node* root, std::string ticker, std::string date) {
    if (!root) return root;
    
    std::string key = ticker + date;
    std::string rootKey = root->data.ticker + root->data.date;
    
    if (key < rootKey) root->left = deleteNode(root->left, ticker, date);
    else if (key > rootKey) root->right = deleteNode(root->right, ticker, date);
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            } else *root = *temp;
            delete temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data.ticker, temp->data.date);
        }
    }

    if (!root) return root;

    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    int balance = getBalanceFactor(root);

    // Rebalance
    if (balance > 1) {
        if (getBalanceFactor(root->left) >= 0) return rightRotate(root);
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1) {
        if (getBalanceFactor(root->right) <= 0) return leftRotate(root);
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

Node* AVLTree::searchNode(Node* root, std::string ticker, std::string date) {
    if (!root) return nullptr;
    std::string key = ticker + date;
    std::string rootKey = root->data.ticker + root->data.date;
    
    if (key < rootKey) return searchNode(root->left, ticker, date);
    if (key > rootKey) return searchNode(root->right, ticker, date);
    return root;
}

void AVLTree::inOrderTraversal(Node* root, std::vector<StockData>& result) {
    if (root) {
        inOrderTraversal(root->left, result);
        result.push_back(root->data);
        inOrderTraversal(root->right, result);
    }
}

// Public methods
void AVLTree::insert(StockData data) {
    root = insertNode(root, data);
}

StockData* AVLTree::search(std::string ticker, std::string date) {
    Node* result = searchNode(root, ticker, date);
    return result ? &(result->data) : nullptr;
}

bool AVLTree::update(StockData newData) {
    Node* node = searchNode(root, newData.ticker, newData.date);
    if (!node) return false;
    node->data = newData;
    return true;
}

bool AVLTree::remove(std::string ticker, std::string date) {
    if (!searchNode(root, ticker, date)) return false;
    root = deleteNode(root, ticker, date);
    return true;
}

std::vector<StockData> AVLTree::getAllStocks() {
    std::vector<StockData> result;
    inOrderTraversal(root, result);
    return result;
}

std::vector<StockData> AVLTree::getStocksByTicker(std::string ticker) {
    std::vector<StockData> all = getAllStocks();
    std::vector<StockData> result;
    for (const auto& s : all)
        if (s.ticker == ticker) result.push_back(s);
    return result;
}

std::vector<StockData> AVLTree::getStocksByDateRange(std::string ticker,
                                                   std::string startDate,
                                                   std::string endDate) {
    auto stocks = getStocksByTicker(ticker);
    std::vector<StockData> result;
    for (const auto& s : stocks)
        if (s.date >= startDate && s.date <= endDate)
            result.push_back(s);
    return result;
}

std::vector<StockData> AVLTree::getMultipleTickers(const std::vector<std::string>& tickers) {
    std::vector<StockData> result;
    for (const auto& t : tickers) {
        auto stocks = getStocksByTicker(t);
        result.insert(result.end(), stocks.begin(), stocks.end());
    }
    return result;
}
