#include "utils.h"

#define EARTH_RADIUS 6370000

//Clears the console
void ClearScreen()
{
	COORD upperLeftCorner = { 0,0 };
	DWORD charsWritten;
	DWORD conSize;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO  csbi;

	GetConsoleScreenBufferInfo(hCon, &csbi);
	conSize = csbi.dwSize.X * csbi.dwSize.Y;

	// fill with spaces
	FillConsoleOutputCharacter(hCon, TEXT(' '), conSize, upperLeftCorner, &charsWritten);
	GetConsoleScreenBufferInfo(hCon, &csbi);
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, conSize, upperLeftCorner, &charsWritten);

	// cursor to upper left corner
	SetConsoleCursorPosition(hCon, upperLeftCorner);
}
//------------------------------------------------------------------------------
//Receives a phrase and removes all the unnecessary spaces from it
void RemoveSpacesFromPhrase(string &phrase)
{
	//If the recieved string is empty, do nothing to it and return immediately
	if (phrase.length() == 0)
		return;

	//Remove all spaces until the first non-space char is reached; if the string gets emptied during the removal process, return
	while (phrase[0] == ' ')
	{
		phrase.erase(0, 1);
		if (phrase.length() == 0)
			return;
	}

	//Remove the next unncessary spaces in the middle of the text until the pre-last char of the string is reached
	int last_pos = (int)phrase.length() - 1;
	for (int i = 0; i < last_pos; i++)
		if ((phrase[i] == ' ') && (phrase[i + 1] == ' '))
		{
			phrase.erase(i, 1);
			i--;
			last_pos--;
		}

	//Remove all the spaces in the end of the string
	if (phrase[phrase.length() - 1] == ' ')
		phrase.erase(phrase.length() - 1, 1);
}
//------------------------------------------------------------------------------
//Receives a word and removes all the unnecessary spaces from it
void RemoveSpacesFromWord(string &word)
{
	for (unsigned int i = 0; i < word.length(); i++)
		if (word[i] == ' ')
		{
			word.erase(i, 1);
			i--;
		}
}
//------------------------------------------------------------------------------
//Compares two chars case insensitively
bool CharsAreEqualCaseInsensitive(unsigned char a, unsigned char b)
{
	return tolower(a) == tolower(b);
}
//------------------------------------------------------------------------------
//Compares two chars case insensitively
bool StringsAreEqualCaseInsensitive(string a, string b)
{
	if (a.length() != b.length())
		return false;

	return equal(a.begin(), a.end(), b.begin(), CharsAreEqualCaseInsensitive);
}
//-----------------------------------------Input Validation Functions------------------------------------------------
//Reads a value of type double from user
//Does not return anything while the user does not introduce the intended input
//Has a call_id to display the appropriate error message, depending on what type of data it was called to read
unsigned int ReadUnsignedInt(string error_msg)
{
	int i;
	cin >> i;
	while (cin.fail())
	{
		if (cin.eof())
			cin.clear();
		else
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
		cout << endl << error_msg;
		cin >> i;
	}
	//Ignore the next characters inputed, to make sure the cin buffer is clean before being used by another function
	cin.ignore(INT_MAX, '\n');
	return i;
}
//------------------------------------------------------------------------------
//Reads a value of type string from user
//Does not return anything while the user does not introduce the intended input
//Has a call_id to display the appropriate error message, depending on what type of data it was called to read
string ReadString(string error_msg)
{
	string str;
	getline(cin, str);
	RemoveSpacesFromPhrase(str);
	while (cin.fail() || str.empty())
	{
		if (cin.fail())
			if (cin.eof())
				cin.clear();
			else
			{
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
		cout << endl << error_msg;
		getline(cin, str);
		RemoveSpacesFromPhrase(str);
	}
	return str;
}
//------------------------------------------------------------------------------
//Reads a value of type double from user
//Does not return anything while the user does not introduce the intended input
//Has a call_id to display the appropriate error message, depending on what type of data it was called to read
float ReadFloat(string error_msg)
{
	float flt;
	cin >> flt;
	while (cin.fail())
	{
		if (cin.eof())
			cin.clear();
		else
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
		cout << endl << error_msg;
		cin >> flt;
	}
	//Ignore the next characters inputed, to make sure the cin buffer is clean before being used by another function
	cin.ignore(INT_MAX, '\n');
	return flt;
}
//------------------------------------------------------------------------------
void PolarToCartesianCoords(Coords &coords)
{
	double longitude = coords.GetX();
	double latitude = coords.GetY();

	double cart_x = EARTH_RADIUS*cos(latitude)*sin(longitude);
	double cart_y = EARTH_RADIUS*sin(latitude);
	coords.SetX(cart_x);
	coords.SetY(cart_y);
}

double DistanceBetCartesianCoords(const Coords &coords_1, const Coords &coords_2)
{
	double dif_x = coords_2.GetX() - coords_1.GetX();
	double dif_y = coords_2.GetY() - coords_1.GetY();

	return sqrt(dif_x * dif_x + dif_y * dif_y);
}

void GetWordsFromAString(string text, vector<string>& words, unordered_set<string> &words_not_to_include)
{
	istringstream iss(text);
	while (!iss.eof())
	{
		string temp;
		iss >> temp;
		if (!temp.empty())
			if(words_not_to_include.find(temp) == words_not_to_include.end())
				words.push_back(temp);
	}
}
