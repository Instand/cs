#ifndef FUTURE_HPP
#define FUTURE_HPP

#include <cstddef>
#include <future>

#include <cs/logger/logger.hpp>
#include <cs/signals/signals.hpp>

#include <cs/concurrent/details/future_base.hpp>

namespace cs {
class Concurrent;

// object to get future result from concurrent
// generates signal when finished
// policy used to control backend future to await result
template <typename Result>
class FutureWatcher : public details::FutureBase<Result> {
    using FinishSignal = cs::Signal<void(const Result&)>;
    using FailedSignal = cs::Signal<void()>;

public:
    explicit FutureWatcher(RunPolicy policy, Future<Result>&& future):
        details::FutureBase<Result>(policy, std::move(future)) {
        watch();
    }

    FutureWatcher() = default;
    ~FutureWatcher() = default;
    FutureWatcher(FutureWatcher&&) = default;

    FutureWatcher& operator=(FutureWatcher&& watcher) = delete;

protected:
    using Super = details::FutureBase<Result>;
    friend class Concurrent;

    void watch() {
        auto closure = [=] {
            try {
                Result result = Super::future_.get();

                auto lambda = [this, res = std::move(result)]() {
                    Super::await(finished);
                    emit finished(std::move(res));

                    Super::setCompletedState();
                };

                lambda();
            }
            catch (std::exception& e) {
                Super::await(failed);

                cslog() << "Concurrent execution with " << typeid(Result).name() << " failed, " << e.what();
                emit failed();
            }
        };

        Super::state_ = WatcherState::Running;
        details::Worker::execute(Super::policy_, std::move(closure));
    }

public signals:
    FinishSignal finished;
    FailedSignal failed;
};

template <>
class FutureWatcher<void> : public details::FutureBase<void> {
    using FinishSignal = cs::Signal<void()>;
    using FailedSignal = cs::Signal<void()>;

public:
    explicit FutureWatcher(RunPolicy policy, Future<void>&& future):
        details::FutureBase<void>(policy, std::move(future)) {
        watch();
    }

    FutureWatcher() = default;
    ~FutureWatcher() = default;
    FutureWatcher(FutureWatcher&& watcher) = default;

    FutureWatcher& operator=(FutureWatcher&& watcher) noexcept = delete;

protected:
    using Super = FutureBase<void>;
    friend class Concurrent;

    void watch() {
        auto closure = [=] {
            try {
                Super::future_.get();

                auto signal = [=] {
                    Super::await(finished);
                    emit finished();

                    Super::setCompletedState();
                };

                signal();
            }
            catch (std::exception& e) {
                Super::await(failed);

                cslog() << "Concurrent execution with void result failed, " << e.what();
                emit failed();
            }
        };

        Super::state_ = WatcherState::Running;
        details::Worker::execute(Super::policy_, std::move(closure));
    }

public signals:
    FinishSignal finished;
    FailedSignal failed;
};

// safe pointer to watcher
template <typename T>
using FutureWatcherPtr = std::shared_ptr<FutureWatcher<T>>;
}

#endif // FUTURE_HPP
