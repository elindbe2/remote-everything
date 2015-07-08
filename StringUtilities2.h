/*
 *  StringUtilities.hpp
 *
 *  Created by Gary Woodcock on 11/7/10.
 *  Copyright 2010-12 Twitch LLC. All rights reserved.
 *
 */
// ==============================================================================================================
#pragma once

#ifndef __STRINGUTILITIES_HPP__
#define __STRINGUTILITIES_HPP__

#include "SharedInternal.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include "extended_type_traits.h"

//using namespace std; //pollutes every downstream file with whole std::namespace ...


static	const	std::string  kBlankString = "";
static	const	std::string  kUnimplementedRoutineString  = "Unimplemented routine!";
    
// ==============================================================================================================
// StringUtilities class
// ==============================================================================================================
namespace StringUtilities
{	
    TW_SHARED_API std::vector<std::string>          Split           (const std::string&          inputString, 
                                                                     char                   delimiter = '|');
    TW_SHARED_API std::string				        Join			(std::vector<std::string> const& inputStrings,
						 						                     std::string	const   	  delimiter = "|");
	TW_SHARED_API std::string				        RemoveSpaces    (std::string s);
	
    TW_SHARED_API char                              GetStandardDelimiter();
	

	TW_SHARED_API std::string                  Replace(const std::string&          inputString,
		                                               const std::string&          searchString,
		                                               const std::string&          replaceString);
	TW_SHARED_API std::string                  ByteVectorToString(std::vector<unsigned char>  theVector);
	TW_SHARED_API std::vector<unsigned char>   StringToByteVector(const std::string&          theString);
	TW_SHARED_API unsigned char*               StringToByteArray(const std::string&          theString);
	TW_SHARED_API std::string                  ToLowerCase(const std::string&          theString);

	TW_SHARED_API std::string                  RemoveWhitespaceNearDelimiter(std::string const& s, char const delimiter = '|');

	std::wstring			                   s2ws				(const std::string&		s);
	TW_SHARED_API void ulongToStr(unsigned long n, char *start, unsigned long size);
	TW_SHARED_API void longToStr(long n, char *start, int size);
	TW_SHARED_API void dblToStr( double d, char * start );

	TW_SHARED_API int itoa(int val, char* buf); // not sure we need this...

	TW_SHARED_API void RemoveColonsFromString(char* str);

	TW_SHARED_API std::string trim(const std::string& theString, const char* toBeTrimmed);
};


//toString
template <typename T> //built-in non-enum types
typename std::enable_if<!std::is_enum<T>::value &&
					    !is_iterable<T>::value &&
						!is_pair<T>::value, std::string>::type toString(T t)
{
	try
	{
        std::stringstream ss;
        ss << t;
        return ss.str();
	}
	catch (std::exception e)
	{
		 throw std::exception("toString<generic>: bad cast");
	}

	 return "";
}

template<> inline
std::string toString<double>(double t)
{
	try
	{
        if (_isnan(t))
        {
            return "nan";
	}
        else
        { 
            std::stringstream ss;
            ss << t;
            return ss.str();
        }
    }
	catch (std::exception e)
	{
        throw std::exception("toString<generic>: bad cast");
	}
}

template <typename T> //enums
typename std::enable_if<std::is_enum<T>::value, std::string>::type toString(T t)
{
	short const s = static_cast<short>(t);
	try
	{
        std::stringstream ss;
        ss << s;
        return ss.str();
	}
	catch (std::exception e)
	{
		throw std::exception("toString<enum>: bad cast");
	}

	return "";
}

template <typename T> //enums
typename std::enable_if<is_iterable<T>::value, std::string>::type toString(T t)
{
	auto str_vec = std::vector<std::string>();
	str_vec.resize(t.size());
	std::transform(t.begin(), t.end(), str_vec.begin(), toString<T::value_type>);
	return StringUtilities::Join(str_vec);
}

template <typename T> //enums
typename std::enable_if<is_pair<T>::value, std::string>::type toString(T t)
{
	auto str = toString<T::first_type>(t.first) + "#" +
					toString<T::second_type>(t.second);
	return str;
}


template <size_t N>
std::string toString(char(&s)[N])
{
	return std::string(s);
}

template <size_t N>
std::string toString(char const (&s)[N])
{
	return std::string(s);
}

TW_SHARED_API std::string toString(char const* const s);

TW_SHARED_API std::string toString(bool t);

// This just convets one double, for an array of doubles this will not work.
// I suggest making a wrapper type containing the array (or use a vector of doubles)
TW_SHARED_API std::string toString(double* d);

