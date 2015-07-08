#pragma once

#include <string>
#include <vector>
#include <functional>
#include "easy_bind.h"
#include "StringUtilities.hpp"


class EmptyClass{};

template <typename SIG>
class Invoker;

template < typename RET >
class Invoker< RET () >
{
    typedef std::function< RET() > fn_t;
public:
    Invoker()
    {
    }

    Invoker( fn_t const& fn_in )
		: fn(fn_in)
	{
	} 

	template < typename U = RET >
    typename enable_if<!is_void<U>::value,std::string>::type
    Invoke( std::vector<std::string> &, unsigned long )
	{
        return toString<RET>(fn());
	}	

	template < typename U = RET >
    typename enable_if<is_void<U>::value,std::string>::type
    Invoke( std::vector<std::string> &, unsigned long )
	{
        fn();
		return "";
	}

private:
		fn_t fn;
};
	
template < typename RET, typename T, typename... ARGS >
class Invoker< RET ( T, ARGS... ) >
{
    typedef std::function< RET( T, ARGS... ) > fn_t;

public:
    Invoker()
    {
    }

    Invoker( fn_t const& fn_in )
        : fn_( fn_in )
	{
	}

    // active for functions with non-void return value
	template < typename U = RET >
    typename enable_if<!is_void<U>::value,std::string>::type
    Invoke( std::vector<std::string> & args, unsigned long idx = 0 )
	{
        // convert one argument from a string
        auto t = fromString<T>(args.at(idx));
        // now bind it to bound, use a ref so we can
        // get the value later (for in/out arguments)
        auto bound = easy_bind(fn_, std::ref(t));

        // now create our next invoker
        nextInvoker_ = Invoker< RET(ARGS...) >(bound);

        // recursion
        auto ret_str = nextInvoker_.Invoke(args, idx+1);

        // now recover our parameter and replace the
        // string in args with its new value
        args[idx] = toString<T>(t);

        return ret_str;
	}	

    // active for functions which return void
	template < typename U = RET >
    typename enable_if<is_void<U>::value,std::string>::type
    Invoke( std::vector<std::string> & args, unsigned long idx = 0 )
	{
        // convert one argument from a string
        auto t = fromString<T>(args.at(idx));
        // now bind it to bound, use a ref so we can
        // get the value later (for in/out arguments)
        auto bound = easy_bind(fn_, std::ref(t));

        // now create our next invoker
        nextInvoker_ = Invoker< RET(ARGS...) >(bound);

        // recursion
        nextInvoker_.Invoke(args, idx+1);

        // now recover our parameter and replace the
        // string in args with its new value
        args[idx] = toString<T>(t);

        return "";
	}

private:
    Invoker< RET( ARGS... ) > nextInvoker_;
    fn_t fn_;
};

// This template allows you to invoke a method.  It takes as
// its template parameter the type of a method pointer
//    ie. decltype(&MyClass::myMethod)
template < typename K, typename RET, typename... ARGS >
class Invoker< RET(K::*)( ARGS... ) >
{
    typedef RET(K::*method_ptr_t)( ARGS... );
    typedef std::function< RET( ARGS... ) > fn_t;

public:
    Invoker( method_ptr_t const& method_ptr, K * thisp )
    {
        fn_ = easy_bind( std::function< RET( K*, ARGS... )>(method_ptr), thisp );
    }

    Invoker( method_ptr_t const& method_ptr, K & thisr )
    {
        fn_ = easy_bind( std::function< RET( K*, ARGS... )>(method_ptr), &thisr );
    }

    template < typename U = RET >
    typename enable_if<!is_void<U>::value,std::string>::type
    Invoke( std::vector<std::string> & args, unsigned long idx = 0 )
    {
        // now that the class pointer has been bound, we can simply
        // use the standard version of Invoker
        nextInvoker_ = Invoker< RET(ARGS...) >(fn_);
        return nextInvoker_.Invoke(args,idx);
    }

    template < typename U = RET >
    typename enable_if<is_void<U>::value,std::string>::type
    Invoke( std::vector<std::string> & args, unsigned long idx = 0 )
    {
        nextInvoker_ = Invoker< RET(ARGS...) >(fn_);
        nextInvoker_.Invoke(args,idx);
        return "";
    }

private:
    Invoker< RET( ARGS... ) > nextInvoker_;
    fn_t fn_;
};
