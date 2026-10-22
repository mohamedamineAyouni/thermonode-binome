/* ENF-05 : tampon circulaire, limites 0 / 1 / 59 / 60 / 61 */
#include "unity.h"
#include "ringbuf.h"

static frame_t make_frame(uint32_t seq)
{
    frame_t frame = {"TN01", seq, 0u, 2500, STATE_NORMAL};
    return frame;
}

void setUp(void) { ringbuf_reset(); }
void tearDown(void) {}

void test_ENF05_empty_pop_returns_false(void)
{
    frame_t frame;
    TEST_ASSERT_FALSE(ringbuf_pop(&frame));
    TEST_ASSERT_EQUAL_size_t(0, ringbuf_count());
}

void test_ENF05_one_push_one_pop(void)
{
    frame_t input = make_frame(7u);
    frame_t output;

    TEST_ASSERT_TRUE(ringbuf_push(&input));
    TEST_ASSERT_EQUAL_size_t(1, ringbuf_count());
    TEST_ASSERT_TRUE(ringbuf_pop(&output));
    TEST_ASSERT_EQUAL_UINT32(7u, output.seq);
    TEST_ASSERT_EQUAL_size_t(0, ringbuf_count());
}

void test_ENF05_59_and_60_fit(void)
{
    for (uint32_t i = 0u; i < 59u; i++) {
        frame_t frame = make_frame(i);
        TEST_ASSERT_TRUE(ringbuf_push(&frame));
    }
    TEST_ASSERT_EQUAL_size_t(59, ringbuf_count());

    frame_t frame = make_frame(59u);
    TEST_ASSERT_TRUE(ringbuf_push(&frame));
    TEST_ASSERT_EQUAL_size_t(60, ringbuf_count());
}

void test_ENF05_push_61_keeps_60_and_drops_oldest(void)
{
    for (uint32_t i = 0u; i < 61u; i++) {
        frame_t frame = make_frame(i);
        ringbuf_push(&frame);
    }

    TEST_ASSERT_EQUAL_size_t(60, ringbuf_count());

    frame_t output;
    TEST_ASSERT_TRUE(ringbuf_pop(&output));
    TEST_ASSERT_EQUAL_UINT32(1u, output.seq);
}

void test_ENF05_fifo_order_on_drain(void)
{
    for (uint32_t i = 0u; i < 10u; i++) {
        frame_t frame = make_frame(i);
        ringbuf_push(&frame);
    }

    for (uint32_t i = 0u; i < 10u; i++) {
        frame_t output;
        TEST_ASSERT_TRUE(ringbuf_pop(&output));
        TEST_ASSERT_EQUAL_UINT32(i, output.seq);
    }

    frame_t output;
    TEST_ASSERT_FALSE(ringbuf_pop(&output));
}

void test_ENF05_peek_does_not_remove(void)
{
    frame_t input = make_frame(3u);
    frame_t output;

    ringbuf_push(&input);
    TEST_ASSERT_TRUE(ringbuf_peek(&output));
    TEST_ASSERT_EQUAL_UINT32(3u, output.seq);
    TEST_ASSERT_EQUAL_size_t(1, ringbuf_count());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ENF05_empty_pop_returns_false);
    RUN_TEST(test_ENF05_one_push_one_pop);
    RUN_TEST(test_ENF05_59_and_60_fit);
    RUN_TEST(test_ENF05_push_61_keeps_60_and_drops_oldest);
    RUN_TEST(test_ENF05_fifo_order_on_drain);
    RUN_TEST(test_ENF05_peek_does_not_remove);
    return UNITY_END();
}
