#pragma once

namespace crow
{
    class Connection;
    struct response
    {
        friend class crow::Connection;

        istring body_;
        int code{200};
        unordered_map<istring, istring> headers;

        response() {}
        explicit response(int code) : code(code) {}

        response& operator=(const response& r) = delete;

        response(response&& r);
        response& operator=(response&& r);

        void clear();
        void end();
        void end(const istring& body); ///< Set this->body_ and call this->end()
		
		bool is_completed() const { return completed_; }
        bool is_alive()		const { return is_alive_helper_ && is_alive_helper_(); }

        private:
            bool completed_{};
            function<void()> complete_request_handler_;
            function<bool()> is_alive_helper_;
    };
}
