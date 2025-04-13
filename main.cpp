#include "AVLTree.h"
#include <iostream>
#include <iomanip>
#include <string>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/asyncrt_utils.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;

std::string GET_API_KEY(const std::string& filePath) {
    std::ifstream config(filePath);
    std::string API_KEY;

    std::getline(config, API_KEY);
	API_KEY = API_KEY.substr(8); // Remove "API_KEY=" prefix

	config.close();

	return API_KEY;
}

void displayMenu() {
    std::cout << "===== Stock Market Data Analyzer =====\n";
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

void importData(AVLTree &stockTree, std::string ticker) {
    // Fetch from config file
    std::string configData = GET_API_KEY("config.txt");
    char* API_KEY = new char[configData.length() + 1];
    std::strcpy(API_KEY, configData.c_str());

    char* symbol = new char[ticker.length() + 1];
    std::strcpy(symbol, ticker.c_str());
    
    // Create the URI
    uri_builder builder(U("https://api.stockdata.org/v1/data/eod"));
    builder.append_query(U("symbols"), symbol);
    builder.append_query(U("api_token"), API_KEY);
    builder.append_query(U("date_from"), ("2025-01-01")); // Date to get stocks from onward.

    // Make the HTTP request
    http_client client(builder.to_uri());

    client.request(methods::GET).then([&stockTree, &ticker](http_response response) {
        if (response.status_code() == status_codes::OK) {
            // Parse the JSON response
            json::value json_response = response.extract_json().get();

            auto data = json_response[U("data")].as_array();
            if (data.size() != 0) {
                for (auto& entry : data) {
                    std::string dateTime = utility::conversions::to_utf8string(entry[U("date")].as_string());
                    std::string date = dateTime.substr(0, dateTime.find('T')); // Extract only YYYY-MM-DD

                    double open = entry[U("open")].as_double();
                    double close = entry[U("close")].as_double();
                    double high = entry[U("high")].as_double();
                    double low = entry[U("low")].as_double();
                    int volume = entry[U("volume")].as_integer();

                    StockData temp(ticker, date, open, close, high, low, volume);
                    stockTree.insert(temp);
                }
                std::cout << "Stock data inserted successfully.\n";
            } 
            else {
                std::cout << "No stock data found.\n" << std::endl;
            }
        }
        else {
            std::cerr << "Error: " << response.status_code() << std::endl;
        }
        }).wait();
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
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }

        char temp;
        std::cin.get(temp);

        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		system("cls"); // Clear console
    }

    return 0;
}
