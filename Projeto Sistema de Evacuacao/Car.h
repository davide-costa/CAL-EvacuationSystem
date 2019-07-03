#pragma once

#include <iostream>

#include "LicensePlate.h"
#include "Coords.h"
#include "Graph.h"

using std::ostream;

class Car
{
	friend bool operator==(const Car &car1, const Car &car2);
	friend ostream &operator<<(ostream &os, const Car &car);

private:
	unsigned int id;
	LicensePlate license_plate;
	int source_node_id;
	int dst_node_id;
	bool path_affected;
	vector<Coords> original_path;
	vector<Coords> alternative_path;

public:
	Car(const LicensePlate &license_plate, int source, int destination);
	Car();
	LicensePlate GetLicensePlate() const;
	int GetSource() const;
	int GetDestination() const;
	vector<Coords> GetOriginalPath() const;
	vector<Coords> GetAlternativePath() const;
	bool IsPathAffected();
	void SetPathAffected();
	void SetPathNotAffected();
	void SetOriginalPath(vector<Coords> path);
	void SetAlternativePath(vector<Coords> path);
};