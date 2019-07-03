/**
*
* @file
* @brief Contains class declarations necessary to represent a car's license plate
*
*/


#pragma once

#include <string>

using namespace std;

/**
*
* @brief Represents a license plate. (In portuguese format)
*
*/

class LicensePlate
{
	/**
	*
	* @brief Compares two LicensePlate objects. A first LicensePlate object is equal to the second if the three groups of characters are exactly the same
	* @param license_plate1 The first LicensePlate object to be compared.
	* @param license_plate2 The second LicensePlate object to be compared.
	* @return a bool value indicating whether the first is equal to the second or not.
	*
	*/
	friend bool operator==(const LicensePlate &license_plate1, const LicensePlate &license_plate2);
	/**
	*
	* @brief Compares two LicensePlate objects. A first LicensePlate object is different than the second if at least one of the three groups of the characters is different (it returns the opposite of the == operator)
	* @param license_plate1 The first LicensePlate object to be compared.
	* @param license_plate2 The second LicensePlate object to be compared.
	* @return a bool value indicating whether the first is different than the second or not.
	*
	*/
	friend bool operator!=(const LicensePlate &license_plate1, const LicensePlate &license_plate2);
	/**
	*
	* @brief Compares two LicensePlate objects. A first LicensePlate object is less than the second if the month and year of the first respect to a previous date then the second
	* @param license_plate1 The first LicensePlate object to be compared.
	* @param license_plate2 The second LicensePlate object to be compared.
	* @return a bool value indicating whether the first is lower than the second or not.
	*
	*/
	friend bool operator<(const LicensePlate &license_plate1, const LicensePlate &license_plate2);
	/**
	*
	* @brief Outputs a LicensePlate to an ostream
	* @param os The ostream to output to.
	* @param license_plate The LicensePlate to output to the ostream.
	* @return the ostream after outputing the LicensePlate.
	*
	*/
	friend ostream &operator<<(ostream &os, const LicensePlate license_plate);
	/**
	*
	* @brief Extracts a LicensePlate from an istream
	* @param is The istream to input from.
	* @param license_plate The LicensePlate to input from the istream.
	* @return the istream after inputting the LicensePlate.
	*
	*/
	friend istream &operator>>(istream &is, LicensePlate &license_plate);
private:
	/**
	*
	* @brief The first group of 2 characters of a portuguese license plate
	*
	*/
	string first;
	/**
	*
	* @brief The second group of 2 characters of a portuguese license plate
	*
	*/
	string second;
	/**
	*
	* @brief The third group of 2 characters of a portuguese license plate
	*
	*/
	string third;
	/**
	*
	* @brief The month of a portuguese license plate
	*
	*/
	unsigned int month;
	/**
	*
	* @brief The year of a portuguese license plate
	*
	*/
	unsigned int year;
public:
	/**
	*
	* @brief Exception thrown when extracting a license plate from a stream fails (using the overloaded operator>>).
	*
	*/
	class ErrorReadingLicensePlate {};
	/**
	*
	* @brief Exception thrown when an invalid license plate is read from a stream (using the overloaded operator>>).
	*
	*/
	class InvalidLicensePlate {};
	/**
	*
	* @brief Creates a new LicensePlate object based on either information loaded from the file or read from the user. Recieves all the attributes of a LicensePlate object.
	*
	*/
	LicensePlate(string first, string second, string third, unsigned char month, unsigned int year);
	/**
	*
	* @brief Default constructor.
	*
	*/
	LicensePlate();
	const string& GetFirst() const;
	const string& GetSecond() const;
	const string& GetThird() const;
	unsigned char GetMonth() const;
	unsigned int GetYear() const;
	void SetFirst(const string& first);
	void SetSecond(const string& second);
	void SetThird(const string& third);
	void SetMonth(unsigned char month);
	void SetYear(unsigned int year);
};