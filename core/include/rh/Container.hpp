#pragma once
#define _RHLIB_INCLUDED_CONCEPTS

#include <rh.hpp>

#include <rh/TypeTraits.hpp>

_RHLIB_BEGIN

template <typename T, typename OfT>
concept Iterator = requires(T iterator) {
  ++iterator;
  --iterator;

  { *iterator } -> ConvertibleTo<OfT&>;
  {  iterator } -> ConvertibleTo<OfT*>;

  { iterator == declval<T>() } -> ConvertibleTo<bool>;
  { iterator != declval<T>() } -> ConvertibleTo<bool>;
};

static_assert(Iterator<int*, int>);
static_assert(Iterator<int const*, const int>);

template <typename T>
concept Iterable = requires(T object) {
  object.begin();
  object.end();
};

template <typename T, typename OfT>
concept IterableOf = requires(T object) {
  { object.begin() } -> Iterator<OfT>;
  { object.end() }   -> Iterator<OfT>;
};

template <typename T, typename OfT>
concept ConstContainer =
  requires(T container) {
    { container.data()  } -> ConvertibleTo<OfT const*>;
    { container.begin() } -> Iterator<OfT const>;
    { container.end()   } -> Iterator<OfT const>;

    { container.isEmpty() } -> ConvertibleTo<bool>;
    { container.length() } -> ConvertibleTo<size_t>;

    { container[declval<size_t>()] } -> ConvertibleTo<OfT const&>;
  };

template <typename T, typename OfT>
concept Container =
  ConstContainer<T, OfT> &&
  requires(T container) {
    { container.data()  } -> ConvertibleTo<OfT*>;
    { container.begin() } -> Iterator<OfT>;
    { container.end()   } -> Iterator<OfT>;

    { container.isEmpty() } -> ConvertibleTo<bool>;
    { container.length() } -> ConvertibleTo<size_t>;

    { container[declval<size_t>()] } -> ConvertibleTo<OfT&>;

    container.reserve(declval<size_t>());
    { container.capacity() } -> ConvertibleTo<size_t>;
    container.shrinkToFit();

    container.clear();

    // new elements are default-constructed
    container.resize(declval<size_t>());

    container.insert(declval<ssize_t>(), declval<OfT&&>());
    container.insert(declval<ssize_t>(), declval<OfT const&>());

    container.append(declval<OfT&&>());
    container.append(declval<OfT const&>());

    container.emplace(declval<ssize_t>());

    container.emplaceBack();

    container.erase(declval<ssize_t>());
    container.erase(declval<ssize_t>(), declval<size_t>());
  };

_RHLIB_END
