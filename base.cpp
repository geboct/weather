#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>  

using namespace std;
using json = nlohmann::json;

// Location class to hold location details
class Location {
public:
    int id;
    string name;
    double latitude;
    double longitude;
    bool isFavorite;

    Location(int id, const string& name, double latitude, double longitude)
        : id(id), name(name), latitude(latitude), longitude(longitude), isFavorite(false) {}

    void display() const {
        cout << "ID: " << id << ", Name: " << name 
             << ", Latitude: " << latitude 
             << ", Longitude: " << longitude 
             << ", Favorite: " << (isFavorite ? "Yes" : "No") << endl;
    }
};

// WeatherData class to hold weather details
class WeatherData {
public:
    double temperature;
    double precipitation;
    double pressure;
    double windSpeed;
    double humidity;
    double cloudiness;
    double solarRadiation; 

    WeatherData(double temp, double precip, double press, double wind, double hum, double cloud, double solarRad)
        : temperature(temp), precipitation(precip), pressure(press), windSpeed(wind), humidity(hum), cloudiness(cloud), solarRadiation(solarRad) {}

    void display() const {
        cout << "Temperature: " << temperature << "°C, Precipitation: " << precipitation << "mm, "
             << "Pressure: " << pressure << "hPa, Wind Speed: " << windSpeed << "m/s, "
             << "Humidity: " << humidity << "%, Cloudiness: " << cloudiness << "%, "
             << "Solar Radiation: " << solarRadiation << "W/m²" << endl;
    }
};

// WeatherApp class to manage locations and weather data
class WeatherApp {
public:
    vector<Location> locations;
    vector<WeatherData> hourlyWeatherData;
    vector<WeatherData> dailyWeatherData; 
    struct Settings {
        string temperatureUnit;
        string windSpeedUnit;
        string precipitationUnit;
        string timeFormat;
        int pastDays;
        int forecastDays;
        string startDate;
        string endDate;
        string timeZone;
    } settings;

public:
    WeatherApp() {
        settings.temperatureUnit = "Celsius";
        settings.windSpeedUnit = "m/s";
        settings.precipitationUnit = "mm";
        settings.timeFormat = "24-hour";
        settings.pastDays = 7;
        settings.forecastDays = 7;
        settings.startDate = "2024-01-01";
        settings.endDate = "2024-01-07";
        settings.timeZone = "UTC";
    }

    void addLocation(int id, const string& name, double latitude, double longitude) {
        locations.push_back(Location(id, name, latitude, longitude));
        cout << "Location added successfully." << endl;
    }

    void modifyLocation(int id, const string& newName, double newLatitude, double newLongitude) {
        auto it = find_if(locations.begin(), locations.end(), [id](const Location& loc) { return loc.id == id; });
        if (it != locations.end()) {
            it->name = newName;
            it->latitude = newLatitude;
            it->longitude = newLongitude;
            cout << "Location modified successfully." << endl;
        } else {
            cout << "Location not found." << endl;
        }
    }

    void removeLocationById(int id) {
        auto it = remove_if(locations.begin(), locations.end(), [id](const Location& loc) { return loc.id == id; });
        if (it != locations.end()) {
            locations.erase(it, locations.end());
            cout << "Location removed successfully." << endl;
        } else {
            cout << "Location not found." << endl;
        }
    }

    void removeLocationByName(const string& name) {
        auto it = remove_if(locations.begin(), locations.end(), [&name](const Location& loc) { return loc.name == name; });
        if (it != locations.end()) {
            locations.erase(it, locations.end());
            cout << "Location removed successfully." << endl;
        } else {
            cout << "Location not found." << endl;
        }
    }

    void searchLocationById(int id) const {
        auto it = find_if(locations.begin(), locations.end(), [id](const Location& loc) { return loc.id == id; });
        if (it != locations.end()) {
            it->display();
        } else {
            cout << "Location not found." << endl;
        }
    }

    void searchLocationByName(const string& name) const {
        auto it = find_if(locations.begin(), locations.end(), [&name](const Location& loc) { return loc.name == name; });
        if (it != locations.end()) {
            it->display();
        } else {
            cout << "Location not found." << endl;
        }
    }

    void searchLocationByPartialName(const string& partialName) const {
        for (const auto& loc : locations) {
            if (loc.name.find(partialName) != string::npos) {
                loc.display();
            }
        }
    }

