#pragma once
#include <vector>
#include <cmath>

namespace FinancialMetrics {
    inline double calculateSMA(const std::vector<StockData>& stocks, int period) {
        if (stocks.size() < period) return 0;
        double sum = 0;
        for (int i = 0; i < period; i++)
            sum += stocks[i].closePrice;
        return sum / period;
    }

    inline double calculateEMA(const std::vector<StockData>& stocks, int period) {
        if (stocks.size() < period) return 0;
        double multiplier = 2.0 / (period + 1);
        double ema = calculateSMA(stocks, period);
        for (size_t i = period; i < stocks.size(); i++)
            ema = (stocks[i].closePrice * multiplier) + ema * (1 - multiplier);
        return ema;
    }

    inline double calculateVolatility(const std::vector<StockData>& stocks) {
        if (stocks.empty()) return 0;
        double mean = calculateSMA(stocks, stocks.size());
        double variance = 0;
        for (const auto& s : stocks)
            variance += pow(s.closePrice - mean, 2);
        return sqrt(variance / stocks.size());
    }

    inline double dailyPriceChange(double open, double close) {
        return close - open;
    }

    inline double percentageReturn(double initial, double current) {
        return ((current - initial) / initial) * 100;
    }
}
