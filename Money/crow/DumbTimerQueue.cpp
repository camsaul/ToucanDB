#include "DumbTimerQueue.h"

namespace crow {
	namespace detail {
		DumbTimerQueue::Key DumbTimerQueue::Key::New() {
			Key k {};
			k.cancelled_ = make_shared<bool>(false);
			return k;
		}
		
		void DumbTimerQueue::Key::Cancel() {
			if (!cancelled_ || *cancelled_) return; // no-op
			*cancelled_ = true;
			cancelled_ = nullptr;
		}
	
		DumbTimerQueue& DumbTimerQueue::Current()
		{
			thread_local DumbTimerQueue q;
			return q;
		}
		
		DumbTimerQueue::Key DumbTimerQueue::Add(QueueFn f)
		{
			auto key = Key::New();
			queue_.emplace_back(key.cancelled_, move(f));
			return key;
		}
		
		void DumbTimerQueue::Process() {
			while (!IsEmpty()) {
				auto entry = queue_.front();
				if (!(*entry.first)) {
					entry.second();
				}
				queue_.pop_front();
			}
		}
		
		bool DumbTimerQueue::IsEmpty() {
			return !io_service_ || queue_.empty();
		}
		
		void DumbTimerQueue::set_io_service(boost::asio::io_service& io_service)
		{
			io_service_ = &io_service;
		}
	}
}