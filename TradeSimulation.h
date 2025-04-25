#pragma once

#include "AVLTree.h"
#include <map>
#include <string>

class Portfolio {
private:
    std::map<std::string, int> holdings;
    double cashBalance;
    std::vector<std::string> transactionHistory;

public:
    Portfolio(double initialBalance) : cashBalance(initialBalance) {}

    // Buy a stock
    bool buyStock(const std::string& ticker, const std::string& date, int quantity, const AVLTree& stockData);

    // Sell a stock
    bool sellStock(const std::string& ticker, const std::string& date, int quantity, const AVLTree& stockData);

    // Get current portfolio value
    double getPortfolioValue(const AVLTree& stockData, const std::string& currentDate) const;

    // Get cash balance
    double getCashBalance() const { return cashBalance; }

    // Get holdings
    const std::map<std::string, int>& getHoldings() const { return holdings; }

    // Get transaction history
    const std::vector<std::string>& getTransactionHistory() const { return transactionHistory; }

    // Display portfolio summary
    void displayPortfolio(const AVLTree& stockData, const std::string& currentDate) const;
};

