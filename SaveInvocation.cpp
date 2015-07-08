#include "SaveInvocation.h"
#include "Logging.hpp"


DBMgr sTraderDBMgr;
Lockable sLockStatic;
// TODO: FIX THIS!
//template <>
//void ArgToSStream<YieldAdjustmentInfo*>( std::ostream& os, std::string name, YieldAdjustmentInfo* arg )
//{
//	os << name << "=" << arg <<"\001";
//}
//
//template <>
//void ArgToSStream<YieldAdjustmentInfo*&>( std::ostream& os, std::string name, YieldAdjustmentInfo*& arg )
//{
//	os << name << "=" << arg <<"\001";
//}
//
//template <>
//void ArgToSStream<YieldAdjustmentInfo&>( std::ostream& os, std::string name, YieldAdjustmentInfo& arg )
//{
//	os << name << "=" << &arg <<"\001";
//}

void LogParams0( void* klassp, std::string function  )
{
	std::stringstream ss;
	ss<<"PRCT_INVOCATION="<< 1<<"\001";
	ss<< function << "=" << 0 << "\001";
	LOG_INFO(ss.str().c_str());
}

template <> 
void ArgToSStream<char>( std::ostream& os, std::string name, char*& arg )
{
	if(arg != NULL)
		os << name << "=" << std::string(arg) <<"\001";

	else
		os << name << "=" << "\001";
}

template <>	
void ArgToSStream<const char>( std::ostream& os, std::string name, const char*& arg )
{
	if(arg != NULL)
		os << name << "=" << std::string(arg) <<"\001";
	else
		os << name << "=" << "\001";
}

void Log(std::string message)
{

	sLockStatic.Lock();

	TimestampString timeStampStr;
	GetTimeStampString(timeStampStr, true, true, true);

	unsigned long testInstanceID = ParseTestInstanceID(message);

	char query[32 * 1024];
	_snprintf(query, sizeof(query)-1, "Insert into proxyfncalls (timestamp, test_instance_id, message) VALUES('%s', %d, '%s')",
		&(timeStampStr[0]), testInstanceID, message.c_str());
	query[strlen(query)] = 0;

	if (sTraderDBMgr.IsStarted())
		sTraderDBMgr.PostSQL(query);
	//else
		//assert(false && "DBMgr is not started!"); // commented out as it's in gui thread and seems to cause problems...
													// specifically, when db goes down and user cxls order in mfc client...

	sLockStatic.Unlock();
}

unsigned long ParseTestInstanceID(std::string message)
{
	unsigned long result = 0;

	vector<string> vars = StringUtilities::Split(message, '\001');

	vector<string> testInstanceIDVec = StringUtilities::Split(vars[2], '=');	//var[0] is the PRCT_INVOCATION key var[1] is the function name and we make var[2] the testInstanceID
	if (testInstanceIDVec[0] == "testInstanceID")
	{
		string testInstanceID = testInstanceIDVec[1];
		sscanf(testInstanceID.c_str(), "%u", &result);
	}
	else
	{
		assert(false);
	}

	return result;

}

void StartUpDB(const char* dbHost, const char* dbName, const char* dbLogin, const char* dbPassword, void* param5)
{
	sTraderDBMgr.Startup(dbHost, dbName, dbLogin, dbPassword, NULL);
}

void CreateProxyFnLogTableIfNotExists(const char* table_name)
{
	stringstream ss;
	ss << "CREATE TABLE IF NOT EXISTS`" << table_name << "` ( ";
	ss << "`id` int(11) NOT NULL AUTO_INCREMENT, ";
	ss << "`timestamp` varchar(45) NOT NULL, ";
	ss << "`test_instance_id` int(11) NOT NULL, ";
	ss << "`message` text NOT NULL, ";
	ss << "PRIMARY KEY (`id`) ";
	ss << ") ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 ";

	if (sTraderDBMgr.IsStarted())
		sTraderDBMgr.ExecQuery(ss.str().c_str(), false);
	else
		assert(false && "DBMgr is not started!");
}