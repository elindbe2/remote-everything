#pragma once

#include "RepeatDefines.h"
#include "DBMgr.h"
#include "StringUtilities.hpp"

// Create my defines used to generate the rest of the code 

#ifdef ENABLE_INVOCATION
#define ARG_TO_SSTREAM( num ) ArgToSStream( ss, arg_names[num-1], a##num );
#define GET_LOG_ARG(num) ,T##num a##num

#define DEFINE_PARAM_LOGGER( num )\
template <RCN##num( typename T )> \
void LogParams##num( void* /*klassp*/, std::string function, std::string args RF##num(GET_LOG_ARG) )\
{\
	args.erase(std::remove_if(args.begin(),args.end(),isspace), args.end());\
	std::vector<std::string> arg_names = StringUtilities::Split(args,',');\
	std::stringstream ss;\
	ss<<"PRCT_INVOCATION="<<num + 1<<"\001";\
	ss<< function << "=" << num << "\001";\
	RF##num( ARG_TO_SSTREAM )\
	Log(ss.str().c_str()); \
}

void LogParams0( void* klassp, std::string function  );

#define SAVE_INVOCATION( NARGS, ... ) LogParams##NARGS( this, __FUNCTION__, #__VA_ARGS__, ##__VA_ARGS__ );
#define SAVE_INVOCATION0 LogParams0( this,  __FUNCTION__ );

#else
#define SAVE_INVOCATION( NARGS, ... )
#define DEFINE_PARAM_LOGGER( num )
#endif

// Create my invocation save code
template <typename T> 
void ArgToSStream( std::ostream& os, std::string name, T arg )
{
	os << name << "=" << arg <<"\001";
}

template <typename T> 
void ArgToSStream( std::ostream& os, std::string name, T*& arg )
{
	if(arg != NULL)
		os << name << "=" << *arg <<"\001";
	else
		os << name << "=" << "\001";
}

template <> 
void ArgToSStream<char>( std::ostream& os, std::string name, char*& arg );

template <>	
TW_TRADING_API
void ArgToSStream<const char>( std::ostream& os, std::string name, const char*& arg );

// TODO: FIX THIS!
//template <>
//void ArgToSStream<YieldAdjustmentInfo*>( std::ostream& os, std::string name, YieldAdjustmentInfo* arg );
//
//template <>
//void ArgToSStream<YieldAdjustmentInfo*&>( std::ostream& os, std::string name, YieldAdjustmentInfo*& arg );
//
//template <>
//void ArgToSStream<YieldAdjustmentInfo&>( std::ostream& os, std::string name, YieldAdjustmentInfo& arg );

DEFINE_PARAM_LOGGER( 1 )
DEFINE_PARAM_LOGGER( 2 )
DEFINE_PARAM_LOGGER( 3 )
DEFINE_PARAM_LOGGER( 4 )
DEFINE_PARAM_LOGGER( 5 )
DEFINE_PARAM_LOGGER( 6 )
DEFINE_PARAM_LOGGER( 7 )
DEFINE_PARAM_LOGGER( 8 )
DEFINE_PARAM_LOGGER( 9 )
DEFINE_PARAM_LOGGER( 10 )
DEFINE_PARAM_LOGGER( 11 )
DEFINE_PARAM_LOGGER( 12 )
DEFINE_PARAM_LOGGER( 13 )
DEFINE_PARAM_LOGGER( 14 )
DEFINE_PARAM_LOGGER( 15 )
DEFINE_PARAM_LOGGER( 16 )
DEFINE_PARAM_LOGGER( 17 )
DEFINE_PARAM_LOGGER( 18 )
DEFINE_PARAM_LOGGER( 19 )
DEFINE_PARAM_LOGGER( 20 )
DEFINE_PARAM_LOGGER( 21 )
DEFINE_PARAM_LOGGER( 22 )
DEFINE_PARAM_LOGGER( 23 )
DEFINE_PARAM_LOGGER( 24 )
DEFINE_PARAM_LOGGER( 25 )
DEFINE_PARAM_LOGGER( 26 )
DEFINE_PARAM_LOGGER( 27 )
DEFINE_PARAM_LOGGER( 28 )
DEFINE_PARAM_LOGGER( 29 )


TW_TRADING_API void Log(std::string message);

unsigned long ParseTestInstanceID(std::string message);
void CreateProxyFnLogTableIfNotExists(const char* table_name);
void StartUpDB(const char* dbHost, const char* dbName, const char* dbLogin, const char* dbPassword, void* param5);