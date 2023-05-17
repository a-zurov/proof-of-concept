#pragma once

#include "net_common.h"
#include <shared_mutex>

namespace olc
{
    namespace net
    {
        template<typename T>
        class queue_base
        {
        protected:
            mutable std::mutex mtxQueue_;
            std::deque<T> deqQueue_;

        public:
            queue_base() = default;
            queue_base(const queue_base<T>&) = delete;
            virtual ~queue_base() { clear(); }

            virtual void push_back(const T& item) {

                DBG_DUMP();
                std::lock_guard lock(mtxQueue_);
                deqQueue_.emplace_back(std::move(item));
            }

            virtual T pop_front() {

                DBG_DUMP();
                std::lock_guard lock(mtxQueue_);
                auto t = std::move(deqQueue_.front());
                deqQueue_.pop_front();
                return t;
            }

        public:
            const T& front() const {

                DBG_DUMP();
                std::lock_guard lock(mtxQueue_);
                return deqQueue_.front();
            }

            const T& back() const {

                DBG_DUMP();
                std::lock_guard lock(mtxQueue_);
                return deqQueue_.back();
            }

            bool empty() const {

                DBG_DUMP();
                std::lock_guard lock(mtxQueue_);
                return deqQueue_.empty();
            }

            size_t count() const {

                DBG_DUMP();
                std::lock_guard lock(mtxQueue_);
                return deqQueue_.size();
            }

            void clear() {

                DBG_DUMP();
                std::lock_guard lock(mtxQueue_);
                deqQueue_.clear();
            }
        };

        template<typename T>
        class tsqueue final : public queue_base<T>
        {
            using base = queue_base<T>;

            std::condition_variable cond_var_;

        public:
            tsqueue() = default;
            tsqueue(const tsqueue<T>&) = delete;

        public:
            void push_back(const T& item) override {

                DBG_DUMP();
                base::push_back(item);
                cond_var_.notify_one();
            }

            T pop_n_wait_front() {

                DBG_DUMP();

                std::unique_lock<std::mutex> lock(base::mtxQueue_);
                cond_var_.wait(lock, [this] { return !(this->base::deqQueue_.empty()); });

                auto t = std::move(base::deqQueue_.front());
                base::deqQueue_.pop_front();
                return t;
            }
        };
    }
}
