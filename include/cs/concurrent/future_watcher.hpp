#ifndef FUTURE_HPP
#define FUTURE_HPP

#include <cstddef>
#include <future>

#include <cs/logger/logger.hpp>
#include <cs/signals/signals.hpp>
#include <cs/concurrent/detail/worker.hpp>

namespace cs {
// future entity
template <typename T>
using Future = std::future<T>;

template <typename Result>
class FutureBase : public std::enable_shared_from_this<FutureBase<Result>> {
public:
    using Id = uint64_t;
    friend class Concurrent;

protected:
    FutureBase() {
        ++producedId;

        id_ = producedId;
        state_ = WatcherState::Idle;
        policy_ = RunPolicy::Thread;
    }

    FutureBase(FutureBase&) = delete;
    ~FutureBase() = default;

    explicit FutureBase(const RunPolicy policy, Future<Result>&& future)
    : FutureBase() {
        future_ = std::move(future);
        policy_ = policy;
    }

    FutureBase(FutureBase&& watcher) noexcept
    : future_(std::move(watcher.future_))
    , policy_(watcher.policy_)
    , state_(watcher.state_)
    , id_(watcher.id_) {
    }

    FutureBase& operator=(FutureBase&& watcher) noexcept {
        if (state_ == WatcherState::Running) {
            cslog() << "Trying to use operator= in watcher running state\n";
        }

        future_ = std::move(watcher.future_);
        policy_ = watcher.policy_;
        id_ = watcher.id_;

        return *this;
    }

public:
    // returns current watcher state, if watcher never watched runnable entity
    // then his state is Idle
    WatcherState state() const noexcept {
        return state_;
    }

    Id id() const noexcept {
        return id_;
    }

protected:
    using CompletedSignal = cs::Signal<void(Id)>;

    Future<Result> future_;
    RunPolicy policy_;
    WatcherState state_ = WatcherState::Idle;
    Id id_;

    inline static Id producedId = 0;
    constexpr static std::chrono::milliseconds awaiterTime{10};

    void setCompletedState() {
        future_ = Future<Result>();
        state_ = WatcherState::Compeleted;

        emit completed(id_);
    }

    // TODO: try to find more optimaze realization
    template <typename Awaiter>
    void await(const Awaiter& awaiter) {
        if (cs::Connector::callbacks(&awaiter) != 0) {
            return;
        }

        std::this_thread::sleep_for(awaiterTime);
    }

protected signals:

    // internal utility signal, clients should used finished/failed signals from FutureWatcher
    CompletedSignal completed;
};

// object to get future result from concurrent
// generates signal when finished
// policy used to control backend future to await result
template <typename Result>
class FutureWatcher : public FutureBase<Result> {
public:
    using FinishSignal = cs::Signal<void(const Result&)>;
    using FailedSignal = cs::Signal<void()>;

    explicit FutureWatcher(RunPolicy policy, Future<Result>&& future)
    : FutureBase<Result>(policy, std::move(future)) {
        watch();
    }

    FutureWatcher() = default;
    ~FutureWatcher() = default;
    FutureWatcher(FutureWatcher&&) = default;

    FutureWatcher& operator=(FutureWatcher&& watcher) {
        FutureBase<Result>::operator=(std::move(watcher));

        watch();
        return *this;
    }

protected:
    using Super = FutureBase<Result>;

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
        Worker::execute(Super::policy_, std::move(closure));
    }

public signals:
    FinishSignal finished;
    FailedSignal failed;
};

template <>
class FutureWatcher<void> : public FutureBase<void> {
public:
    using FinishSignal = cs::Signal<void()>;
    using FailedSignal = cs::Signal<void()>;

    explicit FutureWatcher(RunPolicy policy, Future<void>&& future)
    : FutureBase<void>(policy, std::move(future)) {
        watch();
    }

    FutureWatcher() = default;
    ~FutureWatcher() = default;
    FutureWatcher(FutureWatcher&& watcher) = default;

    FutureWatcher& operator=(FutureWatcher&& watcher) noexcept {
        FutureBase<void>::operator=(std::move(watcher));

        watch();
        return *this;
    }

protected:
    using Super = FutureBase<void>;

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
        Worker::execute(Super::policy_, std::move(closure));
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
