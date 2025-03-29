#include "AVLTree.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

void displayMenu() {
    std::cout << "\n===== Stock Market Data Analyzer =====\n";
    std::cout << "1. Insert new stock data\n";
    std::cout << "2. Search for a stock\n";
    std::cout << "3. Update stock data\n";
    std::cout << "4. Remove stock data\n";
    std::cout << "5. Display all stocks\n";
    std::cout << "6. Display stocks by ticker\n";
    std::cout << "7. Display stocks by date range\n";
    std::cout << "8. Exit\n";
    std::cout << "Enter your choice (1-8): ";
}

StockData inputStockData() {
    StockData data;
    std::cout << "Enter ticker symbol: ";
    std::cin >> data.ticker;
    std::cout << "Enter date (YYYY-MM-DD): ";
    std::cin >> data.date;
    std::cout << "Enter opening price: ";
    std::cin >> data.openPrice;
    std::cout << "Enter closing price: ";
    std::cin >> data.closePrice;
    std::cout << "Enter highest price: ";
    std::cin >> data.highPrice;
    std::cout << "Enter lowest price: ";
    std::cin >> data.lowPrice;
    std::cout << "Enter volume: ";
    std::cin >> data.volume;
    return data;
}

void displayStock(const StockData& stock) {
    std::cout << "Ticker: " << stock.ticker << " | Date: " << stock.date << "\n";
    std::cout << "Open: $" << std::fixed << std::setprecision(2) << stock.openPrice;
    std::cout << " | Close: $" << stock.closePrice;
    std::cout << " | High: $" << stock.highPrice;
    std::cout << " | Low: $" << stock.lowPrice;
    std::cout << " | Volume: " << stock.volume << "\n";
}

int main() {
    AVLTree stockTree;
    int choice;

    while (true) {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                StockData newStock = inputStockData();
                stockTree.insert(newStock);
                std::cout << "Stock data inserted successfully.\n";
                break;
            }
            case 2: {
                std::string ticker, date;
                std::cout << "Enter ticker symbol: ";
                std::cin >> ticker;
                std::cout << "Enter date (YYYY-MM-DD): ";
                std::cin >> date;
                StockData* stock = stockTree.search(ticker, date);
                if (stock) {
                    std::cout << "Stock found:\n";
                    displayStock(*stock);
                } else {
                    std::cout << "Stock not found.\n";
                }
                break;
            }
            case 3: {
                StockData updatedStock = inputStockData();
                if (stockTree.update(updatedStock)) {
                    std::cout << "Stock data updated successfully.\n";
                } else {
                    std::cout << "Stock not found. Update failed.\n";
                }
                break;
            }
            case 4: {
                std::string ticker, date;
                std::cout << "Enter ticker symbol to remove: ";
                std::cin >> ticker;
                std::cout << "Enter date (YYYY-MM-DD) to remove: ";
                std::cin >> date;
                if (stockTree.remove(ticker, date)) {
                    std::cout << "Stock data removed successfully.\n";
                } else {
                    std::cout << "Stock not found. Removal failed.\n";
                }
                break;
            }
            case 5: {
                std::vector<StockData> allStocks = stockTree.getAllStocks();
                std::cout << "All stocks:\n";
                for (const auto& stock : allStocks) {
                    displayStock(stock);
                }
                break;
            }
            case 6: {
                std::string ticker;
                std::cout << "Enter ticker symbol: ";
                std::cin >> ticker;
                std::vector<StockData> tickerStocks = stockTree.getStocksByTicker(ticker);
                std::cout << "Stocks for " << ticker << ":\n";
                for (const auto& stock : tickerStocks) {
                    displayStock(stock);
                }
                break;
            }
            case 7: {
                std::string startDate, endDate;
                std::cout << "Enter start date (YYYY-MM-DD): ";
                std::cin >> startDate;
                std::cout << "Enter end date (YYYY-MM-DD): ";
                std::cin >> endDate;
                
                std::vector<StockData> allStocks = stockTree.getAllStocks();
                std::vector<StockData> rangeStocks;
                
                for (const auto& stock : allStocks) {
                    if (stock.date >= startDate && stock.date <= endDate) {
                        rangeStocks.push_back(stock);
                    }
                }
                
                std::cout << "Stocks between " << startDate << " and " << endDate << ":\n";
                for (const auto& stock : rangeStocks) {
                    displayStock(stock);
                }
                break;
            }
            
            case 8:
                std::cout << "Thank you for using Stock Market Data Analyzer. BYE!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }

        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return 0;
}
