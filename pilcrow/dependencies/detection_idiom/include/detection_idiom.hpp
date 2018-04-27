#pragma once
#include <type_traits>

// impl ----------

#if (__cplusplus > 201402L)

using std::void_t;

#else

template <typename...> using void_t = void;

#endif

template <class, template <class> class, class = void_t<>>
struct detect : std::false_type {};

template <class T, template <class> class Op>
struct detect<T, Op, void_t<Op<T>>> : std::true_type {};

template <class T, class Void, template <class...> class Op, class... Args>
struct detector {
    using value_t = std::false_type;
    using type = T;
};

template <class T, template <class...> class Op, class... Args>
struct detector<T, void_t<Op<Args...>>, Op, Args...> {
    using value_t = std::true_type;
    using type = Op<Args...>;
};

struct nonesuch final {
    nonesuch() = delete;
    ~nonesuch() = delete;
    nonesuch(const nonesuch &) = delete;
    void operator =(const nonesuch &) = delete;
};

template <class T, template<class...> class Op, class... Args>
using detected_or = detector<T, void, Op, Args...>;

template <class T, template<class...> class Op, class... Args>
using detected_or_t = typename detected_or<T, Op, Args...>::type;

template <template<class...> class Op, class... Args>
using detected = detected_or<nonesuch, Op, Args...>;

template <template<class...> class Op, class... Args>
using detected_t = typename detected<Op, Args...>::type;

template <template<class...> class Op, class... Args>
using is_detected = typename detected<Op, Args...>::value_t;

template <class T, template<class...> class Op, class... Args>
using is_detected_exact = std::is_same<T, detected_t<Op, Args...>>;

template <class To, template<class...> class Op, class... Args>
using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;

// usage ----------
/*
template <typename T>
using copy_assign_op = decltype(std::declval<T &>() = std::declval<const T &>());

template <typename T>
using is_copy_assignable = is_detected<copy_assign_op, T>;

template <typename T>
constexpr bool CopyAssignable = is_copy_assignable<T>::value;

struct foo {};
struct bar { bar &operator =(const bar &) = delete; };

int main()
{
    static_assert(!CopyAssignable<foo>, "foo is copy assignable");
    static_assert( CopyAssignable<bar>, "bar is not copy assignable");
    return 0;
}
*/