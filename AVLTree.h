#ifndef AVLTREE_H
#define AVLTREE_H

#include <vector>
#include <string>

struct StockData {
    std::string ticker;
    std::string date;
    double openPrice;
    double closePrice;
    double highPrice;
    double lowPrice;
    long volume;
    
    StockData(std::string t = "", std::string d = "", 
              double o = 0, double c = 0, 
              double h = 0, double l = 0, long v = 0)
        : ticker(t), date(d), openPrice(o), 
          closePrice(c), highPrice(h), lowPrice(l), volume(v) {}
};

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
    void insert(StockData data);
    StockData* search(std::string ticker, std::string date);
    bool update(StockData newData);
    bool remove(std::string ticker, std::string date);
    
    // Data Retrieval
    std::vector<StockData> getAllStocks();
    std::vector<StockData> getStocksByTicker(std::string ticker);
    std::vector<StockData> getStocksByDateRange(std::string ticker,
                                              std::string startDate,
                                              std::string endDate);
    std::vector<StockData> getMultipleTickers(const std::vector<std::string>& tickers);
};

#endif
