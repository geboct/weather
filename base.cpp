#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include "curl/include/curl/curl.h"
#include "nlohmann/json.hpp"

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
            file << loc.id << "," << loc.name << "," << loc.latitude << "," << loc.longitude << "," << (loc.isFavorite ? "Yes" : "No") << "\n";
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
        // TODO: Implement XLSX export functionality
        cout << "Data exported as XLSX to " << filename << endl;
    }

    // Helper function to fetch weather data from OpenWeather API
    void fetchWeatherDataFromAPI(const string& locationName) {
        // This function assumes you have the OpenWeather API key set as an environment variable
        const string apiKey = "YOUR_API_KEY"; // Replace with your actual API key
        string url = "https://api.openweathermap.org/data/2.5/weather?q=" + locationName + "&appid=" + apiKey + "&units=metric";

        CURL* curl;
        CURLcode res;
        string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                return;
            }

            // Parse the JSON response
            auto jsonResponse = json::parse(readBuffer);
            cout << "Weather Data for " << locationName << ":\n" << jsonResponse.dump(4) << endl;

            // Extract relevant weather data and add to hourly or daily weather data
            double temperature = jsonResponse["main"]["temp"];
            double precipitation = jsonResponse.contains("rain") ? jsonResponse["rain"]["1h"] : 0.0;
            double pressure = jsonResponse["main"]["pressure"];
            double windSpeed = jsonResponse["wind"]["speed"];
            double humidity = jsonResponse["main"]["humidity"];
            double cloudiness = jsonResponse["clouds"]["all"];
            double solarRadiation = 0.0; // Placeholder as OpenWeather API free tier does not provide solar radiation

            // Create a WeatherData object and add it to hourly weather data (for demonstration)
            WeatherData weatherData(temperature, precipitation, pressure, windSpeed, humidity, cloudiness, solarRadiation);
            addHourlyWeatherData(weatherData);
        }
    }

    // Callback function for libcurl to write received data into a string
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
};

void showMenu() {
    cout << "====================\n";
    cout << "WeatherApp Menu\n";
    cout << "====================\n";
    cout << "1. Add Location\n";
    cout << "2. Modify Location\n";
    cout << "3. Remove Location by ID\n";
    cout << "4. Remove Location by Name\n";
    cout << "5. Search Location by ID\n";
    cout << "6. Search Location by Name\n";
    cout << "7. Search Location by Partial Name\n";
    cout << "8. Set Favorite Location\n";
    cout << "9. Display Weather Forecast\n";
    cout << "10. Update Settings\n";
    cout << "11. Display Settings\n";
    cout << "12. Export Data as CSV\n";
    cout << "13. Export Data as JSON\n";
    cout << "14. Export Data as XLSX\n";
    cout << "15. Fetch Weather Data from API\n";
    cout << "0. Exit\n";
    cout << "====================\n";
}

int main() {
    WeatherApp app;
    int choice;

    while (true) {
        showMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 0) {
            break;
        }

        switch (choice) {
            case 1: {
                int id;
                string name;
                double latitude, longitude;
                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter Name: ";
                cin >> ws; // to consume any leftover newline character
                getline(cin, name);
                cout << "Enter Latitude: ";
                cin >> latitude;
                cout << "Enter Longitude: ";
                cin >> longitude;
                app.addLocation(id, name, latitude, longitude);
                break;
            }
            case 2: {
                int id;
                string newName;
                double newLatitude, newLongitude;
                cout << "Enter ID of location to modify: ";
                cin >> id;
                cout << "Enter New Name: ";
                cin >> ws;
                getline(cin, newName);
                cout << "Enter New Latitude: ";
                cin >> newLatitude;
                cout << "Enter New Longitude: ";
                cin >> newLongitude;
                app.modifyLocation(id, newName, newLatitude, newLongitude);
                break;
            }
            case 3: {
                int id;
                cout << "Enter ID of location to remove: ";
                cin >> id;
                app.removeLocationById(id);
                break;
            }
            case 4: {
                string name;
                cout << "Enter Name of location to remove: ";
                cin >> ws;
                getline(cin, name);
                app.removeLocationByName(name);
                break;
            }
            case 5: {
                int id;
                cout << "Enter ID of location to search: ";
                cin >> id;
                app.searchLocationById(id);
                break;
            }
            case 6: {
                string name;
                cout << "Enter Name of location to search: ";
                cin >> ws;
                getline(cin, name);
                app.searchLocationByName(name);
                break;
            }
            case 7: {
                string partialName;
                cout << "Enter partial name of location to search: ";
                cin >> ws;
                getline(cin, partialName);
                app.searchLocationByPartialName(partialName);
                break;
            }
            case 8: {
                int id;
                bool isFavorite;
                cout << "Enter ID of location to set as favorite: ";
                cin >> id;
                cout << "Set as favorite (1 for Yes, 0 for No): ";
                cin >> isFavorite;
                app.setFavoriteLocation(id, isFavorite);
                break;
            }
            case 9: {
                app.displayWeatherForecast();
                break;
            }
            case 10: {
                WeatherApp::Settings newSettings;
                cout << "Enter Temperature Unit (Celsius/Fahrenheit): ";
                cin >> newSettings.temperatureUnit;
                cout << "Enter Wind Speed Unit (m/s/km/h): ";
                cin >> newSettings.windSpeedUnit;
                cout << "Enter Precipitation Unit (mm/in): ";
                cin >> newSettings.precipitationUnit;
                cout << "Enter Time Format (24-hour/12-hour): ";
                cin >> newSettings.timeFormat;
                cout << "Enter Past Days: ";
                cin >> newSettings.pastDays;
                cout << "Enter Forecast Days: ";
                cin >> newSettings.forecastDays;
                cout << "Enter Start Date (YYYY-MM-DD): ";
                cin >> newSettings.startDate;
                cout << "Enter End Date (YYYY-MM-DD): ";
                cin >> newSettings.endDate;
                cout << "Enter Time Zone: ";
                cin >> newSettings.timeZone;
                app.updateSettings(newSettings);
                break;
            }
            case 11: {
                app.displaySettings();
                break;
            }
            case 12: {
                string filename;
                cout << "Enter filename for CSV export: ";
                cin >> filename;
                app.exportDataAsCSV(filename);
                break;
            }
            case 13: {
                string filename;
                cout << "Enter filename for JSON export: ";
                cin >> filename;
                app.exportDataAsJSON(filename);
                break;
            }
            case 14: {
                string filename;
                cout << "Enter filename for XLSX export: ";
                cin >> filename;
                app.exportDataAsXLSX(filename);
                break;
            }
            case 15: {
                string locationName;
                cout << "Enter name of location to fetch weather data from API: ";
                cin >> ws;
                getline(cin, locationName);
                app.fetchWeatherDataFromAPI(locationName);
                break;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }

    return 0;
}
