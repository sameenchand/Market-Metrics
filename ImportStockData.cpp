#include "ImportStockData.h"

std::string GET_API_KEY(const std::string& filePath) {
	std::ifstream config(filePath);
	std::string API_KEY;

	std::getline(config, API_KEY);
	API_KEY = API_KEY.substr(8); // Remove "API_KEY=" prefix

	config.close();
	return API_KEY;
}

void importData(AVLTree& stockTree, std::string ticker) {
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