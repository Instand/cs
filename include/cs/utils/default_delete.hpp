#ifndef CS_DEFAULT_DELETE
#define CS_DEFAULT_DELETE

#include <type_traits>

namespace cs {
template <typename T>
class DefaultDelete {
public:
    template <typename Type>
    void operator()(Type* value) {
        if constexpr (std::is_array_v<T>) {
            delete[] value;
        }
        else {
            delete value;
        }
    }
};
}

#endif // CS_DEFAULT_DELETE
