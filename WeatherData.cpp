#include "WeatherData.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

using json = nlohmann::json; // Add this line to bring `json` into scope

// Define the static variables for storing the received data
static std::string weatherBuffer;
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

WeatherData::WeatherData(const std::string &apiKey) : apiKey(apiKey) {}

void WeatherData::fetchWeatherForecast(const Location &location) {
    CURL *curl;
    CURLcode res;
    std::string url = "http://api.openweathermap.org/data/2.5/forecast?lat=" + 
                      std::to_string(location.getLatitude()) + "&lon=" + 
                      std::to_string(location.getLongitude()) + "&appid=" + apiKey;

    curl = curl_easy_init();
    if(curl) {
        weatherBuffer.clear();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &weatherBuffer);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
        parseWeatherData(weatherBuffer);
    }
}

void WeatherData::parseWeatherData(const std::string &data) {
    auto jsonData = json::parse(data);
    weatherData.clear();
    for (const auto &item : jsonData["list"]) {
        weatherData["temperature"].push_back(std::to_string(item["main"]["temp"].get<double>()));
        weatherData["pressure"].push_back(std::to_string(item["main"]["pressure"].get<int>()));
        weatherData["humidity"].push_back(std::to_string(item["main"]["humidity"].get<int>()));
        weatherData["wind_speed"].push_back(std::to_string(item["wind"]["speed"].get<double>()));
        weatherData["clouds"].push_back(std::to_string(item["clouds"]["all"].get<int>()));
    }
}

void WeatherData::displayWeatherData() const {
    std::cout << "Weather Data:" << std::endl;
    for (const auto& data : weatherData) {
        std::cout << data.first << ": ";
        for (const auto& value : data.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

void WeatherData::exportToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    for (const auto& [key, value] : weatherData) {
        file << key << ",";
        for (const auto& val : value) {
            file << val << ",";
        }
        file << "\n";
    }
    file.close();
}

void WeatherData::exportToJSON(const std::string &filename) const {
    json j;
    for (const auto &[key, values] : weatherData) {
        j[key] = values;
    }

    std::ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4);  // Indent JSON with 4 spaces
        file.close();
    } else {
        std::cerr << "Unable to open file for writing: " << filename << std::endl;
    }
}

std::string WeatherData::makeApiRequest(const std::string& url) const {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* ptr, size_t size, size_t nmemb, std::string* data) {
            data->append((char*) ptr, size * nmemb);
            return size * nmemb;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    if (res != CURLE_OK) {
        throw std::runtime_error("Failed to make API request: " + std::string(curl_easy_strerror(res)));
    }

    return response;
}
