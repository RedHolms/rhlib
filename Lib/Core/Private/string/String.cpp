#include <rh/String.hpp>

#include <Windows.h>

bool String::contains(String const& substring) const {
  if (substring.empty())
    return true;

  auto length = substring.m_length;

  if (length > m_length)
    return false;

  for (uint i = 0; i <= m_length - length; ++i) {
    bool same = true;
    for (uint j = 0; j < length; ++j) {
      if (m_buffer[i + j] != substring[j]) {
        same = false;
        break;
      }
    }

    if (same)
      return true;
  }

  return false;
}

int String::operator<=>(String const& other) const noexcept {
  if (m_buffer == other.m_buffer)
    return 0;

  if (empty())
    return other.empty() ? 0 : -1;
  if (other.empty())
    return 1;

  size_t minLen = m_length < other.m_length ? m_length : other.m_length;

  int result = 0;
  for (uint i = 0; i < minLen; ++i) {
    auto a = m_buffer[i], b = other.m_buffer[i];
    if (a != b) {
      result = a < b ? -1 : +1;
      break;
    }
  }

  if (result != 0)
    return result;

  if (m_length < other.m_length)
    return -1;
  if (m_length > other.m_length)
    return 1;

  return 0;
}

std::string String::toANSI(int codePage) const {
  if (empty())
    return {};

  if (codePage == CP_ACP)
    codePage = GetACP();

  auto wide = toSTL<wchar_t>();

  size_t requiredSize =
    WideCharToMultiByte(codePage, 0, wide.c_str(), wide.size(), nullptr, 0, nullptr, nullptr);

  std::string result;
  result.resize(requiredSize);

  size_t written = WideCharToMultiByte(
    codePage, 0, wide.c_str(), wide.size(), result.data(), requiredSize, nullptr, nullptr
  );

  result.resize(written);
  return result;
}

void String::_initFromANSI(const char* ansiString, size_t lengthInBytes, uint32_t codePage) {
  if (codePage == CP_ACP)
    codePage = GetACP();

  size_t requiredSize = MultiByteToWideChar(codePage, 0, ansiString, lengthInBytes, nullptr, 0);

  auto buffer = new wchar_t[requiredSize];
  size_t written =
    MultiByteToWideChar(codePage, 0, ansiString, lengthInBytes, buffer, requiredSize);

  _initFromEncoded(buffer, written);
  delete[] buffer;
}

void String::_wantWrite(size_t charsCount) {
  auto required = m_length + charsCount + 1;

  if (m_allocated < required) {
    auto newSize = m_allocated * 2;
    if (newSize < required)
      newSize = required;

    _reallocate(newSize);
  }
}

void String::_reallocate(size_t newSize) {
  if (newSize < m_length + 1)
    newSize = m_length + 1;

  auto prevData = m_buffer;

  m_buffer = new char32_t[newSize];
  memset(m_buffer, 0, newSize);

  m_allocated = newSize;

  if (prevData) {
    memcpy(m_buffer, prevData, m_length * sizeof(char32_t));
    delete[] prevData;
  }
}
