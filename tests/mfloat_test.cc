#include "mvvm/base/mfloat.h"
#include "gtest/gtest.h"

TEST(Mfloat, init) {
  mfloat_t a;
  mfloat_init(&a, 0.0);
  ASSERT_DOUBLE_EQ(mfloat_get(&a), 0.0);

  mfloat_init(&a, 1.5);
  ASSERT_DOUBLE_EQ(mfloat_get(&a), 1.5);

  mfloat_init(&a, -3.14);
  ASSERT_DOUBLE_EQ(mfloat_get(&a), -3.14);
}

TEST(Mfloat, set_get) {
  mfloat_t a;
  mfloat_init(&a, 0.0);

  ASSERT_EQ(mfloat_set(&a, 1.23456), RET_OK);
  ASSERT_DOUBLE_EQ(mfloat_get(&a), 1.23456);

  ASSERT_EQ(mfloat_set(&a, -9.87654), RET_OK);
  ASSERT_DOUBLE_EQ(mfloat_get(&a), -9.87654);
}

TEST(Mfloat, create_destroy) {
  mfloat_t* mf = mfloat_create(3.14);
  ASSERT_TRUE(mf != NULL);
  ASSERT_DOUBLE_EQ(mfloat_get(mf), 3.14);
  ASSERT_EQ(mfloat_destroy(mf), RET_OK);
}

TEST(Mfloat, add) {
  mfloat_t a, b, result;

  /* 0.1 + 0.2 == 0.3 (classic float precision problem) */
  mfloat_init(&a, 0.1);
  mfloat_init(&b, 0.2);
  mfloat_add(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 0.3);

  /* negative addition */
  mfloat_init(&a, -1.5);
  mfloat_init(&b, 0.5);
  mfloat_add(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), -1.0);

  /* in-place: result == a */
  mfloat_init(&a, 2.5);
  mfloat_init(&b, 1.25);
  mfloat_add(&a, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&a), 3.75);
}

TEST(Mfloat, sub) {
  mfloat_t a, b, result;

  mfloat_init(&a, 1.0);
  mfloat_init(&b, 0.3);
  mfloat_sub(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 0.7);

  mfloat_init(&a, 0.0);
  mfloat_init(&b, 5.5);
  mfloat_sub(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), -5.5);

  /* in-place: result == b */
  mfloat_init(&a, 10.0);
  mfloat_init(&b, 4.0);
  mfloat_sub(&b, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&b), 6.0);
}

TEST(Mfloat, mul) {
  mfloat_t a, b, result;

  mfloat_init(&a, 3.0);
  mfloat_init(&b, 4.0);
  mfloat_mul(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 12.0);

  mfloat_init(&a, 0.1);
  mfloat_init(&b, 0.1);
  mfloat_mul(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 0.01);

  mfloat_init(&a, -2.5);
  mfloat_init(&b, 4.0);
  mfloat_mul(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), -10.0);

  /* zero */
  mfloat_init(&a, 0.0);
  mfloat_init(&b, 99.99);
  mfloat_mul(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 0.0);
}

TEST(Mfloat, div) {
  mfloat_t a, b, result;

  mfloat_init(&a, 10.0);
  mfloat_init(&b, 4.0);
  mfloat_div(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 2.5);

  mfloat_init(&a, 1.0);
  mfloat_init(&b, 3.0);
  mfloat_div(&result, &a, &b);
  /* 1/3 to 6 decimal places */
  ASSERT_NEAR(mfloat_get(&result), 1.0 / 3.0, 1e-6);

  mfloat_init(&a, -9.0);
  mfloat_init(&b, 3.0);
  mfloat_div(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), -3.0);
}

TEST(Mfloat, div_by_zero) {
  mfloat_t a, b, result;

  mfloat_init(&a, 5.0);
  mfloat_init(&b, 0.0);
  ASSERT_EQ(mfloat_div(&result, &a, &b), RET_BAD_PARAMS);
}

TEST(Mfloat, compare) {
  mfloat_t a, b;

  mfloat_init(&a, 1.0);
  mfloat_init(&b, 2.0);
  ASSERT_LT(mfloat_compare(&a, &b), 0);

  mfloat_init(&a, 2.0);
  mfloat_init(&b, 2.0);
  ASSERT_EQ(mfloat_compare(&a, &b), 0);

  mfloat_init(&a, 3.0);
  mfloat_init(&b, 2.0);
  ASSERT_GT(mfloat_compare(&a, &b), 0);

  mfloat_init(&a, -1.0);
  mfloat_init(&b, 0.0);
  ASSERT_LT(mfloat_compare(&a, &b), 0);
}

TEST(Mfloat, abs) {
  mfloat_t a, result;

  mfloat_init(&a, -3.5);
  mfloat_abs(&result, &a);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 3.5);

  mfloat_init(&a, 3.5);
  mfloat_abs(&result, &a);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 3.5);

  /* in-place */
  mfloat_init(&a, -7.0);
  mfloat_abs(&a, &a);
  ASSERT_DOUBLE_EQ(mfloat_get(&a), 7.0);
}

TEST(Mfloat, round) {
  mfloat_t a, result;

  /* round to 2 decimal places */
  mfloat_init(&a, 3.14159);
  mfloat_round(&result, &a, 2);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 3.14);

  mfloat_init(&a, 3.145);
  mfloat_round(&result, &a, 2);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 3.15);

  /* round to 0 decimal places */
  mfloat_init(&a, 2.5);
  mfloat_round(&result, &a, 0);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 3.0);

  mfloat_init(&a, 2.4);
  mfloat_round(&result, &a, 0);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 2.0);

  /* negative */
  mfloat_init(&a, -3.145);
  mfloat_round(&result, &a, 2);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), -3.15);
}

TEST(Mfloat, precision_classic) {
  /* Classic float precision demonstration */
  mfloat_t a, b, result;
  double raw_result = 0.1 + 0.2;

  /* Direct float arithmetic is NOT exactly 0.3 */
  ASSERT_NE(raw_result, 0.3);

  /* Mfloat arithmetic IS exactly 0.3 */
  mfloat_init(&a, 0.1);
  mfloat_init(&b, 0.2);
  mfloat_add(&result, &a, &b);
  ASSERT_DOUBLE_EQ(mfloat_get(&result), 0.3);
}
