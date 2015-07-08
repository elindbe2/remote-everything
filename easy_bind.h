#pragma once
// This code was written by StackOverflow user Xeo and I'd like to
// give him credit for this nice piece of code.  His twitter account
// is @tweet_xeo if you'd like to contact him.

#include <functional>
#include <type_traits>
#include <utility>

template <std::size_t... Is>
struct indices {};

template <std::size_t N, std::size_t... Is>
struct build_indices
  : build_indices<N-1, N-1, Is...> {};

template <std::size_t... Is>
struct build_indices<0, Is...> : indices<Is...> {};

template<int I> struct placeholder{};

namespace std{
template<int I>
struct is_placeholder< ::placeholder<I>> : std::integral_constant<int, I>{};
} // std::

namespace detail{
template<std::size_t... Is, class F, class... Args>
auto easy_bind(indices<Is...>, F const& f, Args&&... args)
  -> decltype(std::bind(f, std::forward<Args>(args)..., placeholder<1 + Is>{}...))
{
    return std::bind(f, std::forward<Args>(args)..., placeholder<1 + Is>{}...);
}
} // detail::

template<class R, class... FArgs, class... Args>
auto easy_bind(std::function<R(FArgs...)> const& f, Args&&... args)
    -> decltype(detail::easy_bind(build_indices<sizeof...(FArgs) - sizeof...(Args)>{}, f, std::forward<Args>(args)...))
{
    return detail::easy_bind(build_indices<sizeof...(FArgs) - sizeof...(Args)>{}, f, std::forward<Args>(args)...);
}
