#pragma once

#include <vector>
#include <string>

using namespace std;


void ComputePrefixFunction(vector<int> &pi, string prefix);
bool StringMatcher(string pattern, string text);
unsigned int PartialStringMatcher(string pattern, string text);