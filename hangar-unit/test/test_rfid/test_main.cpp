//
// Created by Eduard on 24/11/2026.
//
#include <unity.h>
#include "BadgeTask.h"
#include "BadgeWhiteList.h"

// ---------------------------------------------------------------------------
// Test helpers
// ---------------------------------------------------------------------------

// A reader where the test decides what the next readings return
class FakeCardReader : public ICardReader {
public:
    void present(const CardId& card) {
        this->card = card;
        hasCard = true;
    }

    void remove() { hasCard = false; }

    bool readCard(CardId& out) override {
        if (!hasCard) return false;
        out = card;
        return true;
    }

private:
    CardId card;
    bool hasCard = false;
};

static CardId makeCard(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    CardId card;
    card.uid[0] = a;
    card.uid[1] = b;
    card.uid[2] = c;
    card.uid[3] = d;
    card.length = 4;
    return card;
}

static const CardId AUTHORIZED = makeCard(0xA3, 0x5F, 0x12, 0x9C);
static const CardId UNKNOWN    = makeCard(0x11, 0x22, 0x33, 0x44);
static const CardId ALLOWED_CARDS[] = { AUTHORIZED };

// Runs one tick and returns true if a reset event was posted (and consumes it)
static bool tickAndConsume(BadgeTask& task, HangarBlackBoard& bb) {
    task.tick();
    bool value = false;
    return bb.resetFromBadge().consume(value);
}

// Runs a sequence of readings (true = card present, false = no card)
// and returns how many reset events were posted
static int countEvents(BadgeTask& task, HangarBlackBoard& bb, FakeCardReader& reader,
                       const CardId& card, const bool* readings, int n) {
    int events = 0;
    for (int i = 0; i < n; i++) {
        if (readings[i]) reader.present(card);
        else             reader.remove();
        if (tickAndConsume(task, bb)) events++;
    }
    return events;
}

void setUp() {}
void tearDown() {}

// ---------------------------------------------------------------------------
// BadgeWhiteList
// ---------------------------------------------------------------------------

void test_authorized_card_is_accepted() {
    BadgeWhiteList whitelist(ALLOWED_CARDS, 1);

    TEST_ASSERT_TRUE(whitelist.isAuthorized(AUTHORIZED));
}

void test_unknown_card_is_rejected() {
    BadgeWhiteList whitelist(ALLOWED_CARDS, 1);

    TEST_ASSERT_FALSE(whitelist.isAuthorized(UNKNOWN));
}

void test_card_with_different_length_is_rejected() {
    BadgeWhiteList whitelist(ALLOWED_CARDS, 1);
    CardId longer = AUTHORIZED;   // same first 4 bytes...
    longer.uid[4] = 0x01;
    longer.uid[5] = 0x02;
    longer.uid[6] = 0x03;
    longer.length = 7;            // ...but 7 bytes long

    TEST_ASSERT_FALSE(whitelist.isAuthorized(longer));
}

void test_empty_whitelist_rejects_all() {
    BadgeWhiteList whitelist(ALLOWED_CARDS, 0);

    TEST_ASSERT_FALSE(whitelist.isAuthorized(AUTHORIZED));
}

// ---------------------------------------------------------------------------
// BadgeTask
// ---------------------------------------------------------------------------

void test_no_event_without_card() {
    HangarBlackBoard bb;
    FakeCardReader reader;
    BadgeWhiteList whitelist(ALLOWED_CARDS, 1);
    BadgeTask task(bb, reader, whitelist);

    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_FALSE(tickAndConsume(task, bb));
    }
}

void test_authorized_card_posts_one_event() {
    HangarBlackBoard bb;
    FakeCardReader reader;
    BadgeWhiteList whitelist(ALLOWED_CARDS, 1);
    BadgeTask task(bb, reader, whitelist);

    reader.present(AUTHORIZED);

    TEST_ASSERT_TRUE(tickAndConsume(task, bb));
}

void test_card_held_posts_only_once() {
    HangarBlackBoard bb;
    FakeCardReader reader;
    BadgeWhiteList whitelist(ALLOWED_CARDS, 1);
    BadgeTask task(bb, reader, whitelist);
    reader.present(AUTHORIZED);

    TEST_ASSERT_TRUE(tickAndConsume(task, bb));
    for (int i = 0; i < 9; i++) {
        TEST_ASSERT_FALSE(tickAndConsume(task, bb));
    }
}

void test_unauthorized_card_posts_no_event() {
    HangarBlackBoard bb;
    FakeCardReader reader;
    BadgeWhiteList whitelist(ALLOWED_CARDS, 1);
    BadgeTask task(bb, reader, whitelist);

    reader.present(UNKNOWN);

    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_FALSE(tickAndConsume(task, bb));
    }
}

void test_intermittent_reads_while_held_do_not_repost() {
    HangarBlackBoard bb;
    FakeCardReader reader;
    BadgeWhiteList whitelist(ALLOWED_CARDS, 1);
    BadgeTask task(bb, reader, whitelist);

    // the real reader alternates while a card is held on it
    const bool readings[] = { true, false, true, false, true, false, true };
    const int events = countEvents(task, bb, reader, AUTHORIZED, readings, 7);

    TEST_ASSERT_EQUAL_INT(1, events);
}

void test_card_removed_after_consecutive_empty_reads() {
    HangarBlackBoard bb;
    FakeCardReader reader;
    BadgeWhiteList whitelist(ALLOWED_CARDS, 1);
    BadgeTask task(bb, reader, whitelist);

    // card, removed for 3 reads, then presented again
    const bool readings[] = { true, false, false, false, true };
    const int events = countEvents(task, bb, reader, AUTHORIZED, readings, 5);

    TEST_ASSERT_EQUAL_INT(2, events);
}

void test_two_empty_reads_are_not_a_removal() {
    HangarBlackBoard bb;
    FakeCardReader reader;
    BadgeWhiteList whitelist(ALLOWED_CARDS, 1);
    BadgeTask task(bb, reader, whitelist);

    // only 2 empty reads: still the same presentation
    const bool readings[] = { true, false, false, true };
    const int events = countEvents(task, bb, reader, AUTHORIZED, readings, 4);

    TEST_ASSERT_EQUAL_INT(1, events);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_authorized_card_is_accepted);
    RUN_TEST(test_unknown_card_is_rejected);
    RUN_TEST(test_card_with_different_length_is_rejected);
    RUN_TEST(test_empty_whitelist_rejects_all);

    RUN_TEST(test_no_event_without_card);
    RUN_TEST(test_authorized_card_posts_one_event);
    RUN_TEST(test_card_held_posts_only_once);
    RUN_TEST(test_unauthorized_card_posts_no_event);
    RUN_TEST(test_intermittent_reads_while_held_do_not_repost);
    RUN_TEST(test_card_removed_after_consecutive_empty_reads);
    RUN_TEST(test_two_empty_reads_are_not_a_removal);

    return UNITY_END();
}

