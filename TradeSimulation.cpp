#include "TradeSimulation.h"
#include <iomanip>
#include <sstream>

bool Portfolio::buyStock(const std::string& ticker, const std::string& date, int quantity, const AVLTree& stockData) {
    if (quantity <= 0) {
        transactionHistory.push_back("Invalid buy order for " + ticker + ": quantity must be positive");
        return false;
    }

    StockData* stock = stockData.search(ticker, date);
    if (!stock) {
        transactionHistory.push_back("Buy failed: No data found for " + ticker + " on " + date);
        return false;
    }

    double totalCost = stock->closePrice * quantity;
    if (totalCost > cashBalance) {
        transactionHistory.push_back("Buy failed: Insufficient funds for " + std::to_string(quantity) +
                                  " shares of " + ticker + " on " + date);
        return false;
    }

    cashBalance -= totalCost;
    holdings[ticker] += quantity;

    std::ostringstream oss;
    oss << "Bought " << quantity << " shares of " << ticker << " at $"
        << std::fixed << std::setprecision(2) << stock->closePrice
        << " on " << date << ". Total cost: $" << totalCost;
    transactionHistory.push_back(oss.str());

    return true;
}

bool Portfolio::sellStock(const std::string& ticker, const std::string& date, int quantity, const AVLTree& stockData) {
    if (quantity <= 0) {
        transactionHistory.push_back("Invalid sell order for " + ticker + ": quantity must be positive");
        return false;
    }

    auto it = holdings.find(ticker);
    if (it == holdings.end() || it->second < quantity) {
        transactionHistory.push_back("Sell failed: Not enough shares of " + ticker + " to sell");
        return false;
    }

    StockData* stock = stockData.search(ticker, date);
    if (!stock) {
        transactionHistory.push_back("Sell failed: No data found for " + ticker + " on " + date);
        return false;
    }

    double totalValue = stock->closePrice * quantity;
    cashBalance += totalValue;
    holdings[ticker] -= quantity;

    if (holdings[ticker] == 0) {
        holdings.erase(ticker);
    }

    std::ostringstream oss;
    oss << "Sold " << quantity << " shares of " << ticker << " at $"
        << std::fixed << std::setprecision(2) << stock->closePrice
        << " on " << date << ". Total value: $" << totalValue;
    transactionHistory.push_back(oss.str());

    return true;
}

double Portfolio::getPortfolioValue(const AVLTree& stockData, const std::string& currentDate) const {
    double totalValue = cashBalance;
    
    for (const auto& holding : holdings) {
        StockData* stock = stockData.search(holding.first, currentDate);
        if (stock) {
            totalValue += stock->closePrice * holding.second;
        }
    }
    
    return totalValue;
}

void Portfolio::displayPortfolio(const AVLTree& stockData, const std::string& currentDate) const {
    std::cout << "\n=== Portfolio Summary ===\n";
    std::cout << "Date: " << currentDate << "\n";
    std::cout << "Cash Balance: $" << std::fixed << std::setprecision(2) << cashBalance << "\n";
    
    std::cout << "\nHoldings:\n";
    if (holdings.empty()) {
        std::cout << "No stock holdings\n";
    } else {
        for (const auto& holding : holdings) {
            StockData* stock = stockData.search(holding.first, currentDate);
            if (stock) {
                std::cout << holding.first << ": " << holding.second << " shares @ $"
                          << std::fixed << std::setprecision(2) << stock->closePrice
                          << " = $" << (stock->closePrice * holding.second) << "\n";
            } else {
                std::cout << holding.first << ": " << holding.second << " shares (no price data for "
                          << currentDate << ")\n";
            }
        }
    }
    
    double portfolioValue = getPortfolioValue(stockData, currentDate);
    std::cout << "\nTotal Portfolio Value: $" << std::fixed << std::setprecision(2)
              << portfolioValue << "\n";
}
