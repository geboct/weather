#ifndef WEATHERDATAAPPLICATION_H
#define WEATHERDATAAPPLICATION_H

#include <vector>
#include "Location.h"
#include "WeatherData.h"

class WeatherDataApplication {
public:
    WeatherDataApplication(const std::string& apiKey);
    void addLocation(int id, const std::string& name, double latitude, double longitude);
    void modifyLocation(int id, const std::string& name, double latitude, double longitude);
    void removeLocation(int id);
    void searchLocationByName(const std::string& name) const;
    void setFavouriteLocation(int id);
    void fetchWeatherForecast(int id);
    void displayWeatherData() const;
    void exportWeatherDataToCSV(const std::string& filename) const;
    void exportWeatherDataToJSON(const std::string& filename) const;

private:
    std::vector<Location> locations;
    WeatherData weatherData;
    Location* favouriteLocation;
};

#endif // WEATHERDATAAPPLICATION_H
