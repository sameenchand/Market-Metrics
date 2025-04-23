#include "AVLTree.h"
#include "FinancialMetrics.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

void displayMenu() {
    std::cout << "\n===== Stock Market Data Analyzer =====\n";
    std::cout << "1. Import stock data from CSV\n";
    std::cout << "2. Search for a stock\n";
    std::cout << "3. Update stock data\n";
    std::cout << "4. Remove stock data\n";
    std::cout << "5. Display all stocks\n";
    std::cout << "6. Display stocks by ticker\n";
    std::cout << "7. Display stocks by date range\n";
    std::cout << "8. Export to CSV\n";
    std::cout << "9. Compare multiple stocks\n";
    std::cout << "10. Calculate financial metrics\n";
    std::cout << "11. Simulate trade\n";
    std::cout << "12. Exit\n";
    std::cout << "Enter your choice (1-12): ";
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

void displayStock(const StockData &stock) {
    std::cout << "Ticker: " << stock.ticker << " | Date: " << stock.date << "\n";
    std::cout << "Open: $" << std::fixed << std::setprecision(2) << stock.openPrice;
    std::cout << " | Close: $" << stock.closePrice;
    std::cout << " | High: $" << stock.highPrice;
    std::cout << " | Low: $" << stock.lowPrice;
    std::cout << " | Volume: " << stock.volume << "\n\n";
}

void exportDataAsCSV(AVLTree stockTree) {
    std::ofstream outFile("stocks.csv");
    if (outFile.is_open()) {
        outFile << "Ticker,Date,Open,Close,High,Low,Volume\n";
        std::vector<StockData> allStocks = stockTree.getAllStocks();
        for (const auto &stock : allStocks) {
            outFile << stock.ticker << "," << stock.date << ","
                    << stock.openPrice << "," << stock.closePrice << ","
                    << stock.highPrice << "," << stock.lowPrice << ","
                    << stock.volume << "\n";
        }
        outFile.close();
        std::cout << "Data exported to stocks.csv successfully.\n";
    } else {
        std::cout << "Error opening file for export.\n";
    }
}

void importDataFromCSV(AVLTree& stockTree, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        StockData data;
        std::string temp;
        std::getline(ss, data.ticker, ',');
        std::getline(ss, data.date, ',');
        std::getline(ss, temp, ','); data.openPrice = std::stod(temp);
        std::getline(ss, temp, ','); data.closePrice = std::stod(temp);
        std::getline(ss, temp, ','); data.highPrice = std::stod(temp);
        std::getline(ss, temp, ','); data.lowPrice = std::stod(temp);
        std::getline(ss, temp, ','); data.volume = std::stol(temp);
        stockTree.insert(data);
    }
    std::cout << "CSV data imported successfully!\n";
    file.close();
}

