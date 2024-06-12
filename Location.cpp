#include "Location.h"

Location::Location(int id, const std::string& name, double latitude, double longitude)
    : id(id), name(name), latitude(latitude), longitude(longitude) {}

int Location::getId() const { return id; }
std::string Location::getName() const { return name; }
double Location::getLatitude() const { return latitude; }
double Location::getLongitude() const { return longitude; }

void Location::setName(const std::string& name) { this->name = name; }
void Location::setLatitude(double latitude) { this->latitude = latitude; }
void Location::setLongitude(double longitude) { this->longitude = longitude; }
