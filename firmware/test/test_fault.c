#include "unity.h"
#include "fault.h"

void setUp(void)
{
    fault_reset();
}

void tearDown(void) {}

void test_no_fault_for_valid_sample(void)
{
    TEST_ASSERT_FALSE(fault_detect(2500, 931, true));
}

void test_fault_for_temperature_below_minimum(void)
{
    TEST_ASSERT_TRUE(fault_detect(-4001, 124, false));
}

void test_fault_for_temperature_above_maximum(void)
{
    TEST_ASSERT_TRUE(fault_detect(12507, 2173, false));
}

void test_not_frozen_before_five_identical_samples(void)
{
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_FALSE(fault_detect(2500, 931, true));
    }
}

void test_frozen_after_five_identical_samples(void)
{
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_FALSE(fault_detect(2500, 931, true));
    }
    TEST_ASSERT_TRUE(fault_detect(2500, 931, true));
}

void test_different_adc_resets_frozen_count(void)
{
    for (int i = 0; i < 4; i++) {
        (void)fault_detect(2500, 931, true);
    }
    TEST_ASSERT_FALSE(fault_detect(2500, 932, true));
}

void test_invalid_sample_resets_frozen_count(void)
{
    for (int i = 0; i < 4; i++) {
        (void)fault_detect(2500, 931, true);
    }
    TEST_ASSERT_TRUE(fault_detect(13000, 931, true));
    TEST_ASSERT_FALSE(fault_detect(2500, 931, true));
}

void test_adc_out_of_range_is_fault(void)
{
    TEST_ASSERT_TRUE(fault_detect(2500, 4096, true));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_no_fault_for_valid_sample);
    RUN_TEST(test_fault_for_temperature_below_minimum);
    RUN_TEST(test_fault_for_temperature_above_maximum);
    RUN_TEST(test_not_frozen_before_five_identical_samples);
    RUN_TEST(test_frozen_after_five_identical_samples);
    RUN_TEST(test_different_adc_resets_frozen_count);
    RUN_TEST(test_invalid_sample_resets_frozen_count);
    RUN_TEST(test_adc_out_of_range_is_fault);
    return UNITY_END();
}
