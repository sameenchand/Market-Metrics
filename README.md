# 📈 Market Metrics - Stock Market Data Analyzer

A comprehensive C++ application for analyzing stock market data with advanced data structures and financial calculations. Built with AVL trees for efficient data management and real-time API integration.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)

## 🚀 Features

### 📊 Data Management
- **AVL Tree Implementation**: Self-balancing binary search tree for O(log n) operations
- **Real-time Data Import**: Fetch live stock data from Stock Data API
- **CSV Import/Export**: Seamless data persistence and backup
- **CRUD Operations**: Create, Read, Update, Delete stock records

### 📈 Financial Analysis
- **Technical Indicators**: 
  - Simple Moving Average (SMA)
  - Exponential Moving Average (EMA)
  - Volatility calculations
- **Price Analysis**: Daily price changes and percentage returns
- **Multi-stock Comparison**: Side-by-side analysis of multiple tickers
- **Date Range Filtering**: Historical data analysis

### 💼 Trading Simulation
- **Trade Calculator**: Simulate buy/sell transactions
- **Commission Calculation**: Realistic trading costs
- **Portfolio Analysis**: Track multiple positions

## 🏗️ Architecture

```
Market-Metrics/
├── main.cpp              # Main application logic and menu system
├── AVLTree.h/.cpp        # Self-balancing binary search tree implementation
├── FinancialMetrics.h    # Financial calculations and indicators
├── ImportStockData.h/.cpp # API integration and data import
├── stocks.csv            # Sample stock data
└── vcpkg.json           # Dependency management
```

### Key Components

- **AVLTree**: Efficient data structure for storing and retrieving stock data
- **FinancialMetrics**: Namespace containing financial calculation functions
- **ImportStockData**: Handles API calls and data fetching
- **StockData**: Core data structure representing stock information

## 🛠️ Installation & Setup

### Prerequisites
- **Visual Studio 2019/2022** (Community edition or higher)
- **Windows 10/11**
- **vcpkg** package manager
- **Stock Data API Key** (free registration required)

### Step-by-Step Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/sameenchand/Market-Metrics.git
   cd Market-Metrics
   ```

2. **Open in Visual Studio**
   - Launch Visual Studio
   - Select "Open a project or solution"
   - Navigate to the project folder
   - Choose "Create a new project from existing code files"
   - Select C++ as the language and "Console application project"

3. **Configure vcpkg**
   - Go to Project Properties → Configuration Properties
   - Select "vcpkg"
   - Under "General", enable "Use Vcpkg manifest"
   - The project will automatically install required dependencies

4. **Set up API Key**
   - Create a free account at [Stock Data](https://www.stockdata.org/)
   - Generate your API key
   - Create a `config.txt` file in the project directory:
     ```
     API_KEY=your_api_key_here
     ```

5. **Build and Run**
   - Press `Ctrl+F5` or go to Debug → Start Without Debugging
   - The application should compile and run successfully

## 📖 Usage

### Main Menu Options

| Option | Description |
|--------|-------------|
| 1 | Import stock data from API |
| 2 | Import stock data from CSV |
| 3 | Search for a specific stock |
| 4 | Update existing stock data |
| 5 | Remove stock data |
| 6 | Display all stocks |
| 7 | Display stocks by ticker |
| 8 | Display stocks by date range |
| 9 | Export data to CSV |
| 10 | Compare multiple stocks |
| 11 | Calculate financial metrics |
| 12 | Simulate trade |
| 13 | Exit |

### Example Workflow

1. **Import Data**: Use option 1 to fetch live data for AAPL
2. **Analyze**: Use option 11 to calculate SMA, EMA, and volatility
3. **Compare**: Use option 10 to compare multiple stocks
4. **Simulate Trade**: Use option 12 to test trading scenarios

## 🔧 Technical Details

### Data Structure
```cpp
struct StockData {
    std::string ticker;      // Stock symbol (e.g., "AAPL")
    std::string date;        // Date in YYYY-MM-DD format
    double openPrice;        // Opening price
    double closePrice;       // Closing price
    double highPrice;        // Highest price
    double lowPrice;         // Lowest price
    long volume;             // Trading volume
};
```

### Performance Characteristics
- **Insertion**: O(log n) average case
- **Search**: O(log n) average case
- **Deletion**: O(log n) average case
- **Memory**: O(n) space complexity

### Dependencies
- **cpprestsdk**: For HTTP client functionality and API integration
- **Standard C++17**: For modern C++ features

## 📊 Sample Data

The project includes `stocks.csv` with sample data in the following format:
```csv
Ticker,Date,Open,Close,High,Low,Volume
AAPL,2024-01-15,185.50,188.25,189.10,185.20,45000000
MSFT,2024-01-15,375.20,378.45,379.80,374.50,32000000
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

**Happy Trading! 📈💰**
