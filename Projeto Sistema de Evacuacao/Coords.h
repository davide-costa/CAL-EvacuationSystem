#pragma once

#include <string>

using namespace std;

class Coords
{
	friend bool operator==(const Coords &c1, const Coords &c2);
	friend bool operator!=(const Coords &c1, const Coords &c2);
	friend ostream &operator<<(ostream &os, const Coords coords);
private:
	int id;
	int x;
	int y;
public:
	Coords();
	Coords(int x, int y, int id);
	int GetId();
	void SetX(int x);
	int GetX() const;
	void SetY(int y);
	int GetY() const;
};