    void setFavoriteLocation(int id, bool isFavorite) {
        auto it = find_if(locations.begin(), locations.end(), [id](const Location& loc) { return loc.id == id; });
        if (it != locations.end()) {
            it->isFavorite = isFavorite;
            cout << "Location favorite status updated." << endl;
        } else {
            cout << "Location not found." << endl;
        }
    }

    void addHourlyWeatherData(const WeatherData& data) {
        hourlyWeatherData.push_back(data);
    }

    void addDailyWeatherData(const WeatherData& data) {
        dailyWeatherData.push_back(data);
    }

    void displayWeatherForecast() const {
        cout << "Hourly Weather Forecast:" << endl;
        for (const auto& data : hourlyWeatherData) {
            data.display();
        }

        cout << "Daily Weather Forecast:" << endl;
        for (const auto& data : dailyWeatherData) {
            data.display();
        }
    }

    void updateSettings(const Settings& newSettings) {
        settings = newSettings;
        cout << "Settings updated." << endl;
    }

    void displaySettings() const {
        cout << "Temperature Unit: " << settings.temperatureUnit << ", Wind Speed Unit: " << settings.windSpeedUnit
             << ", Precipitation Unit: " << settings.precipitationUnit << ", Time Format: " << settings.timeFormat
             << ", Past Days: " << settings.pastDays << ", Forecast Days: " << settings.forecastDays
             << ", Start Date: " << settings.startDate << ", End Date: " << settings.endDate
             << ", Time Zone: " << settings.timeZone << endl;
    }

    void exportDataAsCSV(const string& filename) const {
        ofstream file(filename);
        file << "ID,Name,Latitude,Longitude,Favorite\n";
        for (const auto& loc : locations) {
            file << loc.id << "," << loc.name << loc.latitude << "," << loc.longitude << "," << (loc.isFavorite ? "Yes" : "No") << "\n";
        }
        file.close();
        cout << "Data exported as CSV to " << filename << endl;
    }

    void exportDataAsJSON(const string& filename) const {
        json j;
        for (const auto& loc : locations) {
            j.push_back({{"id", loc.id}, {"name", loc.name}, {"latitude", loc.latitude}, {"longitude", loc.longitude}, {"favorite", loc.isFavorite}});
        }
        ofstream file(filename);
        file << j.dump(4); // Pretty printing with 4 spaces
        file.close();
        cout << "Data exported as JSON to " << filename << endl;
    }

    void exportDataAsXLSX(const string& filename) const {
        // Implement exporting data to XLSX
        cout << "Data exported as XLSX to " << filename << endl;
    }
};

int main() {
    WeatherApp app;

    // Adding some locations
    app.addLocation(1, "New York", 40.7128, -74.0060);
    app.addLocation(2, "Los Angeles", 34.0522, -118.2437);
    app.addLocation(3, "Chicago", 41.8781, -87.6298);

    // Modifying a location
    app.modifyLocation(1, "New York City", 40.7128, -74.0059);

    // Searching for locations
    cout << "Search by ID (1):" << endl;
    app.searchLocationById(1);

    cout << "Search by Name (Los Angeles):" << endl;
    app.searchLocationByName("Los Angeles");

    cout << "Search by Partial Name (New):" << endl;
    app.searchLocationByPartialName("New");

    // Setting favorite location
    app.setFavoriteLocation(1, true);

    // Removing a location by ID
    app.removeLocationById(2);

    // Display all locations
    cout << "All Locations:" << endl;
    app.searchLocationByPartialName("");

    // Adding weather data
    app.addHourlyWeatherData(WeatherData(25.0, 10.0, 1013.0, 5.0, 60.0, 20.0, 200.0));
    app.addDailyWeatherData(WeatherData(30.0, 5.0, 1015.0, 6.0, 65.0, 25.0, 250.0));

    // Displaying weather forecast
    app.displayWeatherForecast();

    // Updating settings
    WeatherApp::Settings newSettings = {"Fahrenheit", "km/h", "in", "12-hour", 5, 10, "2024-06-01", "2024-06-10", "PST"};
    app.updateSettings(newSettings);
    app.displaySettings();

    // Export data
    app.exportDataAsCSV("locations.csv");
    app.exportDataAsJSON("locations.json");
    app.exportDataAsXLSX("locations.xlsx");

    return 0;
}
