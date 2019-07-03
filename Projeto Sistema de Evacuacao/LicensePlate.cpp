#include "LicensePlate.h"
#include "utils.h"
#include <algorithm>

//---------------------------------------Class LicensePlate---------------------------------------
LicensePlate::LicensePlate(string first, string second, string third, unsigned char month, unsigned int year)
{
	this->first = first;
	this->second = second;
	this->third = third;
	this->month = month;
	this->year = year;
}
//------------------------------------------------------------------------------
LicensePlate::LicensePlate()
{

}
//------------------------------------------------------------------------------
const string& LicensePlate::GetFirst() const 
{
	return first;
}
//------------------------------------------------------------------------------
const string& LicensePlate::GetSecond() const
{
	return second;
}
//------------------------------------------------------------------------------
const string& LicensePlate::GetThird() const
{
	return third;
}
//------------------------------------------------------------------------------
unsigned char LicensePlate::GetMonth() const
{
	return month;
}
//------------------------------------------------------------------------------
unsigned int LicensePlate::GetYear() const
{
	return year;
}
//------------------------------------------------------------------------------
void LicensePlate::SetFirst(const string& first) 
{
	this->first = first;
}
//------------------------------------------------------------------------------
void LicensePlate::SetSecond(const string& second) 
{
	this->second = second;
}
//------------------------------------------------------------------------------
void LicensePlate::SetThird(const string& third)
{
	this->third = third;
}
//------------------------------------------------------------------------------
void LicensePlate::SetMonth(unsigned char month)
{
	this->month = month;
}
//------------------------------------------------------------------------------
void LicensePlate::SetYear(unsigned int year) 
{
	this->year = year;
}
//---------------------------------------Operator Overloads---------------------------------------
//------------------------------------------------------------------------------
bool operator==(const LicensePlate &license_plate1, const LicensePlate &license_plate2)
{
	if (license_plate1.first != license_plate2.first)
		return false;

	if (license_plate1.second != license_plate2.second)
		return false;

	if (license_plate1.third != license_plate2.third)
		return false;

	return true;
}
//------------------------------------------------------------------------------
bool operator!=(const LicensePlate & license_plate1, const LicensePlate & license_plate2)
{
	return !(license_plate1 == license_plate2);
}
//------------------------------------------------------------------------------
bool operator<(const LicensePlate &license_plate1, const LicensePlate &license_plate2)
{
	if (license_plate1.GetYear() < license_plate1.GetYear())
		return true;

	if (license_plate1.GetYear() > license_plate1.GetYear())
		return false;

	if (license_plate1.GetMonth() < license_plate1.GetMonth())
		return true;

	if (license_plate1.GetMonth() > license_plate1.GetMonth())
		return false;

	return false;
}
//------------------------------------------------------------------------------
ostream &operator<<(ostream &os, const LicensePlate license_plate)
{
	//In order for the setw to work properly, an entire string must be sent (all at once) to the ostream
	ostringstream license_plate_oss;
	string neat_month;
	if (license_plate.month < 10)
		neat_month = "0";
	license_plate_oss << license_plate.first << "-" << license_plate.second << "-" << license_plate.third << " " << neat_month << license_plate.month << "/" << license_plate.year;
	string license_plate_str = license_plate_oss.str();
	os << license_plate_str;

	return os;
}
//------------------------------------------------------------------------------
istream &operator>>(istream &is, LicensePlate &license_plate)
{
	char ch;
	getline(is, license_plate.first, '-');
	RemoveSpacesFromWord(license_plate.first);
	getline(is, license_plate.second, '-');
	RemoveSpacesFromWord(license_plate.second);
	getline(is, license_plate.third, ' ');
	is >> license_plate.month >> ch;
	is >> license_plate.year;

	transform(license_plate.first.begin(), license_plate.first.end(), license_plate.first.begin(), toupper);
	transform(license_plate.second.begin(), license_plate.second.end(), license_plate.second.begin(), toupper);
	transform(license_plate.third.begin(), license_plate.third.end(), license_plate.third.begin(), toupper);

	if (is.fail())
		throw LicensePlate::ErrorReadingLicensePlate();

	if (!IsLicensePlateValid(license_plate))
		throw LicensePlate::InvalidLicensePlate();

	return is;
}
//------------------------------------------------------------------------------
//Receives a license plate and verifies its validity; returns true if the license plate is valid; returns false otherwise
bool IsLicensePlateValid(const LicensePlate &license_plate)
{
	if (license_plate.GetFirst().length() != 2)
		return false;

	if (license_plate.GetSecond().length() != 2)
		return false;

	if (license_plate.GetThird().length() != 2)
		return false;

	if (license_plate.GetYear() <= 0)
		return false;

	if (license_plate.GetMonth() <= 0 || license_plate.GetMonth() > 12)
		return false;

	return true;
}