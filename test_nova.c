#include "unity.h"
#include "nova_drop.h"

void setUp(void) {
    // Set up before each test
}

void tearDown(void) {
    // Clean up after each test
}

void test_nova_init_consistency(void) {
    NovaState state1, state2;
    uint32_t seed = 12345;
    
    nova_init(&state1, seed);
    nova_init(&state2, seed);
    
    for (int i = 0; i < NOVA_DROP_STATE_SIZE; i++) {
        TEST_ASSERT_EQUAL_UINT32(state1.state[i], state2.state[i]);
    }
}

void test_nova_drop_reproducibility(void) {
    NovaState state1, state2;
    uint32_t seed = 98765;
    
    nova_init(&state1, seed);
    nova_init(&state2, seed);
    
    for (int i = 0; i < 100; i++) {
        TEST_ASSERT_EQUAL_UINT32(nova_drop(&state1), nova_drop(&state2));
    }
}

void test_nova_range_bounds(void) {
    NovaState state;
    nova_init(&state, 42);
    
    uint32_t min = 10, max = 20;
    for (int i = 0; i < 1000; i++) {
        uint32_t val = nova_range(&state, min, max);
        TEST_ASSERT_GREATER_OR_EQUAL_UINT32(min, val);
        TEST_ASSERT_LESS_OR_EQUAL_UINT32(max, val);
    }
}

void test_nova_range_single_value(void) {
    NovaState state;
    nova_init(&state, 42);
    
    // min == max should return min
    TEST_ASSERT_EQUAL_UINT32(5, nova_range(&state, 5, 5));
}

void test_nova_float_bounds(void) {
    NovaState state;
    nova_init(&state, 42);
    
    for (int i = 0; i < 1000; i++) {
        float val = nova_float(&state);
        TEST_ASSERT_FLOAT_WITHIN(0.5f, 0.5f, val); // val should be in [0, 1]
        TEST_ASSERT_TRUE(val >= 0.0f && val < 1.0f);
    }
}

void test_nova_bool_output(void) {
    NovaState state;
    nova_init(&state, 42);
    
    int count0 = 0, count1 = 0;
    for (int i = 0; i < 1000; i++) {
        int val = nova_bool(&state);
        TEST_ASSERT_TRUE(val == 0 || val == 1);
        if (val == 0) count0++;
        else count1++;
    }
    // Very loose check to ensure both outcomes are possible
    TEST_ASSERT_TRUE(count0 > 0 && count1 > 0);
}

void test_nova_auto_seed_uniqueness(void) {
    NovaState state1, state2;
    nova_auto_seed(&state1);
    // Tiny delay to ensure clock differences if urandom fails
    for (volatile int i = 0; i < 100000; i++); 
    nova_auto_seed(&state2);
    
    // It's technically possible but extremely unlikely they match
    int match = 1;
    for (int i = 0; i < NOVA_DROP_STATE_SIZE; i++) {
        if (state1.state[i] != state2.state[i]) {
            match = 0;
            break;
        }
    }
    TEST_ASSERT_FALSE_MESSAGE(match, "Auto-seeded states should be different");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_nova_init_consistency);
    RUN_TEST(test_nova_drop_reproducibility);
    RUN_TEST(test_nova_range_bounds);
    RUN_TEST(test_nova_range_single_value);
    RUN_TEST(test_nova_float_bounds);
    RUN_TEST(test_nova_bool_output);
    RUN_TEST(test_nova_auto_seed_uniqueness);
    return UNITY_END();
}
