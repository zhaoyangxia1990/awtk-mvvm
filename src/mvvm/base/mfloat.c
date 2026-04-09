/**
 * File:   mfloat.c
 * Author: AWTK Develop Team
 * Brief:  Mfloat - float with precision
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-03-01 AWTK Develop Team created
 *
 */

#include <math.h>
#include "tkc/mem.h"
#include "mvvm/base/mfloat.h"

mfloat_t* mfloat_create(double v) {
  mfloat_t* mf = TKMEM_ZALLOC(mfloat_t);
  return_value_if_fail(mf != NULL, NULL);

  mf->value = (int64_t)round(v * MFLOAT_PRECISION);

  return mf;
}

ret_t mfloat_destroy(mfloat_t* mf) {
  return_value_if_fail(mf != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(mf);

  return RET_OK;
}

mfloat_t* mfloat_init(mfloat_t* mf, double v) {
  return_value_if_fail(mf != NULL, NULL);

  mf->value = (int64_t)round(v * MFLOAT_PRECISION);

  return mf;
}

ret_t mfloat_set(mfloat_t* mf, double v) {
  return_value_if_fail(mf != NULL, RET_BAD_PARAMS);

  mf->value = (int64_t)round(v * MFLOAT_PRECISION);

  return RET_OK;
}

double mfloat_get(const mfloat_t* mf) {
  return_value_if_fail(mf != NULL, 0.0);

  return (double)mf->value / (double)MFLOAT_PRECISION;
}

ret_t mfloat_add(mfloat_t* result, const mfloat_t* a, const mfloat_t* b) {
  return_value_if_fail(result != NULL && a != NULL && b != NULL, RET_BAD_PARAMS);

  result->value = a->value + b->value;

  return RET_OK;
}

ret_t mfloat_sub(mfloat_t* result, const mfloat_t* a, const mfloat_t* b) {
  return_value_if_fail(result != NULL && a != NULL && b != NULL, RET_BAD_PARAMS);

  result->value = a->value - b->value;

  return RET_OK;
}

ret_t mfloat_mul(mfloat_t* result, const mfloat_t* a, const mfloat_t* b) {
  return_value_if_fail(result != NULL && a != NULL && b != NULL, RET_BAD_PARAMS);

  /*
   * Both a->value and b->value are scaled by MFLOAT_PRECISION, so their product
   * is scaled by MFLOAT_PRECISION^2. Divide by MFLOAT_PRECISION to restore scale.
   * Use __int128 to avoid overflow when both values are large.
   */
  result->value = (int64_t)(((__int128)a->value * b->value) / MFLOAT_PRECISION);

  return RET_OK;
}

ret_t mfloat_div(mfloat_t* result, const mfloat_t* a, const mfloat_t* b) {
  return_value_if_fail(result != NULL && a != NULL && b != NULL, RET_BAD_PARAMS);
  return_value_if_fail(b->value != 0, RET_BAD_PARAMS);

  /*
   * a->value / b->value would lose scale. Multiply a->value by MFLOAT_PRECISION
   * first to restore the correct scale in the result.
   * Integer division truncates toward zero, consistent with standard C semantics.
   */
  result->value = (int64_t)(((__int128)a->value * MFLOAT_PRECISION) / b->value);

  return RET_OK;
}

int32_t mfloat_compare(const mfloat_t* a, const mfloat_t* b) {
  return_value_if_fail(a != NULL && b != NULL, 0);

  if (a->value < b->value) {
    return -1;
  } else if (a->value > b->value) {
    return 1;
  } else {
    return 0;
  }
}

ret_t mfloat_abs(mfloat_t* result, const mfloat_t* mf) {
  return_value_if_fail(result != NULL && mf != NULL, RET_BAD_PARAMS);

  result->value = mf->value < 0 ? -mf->value : mf->value;

  return RET_OK;
}

ret_t mfloat_round(mfloat_t* result, const mfloat_t* mf, uint32_t decimal_places) {
  int64_t factor = 1;
  int64_t remainder = 0;
  uint32_t i = 0;
  int64_t half = 0;

  return_value_if_fail(result != NULL && mf != NULL, RET_BAD_PARAMS);
  return_value_if_fail(decimal_places <= 6, RET_BAD_PARAMS);

  /* factor = 10^(6 - decimal_places), the unit to round to */
  for (i = 0; i < (6 - decimal_places); i++) {
    factor *= 10;
  }

  half = factor / 2;
  remainder = mf->value % factor;

  if (mf->value >= 0) {
    result->value = mf->value - remainder + (remainder >= half ? factor : 0);
  } else {
    /* For negative values, round away from zero */
    int64_t abs_remainder = -remainder;
    result->value = mf->value - remainder - (abs_remainder >= half ? factor : 0);
  }

  return RET_OK;
}
