#pragma once

#include "json.h"

namespace crow
{
    class Connection;
    struct response
    {
        friend class crow::Connection;

        istring body_;
//        json::wvalue json_value;
        int code{200};
        std::unordered_map<istring, istring> headers;

        response() {}
        explicit response(int code) : code(code) {}
//        response(std::string body) : body(std::move(body)) {}
//        response(json::wvalue&& json_value) : json_value(std::move(json_value)) {}
//        response(const json::wvalue& json_value) : body(json::dump(json_value)) {}
//        response(int code, std::string body) : body(std::move(body)), code(code) {}

        response& operator=(const response& r) = delete;

        response(response&& r);
        response& operator=(response&& r);

        void clear();
//        void write(const std::string& body_part);

        void end();
		/// Set this->body_ and call this->end()
        void end(const istring& body);
		
		bool is_completed() const { return completed_; }
        bool is_alive() const { return is_alive_helper_ && is_alive_helper_(); }

        private:
            bool completed_{};
            std::function<void()> complete_request_handler_;
            std::function<bool()> is_alive_helper_;
    };
}
