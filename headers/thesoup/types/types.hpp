#pragma once

#include <variant>
#include <string>
#include <stdexcept>

/**
 * \namespace thesoup
 *
 * \brief The root namespace.
 * */
namespace thesoup {
    /**
     * \namespace thesoup::types
     *
     * \brief Sub namespace with all numeric classes and functions.
     * */
    namespace types {
        /**
         * \class Result
         * \tparam T The success type.
         * \tparam E The error type.
         *
         * \brief A class akin to Rust's `Result<T, E>`. Helps us avoid having to throw exceptions.
         * The `Result<T, E>` type is used to return the results of operations in the library.
         *
         * Usage:
         * ------
         * Result res {Result<int, int>::success(1)};
         * Result res2{Result<int,int>::failure(2)};
         * std::cout << res.unwrap() << "\n";
         * std::cout << res2.error() << "\n";
         * if (res) {
         *     std::cout << "Yes\n";
         * } else {
         *     std::cout << "No\n";
         * }
         *
         * \var type Represents the result type (Either success or err).
         *
         * \var val Optional successful return value.
         *
         * \var error Optional error return type.
         *
         * \var message Optional error message
         * */
        template <typename T, typename E> class Result {
        private:
            struct _Error {
                E err;
                _Error(const E& err): err {err} {}
                _Error(E&& err): err {std::move(err)} {}
            };
            const std::variant<T, _Error> store;
            const bool is_error;

            Result(const T& val) noexcept: store {val}, is_error {false} {}
            Result(T&& val) noexcept: store {std::move(val)}, is_error {false} {}
            Result(const E& err, bool is_error) noexcept: store {_Error(err)}, is_error {is_error} {}

        public:

            operator bool() const {
                return !is_error;
            }

            /**
             * \brief Return the success object
             *
             * This function returns the success object wrapped inside of the result object. Calling this function on a
             * Result object of Failure type will throw a `std::runtime_error`
             *
             * \return: Length of the vector (std::size_t)
             * */
            const T& unwrap() {
                if (is_error) {
                    throw std::runtime_error("Result attempted to be unwrapped on error.");
                } else {
                    return std::get<T>(store);
                }
            }

            /**
             * \brief Return the error object
             *
             * This function returns the error object wrapped inside of the result object. Calling this function on a
             * Result object of OK type will throw a `std::runtime_error`
             *
             * \return: Length of the vector (std::size_t)
             * */
            const E& error() {
                if (!is_error) {
                    throw std::runtime_error("Error access attempted in success.");
                } else {
                    return std::get<_Error>(store).err;
                }
            }

            /**
            * \brief Return an `Result` object of success type
            *
            * This function returns a result object of success type. Access the value by calling `unwrap` on it.
            *
            * \return The Result object
            */
            static Result<T,E> success(T&& val) {
                return Result(std::forward<T>(val));
            }

            /**
             * \brief Return an `Result` object of failure type.
             *
             * This function returns a result object of failure type. Access the error by calling the `err` method on it;
             *
             * \tparam T The type of value (not error).
             * \tparam E The type of error.
             *
             * \return The Result object
         */
            static Result<T,E> failure(E&& err) {
                return Result(std::forward<E>(err), true);
            }
        };

        /**
         * \class Slice
         *
         * \tparam T The pointer type.
         *
         * \brief A class akin to python/rust Slice type (a fat pointer).
         *
         * The `Slice<T>` type is meant to provide functionality similar to Rust's `fat pointer` types. It has 2 fields,
         * a start pointer of type `T`, and a size. It provides some convenience overloads:
         *   - Overloaded [] operator for indexing with range checking.
         *   - Overloaded `begin` and `end` methods to enable range for loop.
         *
         * NOTE: Move and move assignment has been disabled to avoid multiple ownership mess.
         *
         * \var start The start address of type `T`.
         *
         * \var size The numer of elements in the slice.
         * */
        template <typename T> struct Slice {
            T* start;
            std::size_t size;

            // Default constructor
            Slice(): start {nullptr}, size {0} {}

            // Constructor
            Slice(T* start, const std::size_t size): start {start}, size {size} {}

            // The copy constructor and assignment operator need to be removed. Ideally, we do not want slices to be shared.
            Slice(const Slice<T>&)=delete;
            void operator=(const Slice<T>&)=delete;

            T& operator[](const std::size_t& idx) {
                if (idx >= size) {
                    throw std::out_of_range("Array index out of range");
                }
                return start[idx];
            }

            const T& operator[](const std::size_t& idx) const {
                if (idx >= size) {
                    throw std::out_of_range("Array index out of range");
                }
                return start[idx];
            }

            T* begin() {
                return start;
            }

            const T* begin() const {
                return start;
            }

            const T* end() {
                return &start[size];
            }

            const T* end() const {
                return &start[size];
            }
        };

        /**
         * \enum Unit
         *
         * \brief Represents the `void` type.
         *
         * This enum is meant to emulate Rust's `Unit` type, so that we can return void types with `Result`. Example
         * `Result<Unit, ErrorCode>`
         * */
        enum class Unit{unit};

        /**
        * \class IsForwardIteratorOfType
        *
        * \brief Forward iterator static asserter.
        *
        * \tparam It The iterator type. Typically you would do domething like decltype(v1.begin()).
        *
        * \tparam T The value type that you want the iterator to have.
        *
        * Are you creating a template for which one of the parameters has to be a forward iterator of type `T`?
        * This struct helps you with that. `IsForwardIterator<decltype(v1.begin()), int>` will evaluate to true
        * if `v1` is a vector of integers.
        *
        * ```
        * std::vector<int> v1 {1,2,3};
        * std::cout << std::boolalpha << Sigabrt::Numeric::Models::IsForwardIterator<decltype(v1.begin()), int>::value << "\n";
        * ```
        * */
        template <typename It,typename T, typename=void> struct IsForwardIteratorOfType {
            static constexpr bool value {false};
        };

        //!\cond NO_DOC
        template <typename It, typename T>
        struct IsForwardIteratorOfType<
                It,
                T,
                typename std::enable_if<
                        (
                                (
                                        std::is_same<typename std::iterator_traits<It>::iterator_category, std::forward_iterator_tag>::value ||
                                        std::is_same<typename std::iterator_traits<It>::iterator_category, std::random_access_iterator_tag>::value ||
                                        std::is_same<typename std::iterator_traits<It>::iterator_category, std::bidirectional_iterator_tag>::value
                                ) &&
                                std::is_same<typename std::iterator_traits<It>::value_type, T>::value
                        )
                >::type
        >  {
            static constexpr bool value {true};
        };
        //!\endcond
    }
}
