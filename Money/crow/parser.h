#pragma once

#include "http_request.h"
#include "http_parser_merged.h"

namespace crow
{
	class Connection;
    struct HTTPParser : public http_parser
    {
        static int on_message_begin(http_parser* self_);
        static int on_url(http_parser* self_, const char* at, size_t length);
        static int on_header_field(http_parser* self_, const char* at, size_t length);
        static int on_header_value(http_parser* self_, const char* at, size_t length);
		static int on_headers_complete(http_parser* self_);
        static int on_body(http_parser* self_, const char* at, size_t length);
        static int on_message_complete(http_parser* self_);
		
        HTTPParser(Connection* handler);

        // return false on error
        bool feed(const char* buffer, int length);
        bool done();
        void clear();
        void process_message();
		
        request to_request() const;

		/// Check HTTP Version, e.g. 1,0 or 1,1
        bool check_version(int major, int minor) const;

        string url;
        string body;

		string headerField_;
		string headerValue_;

        Connection* connection_;
    };
}
