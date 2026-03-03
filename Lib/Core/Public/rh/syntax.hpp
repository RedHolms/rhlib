#pragma once

/**
 * implicit - keyword explicitly marking constructors and
 *   conversion operators implicit
 */
#define implicit explicit(false)

/**
 * forceinline - keyword to force the compiler to inline marked method
 */
#define forceinline __forceinline

/**
 * IMMOVABLE_CLASS(ClassName) - forbid copying/moving specified class
 */
#define IMMOVABLE_CLASS(ClassName)                                                                 \
  ClassName(const ClassName&) = delete;                                                            \
  ClassName& operator=(const ClassName&) = delete;                                                 \
  ClassName(ClassName&&) = delete;                                                                 \
  ClassName& operator=(ClassName&&) = delete;

/**
 * STATIC_CLASS(ClassName) - forbid copying/moving/constructing specified class
 */
#define STATIC_CLASS(ClassName)                                                                    \
  IMMOVABLE_CLASS(ClassName)                                                                       \
  ClassName() = delete;                                                                            \
  ~ClassName() = delete
