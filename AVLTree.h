#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <string>
#include <vector>

// Stock data structure to store in the AVL tree
struct StockData {
    std::string ticker;
    std::string date;
    double openPrice;
    double closePrice;
    double highPrice;
    double lowPrice;
    long volume;
    
    StockData() : ticker(""), date(""), openPrice(0), closePrice(0), 
                  highPrice(0), lowPrice(0), volume(0) {}
    
    StockData(std::string t, std::string d, double o, double c, 
              double h, double l, long v) : ticker(t), date(d), 
              openPrice(o), closePrice(c), highPrice(h), 
              lowPrice(l), volume(v) {}
};

// Node structure for AVL Tree
struct Node {
    StockData data;
    Node* left;
    Node* right;
    int height;
    
    Node(StockData d) : data(d), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;
    
    // Helper functions
    int getHeight(Node* node);
    int getBalanceFactor(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* insertNode(Node* node, StockData data);
    Node* minValueNode(Node* node);
    Node* deleteNode(Node* root, std::string ticker, std::string date);
    Node* searchNode(Node* root, std::string ticker, std::string date);
    void inOrderTraversal(Node* root, std::vector<StockData>& result);
    void destroyTree(Node* node);
    
public:
    AVLTree();
    ~AVLTree();
    
    // CRUD Operations
    void insert(StockData data);                             // Create
    StockData* search(std::string ticker, std::string date); // Read
    bool update(StockData newData);                          // Update
    bool remove(std::string ticker, std::string date);       // Delete
    
    // Additional operations
    std::vector<StockData> getAllStocks();
    std::vector<StockData> getStocksByTicker(std::string ticker);
    std::vector<StockData> getStocksByDateRange(std::string ticker, 
                                               std::string startDate, 
                                               std::string endDate);
};

#endif // AVLTREE_H