int main() {
    AVLTree stockTree;
    int choice;
    while (true) {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                std::string filename;
                std::cout << "Enter CSV filename (e.g., stocks.csv): ";
                std::getline(std::cin, filename);
                importDataFromCSV(stockTree, filename);
                break;
            }
            case 2: {
                std::string ticker, date;
                std::cout << "Enter ticker symbol: ";
                std::getline(std::cin, ticker);
                std::cout << "Enter date (YYYY-MM-DD): ";
                std::getline(std::cin, date);
                StockData* stock = stockTree.search(ticker, date);
                if (stock) {
                    std::cout << "\nStock found:\n";
                    displayStock(*stock);
                } else {
                    std::cout << "Stock not found!\n";
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
                std::getline(std::cin, ticker);
                std::cout << "Enter date (YYYY-MM-DD) to remove: ";
                std::getline(std::cin, date);
                if (stockTree.remove(ticker, date)) {
                    std::cout << "Stock data removed successfully.\n";
                } else {
                    std::cout << "Stock not found. Removal failed.\n";
                }
                break;
            }
            case 5: {
                std::vector<StockData> allStocks = stockTree.getAllStocks();
                std::cout << "\nAll stocks (" << allStocks.size() << " entries):\n";
                for (const auto &stock : allStocks) {
                    displayStock(stock);
                }
                break;
            }
            case 6: {
                std::string ticker;
                std::cout << "Enter ticker symbol: ";
                std::getline(std::cin, ticker);
                std::vector<StockData> tickerStocks = stockTree.getStocksByTicker(ticker);
                std::cout << "\nFound " << tickerStocks.size() << " entries for " << ticker << ":\n";
                for (const auto &stock : tickerStocks) {
                    displayStock(stock);
                }
                break;
            }
            case 7: {
                std::string ticker, startDate, endDate;
                std::cout << "Enter ticker symbol: ";
                std::getline(std::cin, ticker);
                std::cout << "Enter start date (YYYY-MM-DD): ";
                std::getline(std::cin, startDate);
                std::cout << "Enter end date (YYYY-MM-DD): ";
                std::getline(std::cin, endDate);
                std::vector<StockData> rangeStocks = stockTree.getStocksByDateRange(ticker, startDate, endDate);
                std::cout << "\nFound " << rangeStocks.size() << " entries between " 
                          << startDate << " and " << endDate << ":\n";
                for (const auto &stock : rangeStocks) {
                    displayStock(stock);
                }
                break;
            }
            case 8:
                exportDataAsCSV(stockTree);
                break;
            case 9: {
                std::vector<std::string> tickers;
                std::string input;
                std::cout << "Enter tickers (comma-separated): ";
                std::getline(std::cin, input);
                size_t pos;
                while ((pos = input.find(',')) != std::string::npos) {
                    tickers.push_back(input.substr(0, pos));
                    input.erase(0, pos + 1);
                }
                tickers.push_back(input);
                auto stocks = stockTree.getMultipleTickers(tickers);
                std::cout << "\nComparison Table:\n";
                printf("%-10s %-12s %-8s %-8s %-8s\n", 
                       "Ticker", "Date", "Close", "SMA(20)", "Volatility");
                for (const auto &s : stocks) {
                    auto tickerStocks = stockTree.getStocksByTicker(s.ticker);
                    printf("%-10s %-12s %-8.2f %-8.2f %-8.2f\n", 
                           s.ticker.c_str(), s.date.c_str(), 
                           s.closePrice, 
                           FinancialMetrics::calculateSMA(tickerStocks, 20),
                           FinancialMetrics::calculateVolatility(tickerStocks));
                }
                break;
            }
            case 10: {
                std::string ticker;
                std::cout << "Enter ticker symbol: ";
                std::getline(std::cin, ticker);
                auto stocks = stockTree.getStocksByTicker(ticker);
                if (!stocks.empty()) {
                    std::cout << "\nFinancial Metrics for " << ticker << ":\n";
                    std::cout << "20-day SMA: " << FinancialMetrics::calculateSMA(stocks, 20) << "\n";
                    std::cout << "50-day EMA: " << FinancialMetrics::calculateEMA(stocks, 50) << "\n";
                    std::cout << "30-day Volatility: " << FinancialMetrics::calculateVolatility(stocks) << "\n";
                } else {
                    std::cout << "No data found for " << ticker << "\n";
                }
                break;
            }
            case 11: {
                std::string ticker, date;
                char action;
                int quantity;
                std::cout << "Enter ticker symbol: ";
                std::getline(std::cin, ticker);
                std::cout << "Enter trade date (YYYY-MM-DD): ";
                std::getline(std::cin, date);
                std::cout << "Buy (B) or Sell (S): ";
                std::cin >> action;
                std::cout << "Enter quantity: ";
                std::cin >> quantity;
                std::cin.ignore();

                StockData* stock = stockTree.search(ticker, date);
                if (stock) {
                    double total = stock->closePrice * quantity;
                    double commission = std::max(5.0, total * 0.01);
                    std::cout << "\nTrade Simulation Results:\n";
                    std::cout << "---------------------------------\n";
                    std::cout << "Ticker:        " << stock->ticker << "\n";
                    std::cout << "Date:          " << stock->date << "\n";
                    std::cout << "Price:         $" << stock->closePrice << "\n";
                    std::cout << "Quantity:      " << quantity << "\n";
                    std::cout << "Commission:    $" << commission << "\n";
                    std::cout << "Net " << (toupper(action) == 'B' ? "Cost" : "Proceeds") 
                              << ":    $" << total + commission << "\n";
                } else {
                    std::cout << "Stock not found for trade simulation\n";
                }
                break;
            }
            case 12:
                std::cout << "Exiting program...\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    return 0;
}
