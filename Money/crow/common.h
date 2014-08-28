#pragma once

namespace crow
{
    enum class HTTPMethod
    {
        DELETE,
        GET,
        POST,
        PUT,
    };

    inline std::string method_name(HTTPMethod method)
    {
        switch(method)
        {
            case HTTPMethod::DELETE:	return "DELETE";
            case HTTPMethod::GET:		return "GET";
            case HTTPMethod::POST:		return "POST";
            case HTTPMethod::PUT:		return "PUT";
        }
        return "invalid";
    }
	
	namespace black_magic {
		constexpr bool is_equ_p(const char* a, const char* b, unsigned n)
        {
            return
			*a == 0 || *b == 0
			? false :
			n == 0
			? true :
			*a != *b
			? false :
			is_equ_p(a+1, b+1, n-1);
        }
	}
}

constexpr crow::HTTPMethod operator "" _method(const char* str, size_t len)
{
    return
        crow::black_magic::is_equ_p(str, "GET", 3)		? crow::HTTPMethod::GET		:
        crow::black_magic::is_equ_p(str, "DELETE", 6)	? crow::HTTPMethod::DELETE	:
        crow::black_magic::is_equ_p(str, "POST", 4)		? crow::HTTPMethod::POST	:
        crow::black_magic::is_equ_p(str, "PUT", 3)		? crow::HTTPMethod::PUT		:
        throw std::runtime_error("invalid http method");
};

