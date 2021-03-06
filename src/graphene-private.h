/* graphene-private.h: Private header
 *
 * Copyright © 2014  Emmanuele Bassi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __GRAPHENE_PRIVATE_H__
#define __GRAPHENE_PRIVATE_H__

#define _GNU_SOURCE

#include "config.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define GRAPHENE_FLOAT_EPSILON  FLT_EPSILON

#ifndef MIN
# if defined(__GNUC__) && __GNUC__ > 3
#  define MIN(a,b) \
  ({ \
    typeof ((a)) _a = (a); \
    typeof ((b)) _b = (b); \
    _a < _b ? _a : _b; \
  })
# else
#  define MIN(a,b) ((a) < (b) ? (a) : (b))
# endif
#endif

#ifndef MAX
# if defined(__GNUC__) && __GNUC__ > 3
#  define MAX(a,b) \
  ({ \
    typeof ((a)) _a = (a); \
    typeof ((b)) _b = (b); \
    _a > _b ? _a : _b; \
  })
# else
#  define MAX(a,b) ((a) > (b) ? (a) : (b))
# endif
#endif

#ifndef CLAMP
# if defined(__GNUC__) && __GNUC__ > 3
#  define CLAMP(v,min,max) \
  ({ \
    typeof ((v)) _v = (v); \
    typeof ((min)) _min = (min); \
    typeof ((max)) _max = (max); \
    _v < _min ? _min : (_v > _max ? _max : _v); \
  })
# else
#  define CLAMP(v,min,max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))
# endif
#endif

#if defined(__GNUC__) && __GNUC__ > 3
# define likely(x)      (__builtin_expect((x) ? 1 : 0, 1))
# define unlikely(x)    (__builtin_expect((x) ? 1 : 0, 0))
#else
# define likely(x)      (x)
# define unlikely(x)    (x)
#endif

#define GRAPHENE_DEG_TO_RAD(x)          ((x) * (GRAPHENE_PI / 180.f))
#define GRAPHENE_RAD_TO_DEG(x)          ((x) * (180.f / GRAPHENE_PI))

#if defined(__GNUC__)
/* Use typeof on GCC */
# define graphene_fuzzy_equals(n1,n2,epsilon) \
  __extension__({ \
    typeof ((n1)) __n1 = (n1); \
    typeof ((n2)) __n2 = (n2); \
    typeof ((epsilon)) __epsilon = (epsilon); \
    (bool) ((__n1 > __n2 ? (__n1 - __n2) : (__n2 - __n1)) < __epsilon); \
  })

#else
/* fallback for Visual Studio, typeof not supported */
# define graphene_fuzzy_equals(n1,n2,epsilon) \
  (((n1) > (n2) ? ((n1) - (n2)) : ((n2) - (n1))) < (epsilon))

#endif /* __GNUC__ */

static inline float
graphene_flerpf (float a, float b, double factor)
{
  return ((1.f - factor) * a) + (factor * b);
}

static inline double
graphene_flerp (double a, double b, double factor)
{
  return ((1.0 - factor) * a) + (factor * b);
}

static inline int
graphene_ilerp (int a, int b, double factor)
{
  return (int) (((1.0 - factor) * (double) a) + (factor * (double) b));
}

#if defined(__GNUC__) && \
  ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)) && \
  (__STDC_VERSION__ >= 201112L || !defined(__STRICT_ANSI__)) && \
  !defined(__cplusplus)
# define USE_C11_GENERIC 1
#endif

#ifdef USE_C11_GENERIC
# define graphene_lerp(a,b,f) \
  _Generic((a), \
    double: graphene_flerp, \
    float: graphene_flerpf, \
    int: graphene_ilerp, \
    default: graphene_flerp) ((a), (b), (f))

#else
# define graphene_lerp   graphene_flerp
#endif

static inline void
graphene_sincos (float angle, float *sin_out, float *cos_out)
{
#ifdef HAVE_SINCOSF
  sincosf (angle, sin_out, cos_out);
#else
  if (sin_out != NULL)
    *sin_out = sinf (angle);
  if (cos_out != NULL)
    *cos_out = cosf (angle);
#endif /* HAVE_SINCOSF */
}

#endif /* __GRAPHENE_PRIVATE_H__ */
