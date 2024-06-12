#ifndef LOCATION_H
#define LOCATION_H

#include <string>

class Location {
public:
    Location(int id, const std::string& name, double latitude, double longitude);
    int getId() const;
    std::string getName() const;
    double getLatitude() const;
    double getLongitude() const;
    void setName(const std::string& name);
    void setLatitude(double latitude);
    void setLongitude(double longitude);

private:
    int id;
    std::string name;
    double latitude;
    double longitude;
};

#endif // LOCATION_H
