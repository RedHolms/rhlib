#pragma once
#define _RHLIB_INCLUDED_LIST

#include <rh.hpp>

#include <rh/InitList.hpp>
#include <rh/exceptions.hpp>

_RHLIB_BEGIN

template <typename T>
class List {
public:
  using value_type = T;

private:
  T*     m_items     = nullptr;
  size_t m_count     = 0;
  size_t m_allocated = 0;

public:
  constexpr List() noexcept = default;

  constexpr List(size_t preallocate) : List() {
    _reallocate(preallocate);
  }

  constexpr List(size_t count, T const& value = {}) : List() {
    _needAllocated(count);
    
    m_count = count;
    for (size_t i = 0; i < count; ++i)
      constructAt(&m_items[i], value);
  }

  constexpr List(InitList<T> init) : List() {
    _initFromRange(init.begin(), init.end());
  }

  constexpr List(List const& other) : List() {
    operator=(other);
  }

  constexpr List(List&& other) noexcept {
    _stealOther(other);
  }

  constexpr List& operator=(List const& other) {
    _initFromRange(other.begin(), other.end());
    return *this;
  }

  constexpr List& operator=(List&& other) noexcept {
    List::~List();
    _stealOther(other);
    return *this;
  }

  constexpr ~List() {
    clear();
    _reallocate(0);
  }

public:
  [[nodiscard]]
  constexpr T* data() noexcept {
    return m_items;
  }

  [[nodiscard]]
  constexpr const T* data() const noexcept {
    return m_items;
  }

  [[nodiscard]]
  constexpr T* begin() noexcept {
    return m_items;
  }

  [[nodiscard]]
  constexpr const T* begin() const noexcept {
    return m_items;
  }

  [[nodiscard]]
  constexpr T* end() noexcept {
    return m_items + m_count;
  }

  [[nodiscard]]
  constexpr const T* end() const noexcept {
    return m_items + m_count;
  }

  [[nodiscard]]
  constexpr bool isEmpty() const noexcept {
    return m_count == 0;
  }

  [[nodiscard]]
  constexpr size_t length() const noexcept {
    return m_count;
  }

  constexpr void reserve(size_t count) {
    _needAllocated(count);
  }

  [[nodiscard]]
  constexpr size_t capacity() const noexcept {
    return m_allocated;
  }

  constexpr void shrinkToFit() {
    if (m_allocated > m_count)
      _reallocate(m_count);
  }

  constexpr void clear() noexcept {
    for (size_t i = 0; i < m_count; ++i)
      destructAt(&m_items[i]);

    m_count = 0;
  }

  constexpr void resize(size_t newCount) {
    _needAllocated(newCount);

    if (m_count < newCount) {
      // new elements
      for (size_t i = m_count; i < newCount; ++i)
        constructAt(&m_items[i]);
    }
    else if (m_count > newCount) {
      // remove some
      for (size_t i = newCount; i < m_count; ++i)
        destructAt(&m_items[i]);
    }

    m_count = newCount;
  }

  constexpr void insert(ssize_t index, T&& movedValue) {
    if (index > m_count || index < -m_count)
      throw IndexError(U"invalid index for List::insert()");

    if (index == m_count)
      return append(index, forward<T>(movedValue));

    if (index < 0)
      index += m_count;

    _needAllocated(m_count + 1);
    _shiftForInsert(index);

    m_items[index] = forward<T>(movedValue);
    ++m_count;
  }

  constexpr void insert(ssize_t index, T const& value) {
    if (index > m_count || index < -m_count)
      throw IndexError(U"invalid index for List::insert()");

    if (index == m_count)
      return append(index, value);

    if (index < 0)
      index += m_count;

    _needAllocated(m_count + 1);
    _shiftForInsert(index);

    m_items[index] = value;
    ++m_count;
  }

  constexpr void append(T&& movedValue) {
    _needAllocated(m_count + 1);
    constructAt(&m_items[m_count], forward<T>(movedValue));
    ++m_count;
  }

  constexpr void append(T const& value) {
    _needAllocated(m_count + 1);
    constructAt(&m_items[m_count], value);
    ++m_count;
  }

  template <typename... ArgsT>
  constexpr void emplace(ssize_t index, ArgsT&&... args) {
    if (index > m_count || index < -m_count)
      throw IndexError(U"invalid index for List::emplace()");

    if (index == m_count)
      return emplaceBack(index, forward<ArgsT>(args)...);

    if (index < 0)
      index += m_count;

    _needAllocated(m_count + 1);
    _shiftForInsert(index);

    destructAt(&m_items[index]);
    constructAt(&m_items[index], forward<ArgsT>(args)...);

    ++m_count;
  }

  template <typename... ArgsT>
  constexpr void emplaceBack(ArgsT&&... args) {
    _needAllocated(m_count + 1);
    constructAt(&m_items[m_count], forward<ArgsT>(args)...);
    ++m_count;
  }

  constexpr void erase(ssize_t index, size_t count = 1) {
    if (index >= m_count || index < -m_count)
      throw IndexError(U"invalid index for List::erase()");

    if (index < 0)
      index += m_count;

    for (size_t i = 0; i < count; ++i)
      destructAt(&m_items[index + i]);

    for (size_t i = index + count; i < m_count; ++i) {
      if (i - count < index + count)
        constructAt(&m_items[i - count], move(&m_items[i]));
      else {
        m_items[i - count] = move(&m_items[i]);
        if (i < m_count - count)
          destructAt(&m_items[i]);
      }
    }

    m_count -= count;
  }

public:
  [[nodiscard]]
  constexpr T const& operator[](size_t index) const noexcept {
    return m_items[index];
  }

  [[nodiscard]]
  constexpr T& operator[](size_t index) noexcept {
    return m_items[index];
  }

private:
  constexpr void _initFromRange(T const* begin, T const* end) {
    size_t count = static_cast<size_t>(end - begin);
    
    _needAllocated(count);
    m_count = count;

    for (auto it = begin, dest = m_items; it != end; ++it, ++dest)
      *dest = *it; // copy only here, can't move because it's const
  }

  constexpr void _stealOther(List& other) noexcept {
    m_items = other.m_items;
    m_count = other.m_count;
    m_allocated = other.m_allocated;
    other.m_items = nullptr;
    other.m_count = 0;
    other.m_allocated = 0;
  }

  constexpr void _needAllocated(size_t size) {
    if (m_allocated < size)
      _grow(size);
  }

  constexpr void _grow(size_t min_size) {
    size_t want_allocate = m_allocated + m_allocated / 2;

    if (want_allocate < min_size)
      want_allocate = min_size;

    _reallocate(want_allocate);
  }

  constexpr void _reallocate(size_t new_size) {
    if (new_size < m_count)
      new_size = m_count;

    T* prev_buffer = m_items;
    size_t prev_allocated = m_allocated;

    m_allocated = new_size;
    m_items = nullptr;

    if (new_size > 0) {
      m_items = new T[new_size];

      if (prev_buffer) {
        for (size_t i = 0; i < m_count; ++i)
          constructAt(&m_items[i], move(prev_buffer[i]));
      }
    }

    if (prev_buffer) {
      for (size_t i = 0; i < m_count; ++i)
        destructAt(&prev_buffer[i]);

      delete[] prev_buffer;
    }
  }

  constexpr void _shiftForInsert(size_t index) {
    for (ssize_t i = m_count - 1; i >= index; --i) {
      if (i == m_count - 1)
        constructAt(&m_items[i + 1], move(m_items[i]));
      else
        m_items[i + 1] = move(m_items[i]);
    }
  }
};

_RHLIB_END
