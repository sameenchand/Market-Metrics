#include "AVLTree.h"
#include "ImportStockData.h"
#include <iostream>
#include <iomanip>
#include <string>

void displayMenu() {
    std::cout << "===== Stock Market Data Analyzer =====\n";
    std::cout << "1. Insert new stock data\n";
    std::cout << "2. Search for a stock\n";
    std::cout << "3. Update stock data\n";
    std::cout << "4. Remove stock data\n";
    std::cout << "5. Display all stocks\n";
    std::cout << "6. Display stocks by ticker\n";
    std::cout << "7. Display stocks by date range\n";
    std::cout << "8. Export to File\n";
    std::cout << "9. Exit\n";
    std::cout << "Enter your choice (1-9): ";
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

void exportDataAsCSV(AVLTree stockTree) {
    std::ofstream outFile("stocks.csv");
    if (outFile.is_open()) {
        outFile << "Ticker,Date,Open,Close,High,Low,Volume\n";
        std::vector<StockData> allStocks = stockTree.getAllStocks();
        for (const auto& stock : allStocks) {
            outFile << stock.ticker << "," << stock.date << ","
                << stock.openPrice << "," << stock.closePrice << ","
                << stock.highPrice << "," << stock.lowPrice << ","
                << stock.volume << "\n";
        }
        outFile.close();
        std::cout << "Data exported to stocks.csv successfully.\n";
    }
    else {
        std::cout << "Error opening file for export.\n";
    }
}

int main() {
    AVLTree stockTree;
    int choice;

    while (true) {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string ticker;
            std::cout << "Enter ticker symbol: ";
            std::cin >> ticker;
            importData(stockTree, ticker); // Import data from API
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
                std::cout << "\nStock found:\n";
                displayStock(*stock);
            }
            else {
                std::cout << "Stock not found.\n";
            }
            break;
        }
        case 3: {
            StockData updatedStock = inputStockData();
            if (stockTree.update(updatedStock)) {
                std::cout << "Stock data updated successfully.\n";
            }
            else {
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
            }
            else {
                std::cout << "Stock not found. Removal failed.\n";
            }
            break;
        }
        case 5: {
            std::vector<StockData> allStocks = stockTree.getAllStocks();
            std::cout << "\nAll stocks:\n";
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

            std::cout << "\nStocks between " << startDate << " and " << endDate << ":\n";
            for (const auto& stock : rangeStocks) {
                displayStock(stock);
            }
            break;
        }
        case 8:
			exportDataAsCSV(stockTree);
			break;
        case 9:
			std::cout << "Exiting the program...\n";
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }

        // Wait for user input before clearing the screen
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

        // Clear the console screen
		system("cls");
    }
    return 0;
}
