#include <type_traits>
#define FMT_HEADER_ONLY
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

template <typename T, typename = void>
struct is_formattable : std::false_type {};

template <typename T>
struct is_formattable<
    T, std::enable_if_t<fmt::has_formatter<T, fmt::format_context>::value>>
    : std::true_type {};

template <typename T>
static constexpr auto is_formattable_v = is_formattable<T>::value;