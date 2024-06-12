#include <iostream>
#include <string>
#include "WeatherDataApplication.h"
// int main() {
//     std::string apiKey = "0888ef6505339c6bf654fde9bbd01b3e";
//     WeatherDataApplication app(apiKey);

//     app.addLocation(1, "Accra", 40.7128, -74.0060);
//     app.addLocation(2, "Takoradi", 34.0522, -118.2437);

//     app.setFavouriteLocation(1);

//     app.fetchWeatherForecast(1);
//     app.displayWeatherData();

//     app.exportWeatherDataToCSV("weather_data.csv");
//     app.exportWeatherDataToJSON("weather_data.json");

//     return 0;
// }





int main() {
    std::string apiKey = "0888ef6505339c6bf654fde9bbd01b3e";
    WeatherDataApplication app(apiKey);

    // Add some locations
    app.addLocation(1, "Takoradi", 40.7128, -74.0060);
    app.addLocation(2, "Sunyani", 51.5074, -0.1278);
    app.addLocation(3, "Accra", 35.6895, 139.6917);

    // Search for a location by name
    std::string searchName;
    std::cout << "Enter location name to search: ";
    std::cin >> searchName;
    app.searchLocationByName(searchName);

    // Fetch weather data for a specific location
    int locationId;
    std::cout << "Enter location ID to fetch weather data: ";
    std::cin >> locationId;
    app.fetchWeatherForecast(locationId);

    // Display weather data
    app.displayWeatherData();

    // Export weather data to CSV
    std::string csvFilename;
    std::cout << "Enter CSV filename to export weather data: ";
    std::cin >> csvFilename;
    app.exportWeatherDataToCSV(csvFilename);

    // Export weather data to JSON
    std::string jsonFilename;
    std::cout << "Enter JSON filename to export weather data: ";
    std::cin >> jsonFilename;
    app.exportWeatherDataToJSON(jsonFilename);

    return 0;
}
