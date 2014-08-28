#include "http_response.h"

namespace crow {
	response::response(response&& r)
	{
		*this = std::move(r);
	}
	
	response& response::operator=(response&& r)
	{
		body_ = std::move(r.body_);
		code = r.code;
		headers = std::move(r.headers);
		completed_ = r.completed_;
		return *this;
	}
	
	void response::clear()
	{
		body_ = istring::literal("");
		code = 200;
		headers.clear();
		completed_ = false;
	}
		
	void response::end()
	{
		cout << "response::end()" << endl;
		if (!completed_)
		{
			if (complete_request_handler_)
			{
				complete_request_handler_();
			}
			completed_ = true;
		}
	}
	
	void response::end(const istring& body)
	{
		body_ = body;
		end();
	}
}