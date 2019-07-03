#include "Coords.h"

#include <sstream>

Coords::Coords()
{

}

Coords::Coords(int x, int y, int id)
{
	this->id = id;
	this->x = x;
	this->y = y;
}

int Coords::GetId()
{
	return id;
}

void Coords::SetX(int x)
{
	this->x = x;
}

int Coords::GetX() const
{
	return x;
}

void Coords::SetY(int y)
{
	this->y = y;
}

int Coords::GetY() const
{
	return y;
}

bool operator==(const Coords & c1, const Coords & c2)
{
	return c1.id == c2.id;
}

bool operator!=(const Coords & c1, const Coords & c2)
{
	return !(c1 == c2);
}

ostream & operator<<(ostream & os, const Coords coords)
{
	ostringstream temp_oss;
	temp_oss << "( " << coords.x << ", " << coords.y << " )";
	os << temp_oss.str();
	return os;
}
