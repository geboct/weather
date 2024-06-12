#include "WeatherDataApplication.h"
#include <algorithm>
#include <iostream>

WeatherDataApplication::WeatherDataApplication(const std::string& apiKey)
    : weatherData(apiKey), favouriteLocation(nullptr) {}

void WeatherDataApplication::addLocation(int id, const std::string& name, double latitude, double longitude) {
    locations.emplace_back(id, name, latitude, longitude);
}

void WeatherDataApplication::modifyLocation(int id, const std::string& name, double latitude, double longitude) {
    auto it = std::find_if(locations.begin(), locations.end(), [id](const Location& loc) { return loc.getId() == id; });
    if (it != locations.end()) {
        it->setName(name);
        it->setLatitude(latitude);
        it->setLongitude(longitude);
    }
}

void WeatherDataApplication::removeLocation(int id) {
    locations.erase(std::remove_if(locations.begin(), locations.end(), [id](const Location& loc) { return loc.getId() == id; }), locations.end());
}

void WeatherDataApplication::searchLocationByName(const std::string& name) const {
    auto it = std::find_if(locations.begin(), locations.end(), [&name](const Location& loc) { return loc.getName() == name; });
    if (it != locations.end()) {
        std::cout << "Location found: " << it->getName() << std::endl;
    } else {
        std::cout << "Location not found" << std::endl;
    }
}

void WeatherDataApplication::setFavouriteLocation(int id) {
    auto it = std::find_if(locations.begin(), locations.end(), [id](const Location& loc) { return loc.getId() == id; });
    if (it != locations.end()) {
        favouriteLocation = &(*it);
    }
}

void WeatherDataApplication::fetchWeatherForecast(int id) {
    auto it = std::find_if(locations.begin(), locations.end(), [id](const Location& loc) { return loc.getId() == id; });
    if (it != locations.end()) {
        weatherData.fetchWeatherForecast(*it);
    }
}

void WeatherDataApplication::displayWeatherData() const {
    weatherData.displayWeatherData();
}

void WeatherDataApplication::exportWeatherDataToCSV(const std::string& filename) const {
    weatherData.exportToCSV(filename);
}

void WeatherDataApplication::exportWeatherDataToJSON(const std::string& filename) const {
    weatherData.exportToJSON(filename);
}