//fromString
template <typename T> //built-in non-enum types
typename std::enable_if<!std::is_enum<T>::value && 
						!is_output_iterator<T>::value &&
						!is_insertable<T>::value &&
						!is_pair<T>::value, T>::type fromString(std::string const & value)
{
	if (value.empty())
		throw std::exception("Empty string value");

    std::istringstream ss(value);
    T ret;
    if (ss >> ret)
		return ret;
    else
        throw std::exception("Invalid string value");
}

template <typename T> //enums
typename std::enable_if<std::is_enum<T>::value, T>::type fromString(std::string const & value)
{
	short s = 0;
	try
	{
        s = (short) atoi(value.c_str());
	}
	catch (std::exception e)
	{
		throw std::exception("fromString<enum>: bad cast");
	}
	return static_cast<T>(s);
}

template <typename T> //enums
typename std::enable_if<is_output_iterator<T>::value, T>::type fromString(std::string const & str)
{
    if (str == "<<NONE>>") return T(); // our convention for an empty set/list as sent over the wire.

	auto str_vec = StringUtilities::Split(str);
    if (str_vec.front() == "values") str_vec.erase(str_vec.begin());

	auto ret_vec = T();

    for (auto const & s : str_vec)
        ret_vec.push_back(fromString<T::value_type>(s));
	return ret_vec;
}

template <typename T> //enums
typename std::enable_if<is_insertable<T>::value, T>::type fromString(std::string const & str)
{
    if (str == "<<NONE>>") return T(); // our convention for an empty set/list as sent over the wire.

	auto str_vec = StringUtilities::Split(str);
    if (str_vec.front() == "values") str_vec.erase(str_vec.begin());

	auto ret = T();

	for (auto const& s : str_vec)
	{
		ret.insert(fromString<T::value_type>(s));
	}

	return ret;
}

template <typename T> //enums
typename std::enable_if<is_pair<T>::value, T>::type fromString(std::string const & str)
{
	if (str.empty()) return T();

	auto str_vec = StringUtilities::Split(str, '#');
	return std::make_pair(	fromString<std::remove_const<T::first_type>::type>(str_vec.at(0)),
							fromString<T::second_type>(str_vec.at(1)));
}

template <>
TW_SHARED_API std::string fromString<std::string>(std::string const & value);

template <> //bool
TW_SHARED_API bool fromString<bool>(std::string const & value);

//template <> // map<string,string>
//TW_SHARED_API std::map<std::string, std::string> fromString<std::map<std::string, std::string>>(std::string const & value);

template <> 
TW_SHARED_API char * fromString<char *>(std::string const& value);

template <typename K, typename V>
std::map<K, V> fromString(std::string const & value)
{
	// map<K,V> stringified format is "values|k1#v1|k2#v2|..."
	std::vector<std::string> tokens = StringUtilities::Split(value, '|');
	if (tokens.front() == "values") tokens.erase(tokens.begin());
	map<K, V> result;
	for (auto token : tokens)
	{
		std::vector<std::string> sub_tokens = StringUtilities::Split(token, '#');
		if (sub_tokens.size() == 2)
		{
			result.insert(make_pair(fromString<K>(sub_tokens.at(0)), fromString<V>(sub_tokens.at(1))));
		}
	}
	return result;
}

template <> // map<string,string>
TW_SHARED_API std::map<std::string, std::string> fromString<std::map<std::string, std::string>>(std::string const & value);

template <> 
TW_SHARED_API double  fromString<double >(std::string const& value);


int itoa(int val, char* buf); // not sure we need this...
void RemoveColonsFromString(char* str);

//use these with boost::fusion::for_each type loop
struct StringBuilder
{
	std::string message_;
	template <typename T>
	void operator()(T t)
	{
		message_ += ("|" + toString(t));
	}
	StringBuilder(std::string const & messagePrefix) :message_(messagePrefix){}
	std::string getMessage(){ return (message_ + "|"); }
};

struct StructBuilder
{
	std::vector<std::string> tokens_;
	size_t                   index_;
	template <typename T>
	void operator()(T & t)
	{
		t = fromString<T>(tokens_[index_++]);
	}
	StructBuilder(std::vector<std::string> const & tokens) :tokens_(tokens), index_(0){}
};

// called by templated code after fromString is used to release any memory that
// fromString allocates.  fromString generally calls new for pointer types, so
// this function is needed to call delete.  If you call new, you many need to
// specialize this function in order to call delete.
template <typename T>
void deallocatePreviousFromString(T t)
{
	// do nothing
}

TW_SHARED_API void deallocatePreviousFromString(char* cp);
TW_SHARED_API void deallocatePreviousFromString(double* cp);


// ==============================================================================================================
#endif	// __STRINGUTILITIES_HPP__
// ==============================================================================================================
