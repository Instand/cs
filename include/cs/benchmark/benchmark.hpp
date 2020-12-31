#ifndef CS_BENCHMARK_HPP
#define CS_BENCHMARK_HPP

#include <cs/benchmark/details/executor.hpp>

namespace cs {
class Benchmark {
public:
    template <typename Func, typename Result = std::invoke_result_t<Func>>
    Result execute(Func func, std::chrono::seconds max = std::chrono::seconds(30), [[maybe_unused]] const std::string& failedMessage = std::string{}) {
        try {
            if constexpr (std::is_same_v<Result, bool>) {
                cs::details::Executor<bool> executor;
                auto result = executor.run(func, max);

                if (!result) {
                    cslog() << "\n" << failedMessage;
                    cslog() << "Failed running other tests, fix previous test to correct execution";

                    std::exit(0);
                }

                return result;
            }
            else if constexpr (std::is_same_v<Result, void>) {
                cs::details::Executor<void> executor;
                executor.run(func, max);
            }
            else {
                return cs::details::Executor<Result>{}.run(func, max);
            }
        }
        catch (const std::exception& e) {
            cslog() << "Exception catched, message: " << e.what();
            cslog() << "Benchmark would be terminated";

            std::terminate();
        }
    }
};
}

#endif // CS_BENCHMARK_HPP
