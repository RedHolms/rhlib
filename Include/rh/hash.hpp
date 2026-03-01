#pragma once

namespace rh::hash {

namespace fnv {

constexpr size_t prime = sizeof(void*) == 4 ? 0x01000193 : 0x00000100000001b3;
constexpr size_t offsetBasis = sizeof(void*) == 4 ? 0x811c9dc5 : 0xcbf29ce484222325;

constexpr size_t v1(size_t hash, uint8_t byte) {
  hash *= prime;
  hash ^= byte;
  return hash;
}

constexpr size_t v1a(size_t hash, uint8_t byte) {
  hash ^= byte;
  hash *= prime;
  return hash;
}

constexpr size_t v1_appendBytes(void const* data, size_t bytesCount, size_t hash = offsetBasis) {
  auto dataB = static_cast<uint8_t const*>(data);
  for (size_t i = 0; i < bytesCount; ++i)
    hash = v1(hash, dataB[i]);
  return hash;
}

constexpr size_t v1a_appendBytes(void const* data, size_t bytesCount, size_t hash = offsetBasis) {
  auto dataB = static_cast<uint8_t const*>(data);
  for (size_t i = 0; i < bytesCount; ++i)
    hash = v1a(hash, dataB[i]);
  return hash;
}

} // namespace fnv

} // namespace rh::hash
