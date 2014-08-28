#include "parser.h"
#include "http_connection.h"

namespace crow {

	static const istring kHeaderFieldExpect = istring::literal("expect");
	static const istring kHeaderValue100Continue = istring::literal("100-continue");

	int HTTPParser::on_message_begin(http_parser* self_)
	{
		HTTPParser* self = static_cast<HTTPParser*>(self_);
		self->clear();
		return 0;
	}
	
	int HTTPParser::on_url(http_parser* self_, const char* at, size_t length)
	{
		HTTPParser* self = static_cast<HTTPParser*>(self_);
		self->url.insert(self->url.end(), at, at+length);
		return 0;
	}
	
	int HTTPParser::on_header_field(http_parser* self_, const char* at, size_t length)
	{
		HTTPParser* self = static_cast<HTTPParser*>(self_);
		self->headerField_.insert(self->headerField_.end(), at, at+length);
		return 0;
	}
		
	int HTTPParser::on_header_value(http_parser* self_, const char* at, size_t length)
	{
		HTTPParser* self = static_cast<HTTPParser*>(self_);
		self->headerValue_.insert(self->headerValue_.end(), at, at+length);
				
		return 0;
	}
	
	int HTTPParser::on_headers_complete(http_parser* self_)
	{
		HTTPParser* self = static_cast<HTTPParser*>(self_);
		// handle HTTP go ahead if we're there
		if (self->headerField_ == kHeaderFieldExpect && self->headerValue_ == kHeaderValue100Continue) {
			self->connection_->HandleExpect100Continue();
			return 1; // tell http_parser the response is done
		}
		else {
			cout << "Don't know what to do with: " << self->headerField_ << " -> " << self->headerValue_ << endl;
			self->headerField_.empty();
			self->headerValue_.empty();
			return 0;
		}
	}
	
	int HTTPParser::on_body(http_parser* self_, const char* at, size_t length)
	{
		HTTPParser* self = static_cast<HTTPParser*>(self_);
		self->body.insert(self->body.end(), at, at+length);
		return 0;
	}
	
	int HTTPParser::on_message_complete(http_parser* self_)
	{
		HTTPParser* self = static_cast<HTTPParser*>(self_);
		self->process_message();
		return 0;
	}
	
	HTTPParser::HTTPParser(Connection* handler):
		connection_(handler) {
		http_parser_init(this, HTTP_REQUEST);
	}
	
	// return false on error
	bool HTTPParser::feed(const char* buffer, int length)
	{
		const http_parser_settings settings_{
			on_message_begin,
			on_url,
			nullptr,
			on_header_field,
			on_header_value,
			nullptr,
			on_body,
			on_message_complete,
		};
		
		int nparsed = http_parser_execute(this, &settings_, buffer, length);
		return nparsed == length;
	}
	
	bool HTTPParser::done()
	{
		return feed(nullptr, 0);
	}
	
	void HTTPParser::clear()
	{
		url.clear();
		body.clear();
	}
		
	void HTTPParser::process_message()
	{
		connection_->handle();
	}
	
	request HTTPParser::to_request() const
	{
		return request{(HTTPMethod)method, std::move(url), std::move(body)};
	}
	
	bool HTTPParser::check_version(int major, int minor) const
	{
		return http_major == major && http_minor == minor;
	}
}