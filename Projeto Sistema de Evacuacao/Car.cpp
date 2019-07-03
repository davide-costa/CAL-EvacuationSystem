#include "Car.h"

//---------------------------------------Class Car---------------------------------------
Car::Car(const LicensePlate &license_plate, int source, int destination)
{
	static unsigned int curr_id = 0;
	id = curr_id;
	this->license_plate = license_plate;
	this->source_node_id = source;
	this->dst_node_id = destination;
}
//------------------------------------------------------------------------------
Car::Car()
{

}

LicensePlate Car::GetLicensePlate() const
{
	return license_plate;
}

int Car::GetSource() const
{
	return source_node_id;
}

int Car::GetDestination() const
{
	return dst_node_id;
}

vector<Coords> Car::GetOriginalPath() const
{
	return original_path;
}

vector<Coords> Car::GetAlternativePath() const
{
	return alternative_path;
}

bool Car::IsPathAffected()
{
	return path_affected;
}

void Car::SetPathAffected()
{
	path_affected = true;
}

void Car::SetPathNotAffected()
{
	path_affected = false;
}

void Car::SetOriginalPath(vector<Coords> path)
{
	this->original_path = path;
}

void Car::SetAlternativePath(vector<Coords> path)
{
	this->alternative_path = path;
}

//---------------------------------------Operator Overloads---------------------------------------
bool operator==(const Car & car1, const Car & car2)
{
	return car1.license_plate == car2.license_plate;
}
//------------------------------------------------------------------------------
ostream &operator<<(ostream &os, const Car &car)
{
	os << car.license_plate;
	return os;
}
