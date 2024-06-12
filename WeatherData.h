#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <string>
#include <vector>
#include <map>
#include "Location.h"

class WeatherData {
public:
    WeatherData(const std::string& apiKey);
    void fetchWeatherForecast(const Location& location);
    void fetchHistoricalWeather(const Location& location);
    void displayWeatherData() const;
    void exportToCSV(const std::string& filename) const;
    void exportToJSON(const std::string& filename) const;

private:
    std::string apiKey;
    std::map<std::string, std::vector<std::string>> weatherData;

    std::string makeApiRequest(const std::string& url) const;
    void parseWeatherData(const std::string& jsonData);
};

#endif // WEATHERDATA_H
