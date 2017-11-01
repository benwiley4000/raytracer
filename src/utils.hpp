/*
 *  Common utility functions
 */

#ifndef UTILS_HPP
#define UTILS_HPP

namespace utils {
    // https://stackoverflow.com/a/30099727
    // use ignoreResult(some_function_call()) to explicitly ignore
    // return values we don't need, and suppress compiler warnings
    template<typename T>
    inline void ignoreResult(const T &) {}
}

#endif // UTILS_HPP
