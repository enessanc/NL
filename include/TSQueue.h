#pragma once
#include "Common.h"


namespace NL
{
    template<typename T>
    class TSQueue
    {
    public:
        TSQueue() = default;
        TSQueue(const TSQueue<T>& q) = delete;


        const T& front()
        {
            std::scoped_lock lock(muxQueue);
            return deqQueue.front();
        }

        const T& back()
        {
            std::scoped_lock lock(muxQueue);
            return deqQueue.back();
        }

        void push_back(const T& item)
        {
            std::scoped_lock lock(muxQueue);
            deqQueue.emplace_back(item);

            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.notify_one();
        }

        void push_front(const T& item)
        {
            std::scoped_lock lock(muxQueue);
            deqQueue.emplace_front(item);

            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.notify_one();
        }

        bool empty()
        {
            std::scoped_lock lock(muxQueue);
            return deqQueue.empty();
        }

			
        size_t count()
        {
            std::scoped_lock lock(muxQueue);
            return deqQueue.size();
        }

        void clear()
        {
            std::scoped_lock lock(muxQueue);
            deqQueue.clear();
        }

        T pop_back()
        {
            std::scoped_lock lock(muxQueue);
            auto t = std::move(deqQueue.back());
            deqQueue.pop_back();
            return t;
        }

        T pop_front()
        {
            std::scoped_lock lock(muxQueue);
            auto t = std::move(deqQueue.front());
            deqQueue.pop_front();
            return t;
        }

        void wait()
        {
            while (empty())
            {
                std::unique_lock<std::mutex> ul(muxBlocking);
                cvBlocking.wait(ul); //In windows, this works in problematic! But this is no problem here because we are keeping him in wait mode
            }
        }

			
		protected:
			std::mutex muxQueue;
			std::deque<T> deqQueue;

			std::condition_variable cvBlocking;
            std::mutex muxBlocking;
    };
}

