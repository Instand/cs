#ifndef FUTURE_BASE_HPP
#define FUTURE_BASE_HPP

#include <memory>

#include <cs/signals/signals.hpp>
#include <cs/concurrent/details/worker.hpp>

namespace cs {
namespace details {
template <typename Result>
class FutureBase : public std::enable_shared_from_this<FutureBase<Result>> {
public:
    using Id = uint64_t;

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

    FutureBase& operator=(FutureBase&& watcher) noexcept = delete;

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
} // namespace details
}

#endif // FUTURE_BASE_HPP
