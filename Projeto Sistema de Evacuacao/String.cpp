#include "String.h"
#include <algorithm>

void ComputePrefixFunction(vector<int> &pi, string prefix)
{
	pi.clear();
	unsigned int curr_num = 0;
	pi.push_back(curr_num);//first element

	for (int i = 1; i < prefix.length(); i++)
	{
		if (prefix[i] == prefix[curr_num])
		{
			curr_num++;
			pi.push_back(curr_num);
		}
		else
		{
			curr_num = 0;
			pi.push_back(curr_num);
		}
	}
}


bool StringMatcher(string pattern, string text)
{
	vector<int> pi;
	ComputePrefixFunction(pi, pattern);
	unsigned int pattern_length = pattern.length();

	//for case insensitive search, more efficient apllied to our program
	transform(text.begin(), text.end(), text.begin(), tolower);
	transform(pattern.begin(), pattern.end(), pattern.begin(), tolower);

	unsigned int num_elements_equal = 0;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == pattern[num_elements_equal])
			num_elements_equal++;
		else
			num_elements_equal = pi[num_elements_equal];

		if (num_elements_equal == pattern_length)
			return true;
	}

	return false;
}

unsigned int PartialStringMatcher(string pattern, string text)
{
	int n = text.length();
	vector<int> d(n + 1);
	int old, neww;

	//for case insensitive search, more efficient apllied to our program
	transform(text.begin(), text.end(), text.begin(), tolower);
	transform(pattern.begin(), pattern.end(), pattern.begin(), tolower);

	for (int j = 0; j <= n; j++)
		d[j] = j;

	int m = pattern.length();
	for (int i = 1; i <= m; i++)
	{
		old = d[0];
		d[0] = i;
		for (int j = 1; j <= n; j++) 
		{
			if (pattern[i - 1] == text[j - 1]) 
				neww = old;
			else 
			{
				neww = min(old, d[j]);
				neww = min(neww, d[j - 1]);
				neww = neww + 1;
			}
			old = d[j];
			d[j] = neww;
		}
	}

	return d[n];
}
