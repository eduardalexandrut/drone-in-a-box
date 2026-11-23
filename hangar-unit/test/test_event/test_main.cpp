//
// Created by aedua on 11/23/2026.
//
#include <unity.h>
#include "Event.h"

struct TestRequest {
    int type = 0;
    int reqId = 0;
};

void setUp() {}
void tearDown() {}

void test_new_event_is_not_pending() {
    Event<int> event;

    TEST_ASSERT_FALSE(event.isPending());
}

void test_consume_on_empty_returns_false() {
    Event<int> event;
    int out = 0;

    TEST_ASSERT_FALSE(event.consume(out));
}

void test_consume_on_empty_leaves_out_unchanged() {
    Event<int> event;
    int out = 42;

    event.consume(out);

    TEST_ASSERT_EQUAL_INT(42, out);
}

void test_post_makes_event_pending() {
    Event<int> event;

    event.post(5);

    TEST_ASSERT_TRUE(event.isPending());
}

void test_consume_returns_posted_value() {
    Event<int> event;
    int out = 0;
    event.post(5);

    const bool consumed = event.consume(out);

    TEST_ASSERT_TRUE(consumed);
    TEST_ASSERT_EQUAL_INT(5, out);
}

void test_consume_clears_pending() {
    Event<int> event;
    int out = 0;
    event.post(5);

    event.consume(out);

    TEST_ASSERT_FALSE(event.isPending());
}

void test_second_consume_returns_false() {
    Event<int> event;
    int out = 0;
    event.post(5);

    const bool first = event.consume(out);
    const bool second = event.consume(out);

    TEST_ASSERT_TRUE(first);
    TEST_ASSERT_FALSE(second);
}

void test_last_post_wins() {
    Event<int> event;
    int out = 0;
    event.post(1);
    event.post(2);

    const bool first = event.consume(out);
    const bool second = event.consume(out);

    TEST_ASSERT_TRUE(first);
    TEST_ASSERT_EQUAL_INT(2, out);
    TEST_ASSERT_FALSE(second);
}

void test_event_can_be_reused() {
    Event<int> event;
    int out = 0;
    event.post(1);
    event.consume(out);

    event.post(2);
    const bool consumed = event.consume(out);

    TEST_ASSERT_TRUE(consumed);
    TEST_ASSERT_EQUAL_INT(2, out);
}

void test_works_with_bool() {
    Event<bool> event;
    bool out = false;
    event.post(true);

    const bool consumed = event.consume(out);

    TEST_ASSERT_TRUE(consumed);
    TEST_ASSERT_TRUE(out);
}

void test_works_with_struct() {
    Event<TestRequest> event;
    TestRequest request;
    request.type = 1;
    request.reqId = 42;
    TestRequest out;
    event.post(request);

    const bool consumed = event.consume(out);

    TEST_ASSERT_TRUE(consumed);
    TEST_ASSERT_EQUAL_INT(1, out.type);
    TEST_ASSERT_EQUAL_INT(42, out.reqId);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_new_event_is_not_pending);
    RUN_TEST(test_consume_on_empty_returns_false);
    RUN_TEST(test_consume_on_empty_leaves_out_unchanged);
    RUN_TEST(test_post_makes_event_pending);
    RUN_TEST(test_consume_returns_posted_value);
    RUN_TEST(test_consume_clears_pending);
    RUN_TEST(test_second_consume_returns_false);
    RUN_TEST(test_last_post_wins);
    RUN_TEST(test_event_can_be_reused);
    RUN_TEST(test_works_with_bool);
    RUN_TEST(test_works_with_struct);
    return UNITY_END();
}