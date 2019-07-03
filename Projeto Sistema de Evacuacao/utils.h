/**
*
* @file
* @brief Contains all functions declarations that we consider of general utility.
*
*/

#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

#include <sstream>
#include <Windows.h>
#include <algorithm>
#include <unordered_set>

#include "LicensePlate.h"
#include "Coords.h"
#include "Car.h"

using namespace std;

/**
*
* @brief Clears the console windows screen, i. e., erases all the characters on it.
*
*/
void ClearScreen();
/**
*
* @brief Removes all the spaces (character ' ') from a phrase. Since it is a frase, it leaves one space between each word (but no more than 1).
* @param phrase The phrase from which to remove the spaces from. (passed by reference to act also as a return)
*
*/
void RemoveSpacesFromPhrase(string &phrase);
/**
*
* @brief Removes all the spaces (character ' ') from a word. Since it is a word, it removes every ' ' from the string, because a word must not have spaces between its characters.
* @param phrase The word from which to remove the spaces from. (passed by reference to act also as a return)
*
*/
void RemoveSpacesFromWord(string &word);
/**
*
* @brief Verifies if two characters are equal, case insensitively.
* @param a The first character to verify
* @param b The second character to verify
* @return true if they are equal; false if they are not equal.
*
*/
bool CharsAreEqualCaseInsensitive(unsigned char a, unsigned char b);
/**
*
* @brief Verifies if two strings are equal, case insensitively. (uses the CharsAreEqualCaseInsensitive function).
* @param a The first string to verify
* @param b The second string to verify
* @return true if they are equal; false if they are not equal.
*
*/
bool StringsAreEqualCaseInsensitive(string a, string b);
/**
*
* @brief Reads an unsigned int from the user. Does not return until the user introduces a valid input, i. e., an input that doesn't cause a failure in the cin stream. Keeps asking for input until while it doesn't have the desired one. Ignores all remaining input to ensure that if the user introduced something that is not part of the wanted input, it won't compromise the next attempt to read information from the user.
* @param error_msg The appropriate error message to display when it fails to read the input from the user. Since this functions is generic, it can be called from any context. It recieves this pameter to show an error message that is appropriate in the context it was called from.
* @return The unsigned int value read from the user.
*
*/
unsigned int ReadUnsignedInt(string error_msg);
/**
*
* @brief Reads a string from the user. Does not return until the user introduces a valid input, i. e., an input that doesn't cause a failure in the cin stream. Keeps asking for input until while it doesn't have the desired one.
* @param error_msg The appropriate error message to display when it fails to read the input from the user. Since this functions is generic, it can be called from any context. It recieves this pameter to show an error message that is appropriate in the context it was called from.
* @return The string object read from the user.
*
*/
string ReadString(string error_msg);
/**
*
* @brief Reads a floating point number from the user. Does not return until the user introduces a valid input, i. e., an input that doesn't cause a failure in the cin stream. Keeps asking for input until while it doesn't have the desired one. Ignores all remaining input to ensure that if the user introduced something that is not part of the wanted input, it won't compromise the next attempt to read information from the user.
* @param error_msg The appropriate error message to display when it fails to read the input from the user. Since this functions is generic, it can be called from any context. It recieves this pameter to show an error message that is appropriate in the context it was called from.
* @return The floating point number read from the user.
*
*/
float ReadFloat(string error_msg);
/**
*
* @brief Verifies if a LicensePlate object is valid. (something like ABC-43-32 is not valid, or one of many other bad formats).
* @param license_plate The LicensePlate object to verify the validity of
* @return true if the LicensePlate is valid; false if it is not
*
*/
bool IsLicensePlateValid(const LicensePlate &license_plate);
/**
*
* @brief Converts the coordinates it receives as parameter from polar to cartesian form.
* @param coords The Coords object to be converted. Converts the coords that the object represents and sets according to the new format (returns by reference).
*
*/
void PolarToCartesianCoords(Coords &coords);
/**
*
* @brief Calculates the linear distance between to coordinates in cartesian format. It receives them in the form of two Coords objects.
* @param coords_1 The first Coords object to calculate the distance.
* @param coords_2 The second Coords object to calculate the distance.
* @return The distance linear between the two coordinates (represented by first and second Coords objects).
*
*/
double DistanceBetCartesianCoords(const Coords &coords_1, const Coords &coords_2);
void GetWordsFromAString(string text, vector<string> &words, unordered_set<string> &words_not_to_include = unordered_set<string>());