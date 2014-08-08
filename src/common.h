// Copyright 2014 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SPLAT_COMMON_H
#define SPLAT_COMMON_H

#include "mathfu/quaternion.h"
#include "mathfu/glsl_mappings.h"

namespace fpl {

#if defined(__IOS__) || defined(__ANDROID__)
  #define PLATFORM_MOBILE
#endif

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
//
// For disallowing only assign or copy, write the code directly, but declare
// the intend in a comment, for example:
// void operator=(const TypeName&);  // DISALLOW_ASSIGN
// Note, that most uses of DISALLOW_ASSIGN and DISALLOW_COPY are broken
// semantically, one should either use disallow both or neither. Try to
// avoid these in new code.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// ARRAYSIZE performs essentially the same calculation as arraysize,
// but can be used on anonymous types or types defined inside
// functions.  It's less safe than arraysize as it accepts some
// (although not all) pointers.  Therefore, you should use arraysize
// whenever possible.
//
// The expression ARRAYSIZE(a) is a compile-time constant of type
// size_t.
//
// ARRAYSIZE catches a few type errors.  If you see a compiler error
//
//   "warning: division by zero in ..."
//
// when using ARRAYSIZE, you are (wrongfully) giving it a pointer.
// You should only use ARRAYSIZE on statically allocated arrays.
//
// The following comments are on the implementation details, and can
// be ignored by the users.
//
// ARRAYSIZE(arr) works by inspecting sizeof(arr) (the # of bytes in
// the array) and sizeof(*(arr)) (the # of bytes in one array
// element).  If the former is divisible by the latter, perhaps arr is
// indeed an array, in which case the division result is the # of
// elements in the array.  Otherwise, arr cannot possibly be an array,
// and we generate a compiler error to prevent the code from
// compiling.
//
// Since the size of bool is implementation-defined, we need to cast
// !(sizeof(a) & sizeof(*(a))) to size_t in order to ensure the final
// result has type size_t.
//
// This macro is not perfect as it wrongfully accepts certain
// pointers, namely where the pointer size is divisible by the pointee
// size.  Since all our code has to go through a 32-bit compiler,
// where a pointer is 4 bytes, this means all pointers to a type whose
// size is 3 or greater than 4 will be (righteously) rejected.
//
// Kudos to Jorg Brown for this simple and elegant implementation.
//
// - wan 2005-11-16
//
// Starting with Visual C++ 2005, WinNT.h includes ARRAYSIZE.
#if !defined(_WIN32)
  #define ARRAYSIZE(a) \
    ((sizeof(a) / sizeof(*(a))) / \
     static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
#endif // !defined(_WIN32)

typedef mathfu::Quaternion<float> Quat;

// 1 WorldTime = 1/60s.
// That is, for a 60Hz game, WorldTime increments by one every frame.
// For a 30Hz game, WorldTime increments by two every frame.
typedef int WorldTime;

static const float kTwoPi = static_cast<float>(2.0 * M_PI);

// Returns point on unit circle corresponding to a sweep of 'angle' degrees
// from the x-axis.
//   0     ==> ( 1,  0)
//   pi/2  ==> ( 0,  1)
//   pi    ==> (-1,  0)
//   3pi/2 ==> ( 0, -1)
static inline mathfu::vec3 AngleToXZVector(const float angle) {
  return mathfu::vec3(cos(angle), 0.0f, sin(angle));
}

static inline mathfu::mat3 AngleToXZRotationMatrix(const float angle) {
  const float sin_angle = sin(angle);
  const float cos_angle = cos(angle);
  return mathfu::mat3(cos_angle,  0.0f,  -sin_angle,
                      0.0f,       1.0f,  0.0f,
                      sin_angle,  0.0f,  cos_angle);
}

}  // namespace fpl

#endif  // SPLAT_COMMON_H