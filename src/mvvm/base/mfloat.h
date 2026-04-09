/**
 * File:   mfloat.h
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

#ifndef TK_MFLOAT_H
#define TK_MFLOAT_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class mfloat_t
 *
 * 解决float类型计算精度缺失问题的浮点数类。
 *
 * 内部使用整数存储浮点数，通过缩放因子(MFLOAT_PRECISION)进行精度控制，
 * 从而避免直接使用float/double进行运算时产生的精度误差。
 *
 * 示例：
 *
 * ```c
 * mfloat_t a, b, result;
 * mfloat_init(&a, 0.1);
 * mfloat_init(&b, 0.2);
 * mfloat_add(&result, &a, &b);
 * // mfloat_get(&result) == 0.3
 * ```
 *
 */

/**
 * @def MFLOAT_PRECISION
 * 精度因子，即小数点后保留的精度（10^6 = 1000000，保留6位小数）。
 */
#define MFLOAT_PRECISION 1000000LL

typedef struct _mfloat_t {
  /**
   * @property {int64_t} value
   * 内部存储的整数值，等于实际浮点数乘以 MFLOAT_PRECISION。
   */
  int64_t value;
} mfloat_t;

/**
 * @method mfloat_create
 * 创建mfloat对象。
 *
 * @annotation ["constructor"]
 * @param {double} v 初始值。
 *
 * @return {mfloat_t*} 成功返回mfloat_t对象，失败返回NULL。
 */
mfloat_t* mfloat_create(double v);

/**
 * @method mfloat_destroy
 * 销毁mfloat对象。
 *
 * @annotation ["destructor"]
 * @param {mfloat_t*} mf mfloat对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mfloat_destroy(mfloat_t* mf);

/**
 * @method mfloat_init
 * 初始化栈上的mfloat对象。
 *
 * @param {mfloat_t*} mf mfloat对象。
 * @param {double} v 初始值。
 *
 * @return {mfloat_t*} 返回mfloat_t对象。
 */
mfloat_t* mfloat_init(mfloat_t* mf, double v);

/**
 * @method mfloat_set
 * 设置mfloat对象的值。
 *
 * @param {mfloat_t*} mf mfloat对象。
 * @param {double} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mfloat_set(mfloat_t* mf, double v);

/**
 * @method mfloat_get
 * 获取mfloat对象的值。
 *
 * @param {const mfloat_t*} mf mfloat对象。
 *
 * @return {double} 返回浮点数值。
 */
double mfloat_get(const mfloat_t* mf);

/**
 * @method mfloat_add
 * 将a与b相加，结果存入result。
 *
 * @param {mfloat_t*} result 结果对象，可以与a或b相同。
 * @param {const mfloat_t*} a 被加数。
 * @param {const mfloat_t*} b 加数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mfloat_add(mfloat_t* result, const mfloat_t* a, const mfloat_t* b);

/**
 * @method mfloat_sub
 * 将a与b相减，结果存入result。
 *
 * @param {mfloat_t*} result 结果对象，可以与a或b相同。
 * @param {const mfloat_t*} a 被减数。
 * @param {const mfloat_t*} b 减数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mfloat_sub(mfloat_t* result, const mfloat_t* a, const mfloat_t* b);

/**
 * @method mfloat_mul
 * 将a与b相乘，结果存入result。
 *
 * @param {mfloat_t*} result 结果对象，可以与a或b相同。
 * @param {const mfloat_t*} a 被乘数。
 * @param {const mfloat_t*} b 乘数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mfloat_mul(mfloat_t* result, const mfloat_t* a, const mfloat_t* b);

/**
 * @method mfloat_div
 * 将a与b相除，结果存入result。
 *
 * @param {mfloat_t*} result 结果对象，可以与a或b相同。
 * @param {const mfloat_t*} a 被除数。
 * @param {const mfloat_t*} b 除数（不能为0）。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mfloat_div(mfloat_t* result, const mfloat_t* a, const mfloat_t* b);

/**
 * @method mfloat_compare
 * 比较a与b的大小。
 *
 * @param {const mfloat_t*} a 第一个操作数。
 * @param {const mfloat_t*} b 第二个操作数。
 *
 * @return {int32_t} 若a < b返回负数，a == b返回0，a > b返回正数。
 */
int32_t mfloat_compare(const mfloat_t* a, const mfloat_t* b);

/**
 * @method mfloat_abs
 * 求mfloat的绝对值，结果存入result。
 *
 * @param {mfloat_t*} result 结果对象，可以与mf相同。
 * @param {const mfloat_t*} mf 操作数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mfloat_abs(mfloat_t* result, const mfloat_t* mf);

/**
 * @method mfloat_round
 * 对mfloat进行四舍五入，精确到指定小数位数。
 *
 * @param {mfloat_t*} result 结果对象，可以与mf相同。
 * @param {const mfloat_t*} mf 操作数。
 * @param {uint32_t} decimal_places 保留的小数位数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mfloat_round(mfloat_t* result, const mfloat_t* mf, uint32_t decimal_places);

END_C_DECLS

#endif /*TK_MFLOAT_H*/
