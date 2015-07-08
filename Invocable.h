#pragma once

#include "KeyValuePayload.h"
#include "VariadicInvoker.h"
#include "SaveInvocation.h"
#include "Lockable.h"

using InvocationHandlingFcn_t = std::function<void(const KeyValuePayload_t& payload)>;

// derive from invocable to invoke methods/free functions in a class
	class TW_TRADING_API Invocable
	{
	protected: 
	// used by the INVOCABLE_FUNCTION and INVOCABLE_METHOD macros
	void AddInvocation(std::string const& name, std::shared_ptr<FunctionInvoker> fni);

	public:
	Invocable( void const * const thisp, std::string const& table = "" );
		Invocable();
		virtual ~Invocable(){}

		std::vector<std::string> GetInvocableList();// gives you a list of invocations available


		static void SetInvocationResultPublishFcn(InvocationHandlingFcn_t publishFcn) // how to publish async results
		{
			gHandleInvocationResultPublishFcn = publishFcn;
		}

		static void HandleInvocationResult(const KeyValuePayload_t& payload) // make invocation aware of an async result.
		{
			if (gHandleInvocationResultPublishFcn) gHandleInvocationResultPublishFcn(payload);
		}
	
       virtual bool IsAsynchronous() { return false; };

	// do the actual invoke
		virtual std::string Invoke(std::string const& invocationID, std::string name, std::vector<std::string>& args );
        virtual void        AsyncInvoke(std::string const& invocationID, std::string name, std::vector<std::string>& args);
        
		virtual void setParameter(const std::string& key , const std::string& value) {}
		virtual void getParameter(const std::string& key) {}

		void Log(unsigned long testInstanceID, std::string message);

	private:
		FunctionInvocationMap fmap;
		static InvocationHandlingFcn_t gHandleInvocationResultPublishFcn;
	};
