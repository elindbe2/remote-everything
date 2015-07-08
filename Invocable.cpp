#include "Invocable.h"
#include "Environment.h"

InvocationHandlingFcn_t Invocable::gHandleInvocationResultPublishFcn;

Invocable::Invocable( void const * const thisp, string const& table )
{
//#ifdef ENABLE_INVOCATION
	//STraderInfo info;
	//if (Environment::GetTraderInfo(info))
	//{
	//	StartUpDB(info.dbHost, info.dbName, info.dbLogin, info.dbPassword, NULL);

		// Create the  table if it doesn't exist
	//	CreateProxyFnLogTableIfNotExists(table.c_str());
	//}
	//else
//		LOG_ERROR("Can't start invocation, no trader logged on...");
//#endif
}

Invocable::Invocable()
{
}

void Invocable::AddInvocation( string const& name, shared_ptr<FunctionInvoker> fni )
{
	assert(fni != NULL);

	try
	{
		fmap[name] = fni;
	}
	catch (...)
	{
		LOG_ERROR("There was a problem adding the invocation to the map");
		assert(false);
	}
}

vector<string> Invocable::GetInvocableList()
{
	vector<string> ret;

	for( FunctionInvocationMap::iterator it = fmap.begin(); it != fmap.end(); ++it )
	{
		ret.push_back(it->first);
	}

	return ret;
}

std::string Invocable::Invoke(std::string const& invocationID, std::string name, std::vector<std::string> & args )
{
	invocationID; // this is only included in the call to support asynchous calls, eg the invocable override in singlequeuestrategytemplate

	shared_ptr<FunctionInvoker> fip = NULL;
	try
	{
		fip = fmap.at( name );
	}
	catch( out_of_range e )
	{
		LOG_ERROR("Couldn't find function name in the invocation map: %s", name.c_str());
		assert(false);  // Couldn't find name in the map
		throw e;
	}

	// Log the function call
	/*
	stringstream ss;
	ss.str("");
	ss.clear();
	for( auto arg : args )
		ss << arg << " ";
	LOG_INFO("Strategy Proxy Call: %s(%s)", name.c_str(), ss.str().c_str());
	*/

	std::string result = "";
	try
	{
		result = fip->Invoke(args);
	}
	catch (std::exception& e)
	{
		LOG_ERROR("Exception in fip->Invoke(args) (fn: %s) in Invocable.cpp: %s", name.c_str(), e.what());
		throw e;
	}
	

	return result;
}

void Invocable::AsyncInvoke(std::string const&, std::string, std::vector<std::string> &)
{
    assert(false);
    LOG_ERROR("Invocable::AsyncInvoke called but not implemented!");
}