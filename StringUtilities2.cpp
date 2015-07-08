/*
 *  StringUtilities.cpp
 *
 *  Created by Gary Woodcock on 11/7/10.
 *  Copyright 2010-12 Twitch LLC. All rights reserved.
 *
 */
// ==============================================================================================================
#include "StringUtilities.hpp"
#include <sstream>
#include <algorithm>
#include <string.h>
#include <assert.h>

#if WIN32
	#include <windows.h>
#endif

using std::string;
using std::vector;
using std::map;
using std::stringstream;
using std::runtime_error;

//this seems to be a little faster than the standard version
#define dabs( d ) ( (d) < (0) ? (d*-1) : d )

// ==============================================================================================================
vector<string> StringUtilities::Split(const string& inputString, 
                                      char delimiter) 
{
	// This is a really stupid and crude splitter that works primarily for what 
	// is needed by the database code - beware of using it too generally because 
	// it's not very good (though it's reasonably simple and fast).
	vector<string> elements;
	string s;
	stringstream ss(inputString);

	while (getline(ss, s, delimiter))
	{
		elements.push_back(s);
	}
  
	return elements;
}

string	 StringUtilities::Join(vector<string> const& inputStrings,
								   string	const   	  delimiter )
{
	if(inputStrings.size() == 0)
		return "";

	std::string s(inputStrings.at(0));

	for(int i = 1; i < inputStrings.size(); ++i)
	{
		s += delimiter;
		s += inputStrings.at(i);
	}
	return s;
}

string StringUtilities::RemoveSpaces(string s)
{
	s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
	return s;
}

char StringUtilities::GetStandardDelimiter() 
{ 
    return '|'; 
}
// ==============================================================================================================
string StringUtilities::Replace(const string& inputString, 
                                const string& searchString, 
                                const string& replaceString)
{
	string result(kBlankString);
	if (!inputString.empty() && !searchString.empty())
	{
		result.assign(inputString);
		size_t position;
		
		while ((position = result.find(searchString)) != string::npos)
			result.replace(position, searchString.size(), replaceString);		
	}
	return result;
}

// ==============================================================================================================
string StringUtilities::ByteVectorToString(vector<unsigned char> theVector)
{
	string theString(theVector.begin(), theVector.end());
	return theString;
}

// ==============================================================================================================
vector<unsigned char> StringUtilities::StringToByteVector(const string& theString)
{
	vector<unsigned char> theVector(theString.begin(), theString.end());
	return theVector;
}

// ==============================================================================================================
unsigned char* StringUtilities::StringToByteArray(const string& theString)
{
	unsigned char* theArray = NULL;
	if (theString.length() > 0)
	{
		theArray = (unsigned char*)malloc(theString.length());
		if (theArray != NULL)
			memcpy(theArray, theString.data(), theString.length());
	}
	return theArray;
}

// ==============================================================================================================
string StringUtilities::ToLowerCase(const string& theString)
{
	string str(theString);
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

// ==============================================================================================================

std::wstring StringUtilities::s2ws(const std::string& s)
{
	 int len;
	 int slength = (int)s.length() + 1;
	 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	 wchar_t* buf = new wchar_t[len];
	 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	 std::wstring r(buf);
	 delete[] buf;
	 return r;
}

void StringUtilities::ulongToStr(unsigned long n, char *start, unsigned long size)
{
       char* p = start + size-1;
       for (unsigned long i = 0; i < size; ++i)
       {
              *(p) = '0' + (n % 10);
              n /= 10;
              --p;
       }
}

void StringUtilities::longToStr(long n, char *start, int size)
{
	if( n < 0 )
	{
		start[0] = '-';
		n = n*-1;
		ulongToStr( n, start+1, size-1 );
	}
	else ulongToStr( n, start, size );
}

void StringUtilities::dblToStr( double d, char * start )
{  
	   assert(static_cast<long>(dabs(d)) < LONG_MAX);
       //Find decimal
       int i = 0;
       while( start[i] != '.' )
              ++i;

       // print left of decimal
       int x = (int) d;
       longToStr( x, start, i );
	   if( x == 0 && d < 0.0 ) start[0] = '-';

       // set start to after decimal
       start = start + i + 1;
       d = d - x;

       // find end, shift the digits of d over to right of decimal
       i = 0;
       while( start[i] != '\001' )
       {
              d *= 10;
              ++i;
       }
       //--i;
       //d /= 10;
       // print right of decimal
       unsigned int y = (unsigned int)(dabs(d) + 0.5);
       ulongToStr( y, start, i );
}

int
StringUtilities::itoa(int val, char* buf)
{
    const unsigned int radix = 10;

    char* p;
    unsigned int a;        //every digit
    int len;
    char* b;            //start of the digit char
    char temp;
    unsigned int u;

    p = buf;

    if (val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }
    u = (unsigned int)val;

    b = p;

    do
    {
        a = u % radix;
        u /= radix;

        *p++ = (char)(a + '0');

    } while (u > 0);

    len = (int)(p - buf);

    *p-- = 0;

    //swap
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;

    } while (b < p);

    return len;
}

void StringUtilities::RemoveColonsFromString(char* str)
{
	char* colonPos = strstr(str, ":");
	while (colonPos != NULL)
	{
		colonPos[0] = '.';
		colonPos = strstr(colonPos+1, ":");
	}
}

string StringUtilities::trim(const string& theString, const char* toBeTrimmed)
{
    if (theString.empty())
        return theString;

    size_t start = theString.find_first_not_of(toBeTrimmed);
    size_t end = theString.find_last_not_of(toBeTrimmed);

    return theString.substr(start, (end - start) + 1);

}

string toString(char const * const s)
{
	return string(s);
}

string toString(bool t)
{
	return ((t) ? ("1") : ("0"));
}


//fromString

template<> // string
string fromString<string>(string const & value)
{
    return value;
}

template <> //bool
bool fromString<bool>(string const & value)
{
	if (value == "true" || value == "1") return true;
	else                return false;
}

template <>
char * fromString<char *>(std::string const& value)
{
	char * cp = new char[value.size()+1]; // create a new string of the appropriate size
	memcpy(cp, value.data(), value.size()); // copy the string contents
	cp[value.size()] = '\0'; // set last character to null
	return cp;
}

void deallocatePreviousFromString(char* cp)
{
	delete[] cp;
}

// array of doubles
std::string toString(double* d)
{
	if (!d) return "";

	return toString<double>(*d);
}

template<>
double fromString<double>(std::string const& value)
{
	if( value == "nan" )
		return std::numeric_limits<double>::quiet_NaN();

	double d;
	std::stringstream ss;
	ss << value;
	ss >> d;

	return d;
}

template<>
double * fromString<double *>(std::string const& value)
{
	double * d = new double;
	*d = fromString<double>(value);
	return d;
}

void deallocatePreviousFromString(double* d)
{
	delete d;
}

template <> // map<string,string>
map<string, string> fromString<map<string, string>>(string const & value)
{
	// map<K,T> stringified format is "values|k1#v1|k2#v2|..."
	vector<string> tokens = StringUtilities::Split(value, '|');
	if (tokens.front() == "values") tokens.erase(tokens.begin());

	map<string, string> result;
	for (auto token : tokens)
	{
		vector<string> sub_tokens = StringUtilities::Split(token, '#');
		if (sub_tokens.size() == 2)
		{
			result.insert(make_pair(sub_tokens.at(0), sub_tokens.at(1)));
		}
	}
	return result;
}

// ==============================================================================================================
