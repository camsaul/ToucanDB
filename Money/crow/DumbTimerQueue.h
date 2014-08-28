#pragma once

namespace crow
{
    namespace detail 
    {
        /// fast timer queue for fixed tick value.
        class DumbTimerQueue {
        public:
			class Key {
				friend class DumbTimerQueue;
			public:
				void Cancel();
				
				Key() = default;
			private:
				shared_ptr<bool> cancelled_ = nullptr;
				static Key New();
			};
			
			using QueueFn    = function<void()>;
			using QueueEntry = pair<shared_ptr<bool>, QueueFn>;
			using Queue		 = deque<QueueEntry>;
			
            /// tls based queue to avoid locking
            static DumbTimerQueue& Current();

            Key Add(QueueFn f);

            void Process();
            void set_io_service(boost::asio::io_service& io_service);
			
			~DumbTimerQueue() {
				cout << "~DumbTimerQueue()" << endl;
			}

        private:
			void Cancel(Key& k);
            
			DumbTimerQueue() = default;
			DumbTimerQueue(const DumbTimerQueue&) = delete;
			DumbTimerQueue& operator=(const DumbTimerQueue&) = delete;
			DumbTimerQueue(DumbTimerQueue&&) = delete;
			DumbTimerQueue& operator=(DumbTimerQueue&&) = delete;
			
//			static void ProcessOne(struct pt* pt, QueueEntry& entry);
			bool IsEmpty();

            boost::asio::io_service* io_service_ = nullptr;
            Queue queue_;
        };
    }
}
