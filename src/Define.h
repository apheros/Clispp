#pragma once
#include <type_traits>


template<typename T>
using StorageType = typename std::decay<typename std::remove_reference<T>::type>::type;

template<Boolean, typename T>
struct is_true : public std::false_type
{
};

template<typename T>
struct is_true<true, T> : public std::true_type
{
};

template<typename T>
struct is_bool : std::false_type
{
};

template<>
struct is_bool<Boolean> : std::true_type
{
};

template<typename T>
struct is_string : std::false_type
{
};

template<>
struct is_string<String> : std::true_type
{
};

template<typename T>
struct is_list : std::false_type
{
};

template<typename T, typename A>
struct is_list<list<T, A>> : std::true_type
{
};

template<typename T>
struct is_number : is_true<std::is_arithmetic<StorageType<T>>::value && !is_same<T, Boolean>::value, T>
{
};
