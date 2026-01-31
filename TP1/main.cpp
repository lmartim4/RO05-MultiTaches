#include <iostream>

#include "time.h"

double timespec_to_ms(const timespec& time_ts){
    return time_ts.tv_sec * 1000.0 + time_ts.tv_nsec / (1000.0 * 1000.0);
}

timespec timespec_from_ms(double time_ms){
    timespec result;
    result.tv_sec = static_cast<time_t>(time_ms / 1000.0);
    result.tv_nsec = static_cast<long>((time_ms - result.tv_sec * 1000.0) * 1000000.0);

    // Normalize to ensure tv_nsec is always positive
    if (result.tv_nsec < 0) {
        result.tv_sec--;
        result.tv_nsec += 1000000000;
    }

    return result;
}
timespec timespec_now(){
    timespec current_time;
    clock_gettime(CLOCK_REALTIME, &current_time);
    return current_time;
}
timespec timespec_negate(const timespec& time_ts){
    timespec result;
    result.tv_sec = -time_ts.tv_sec;
    result.tv_nsec = -time_ts.tv_nsec;

    if (result.tv_nsec < 0) {
        result.tv_sec--;
        result.tv_nsec += 1000000000;
    }

    return result;
}
void timespec_wait(const timespec& delay_ts){
    nanosleep(&delay_ts, NULL);
}

timespec  operator- (const timespec& time_ts){
    return timespec_negate(time_ts);
}
timespec  operator+ (const timespec& time1_ts, const timespec& time2_ts){
    timespec result;
    result.tv_sec = time1_ts.tv_sec + time2_ts.tv_sec;
    result.tv_nsec = time1_ts.tv_nsec + time2_ts.tv_nsec;

    // Normalize to ensure tv_nsec is always in range [0, 1000000000)
    while (result.tv_nsec >= 1000000000) {
        result.tv_sec++;
        result.tv_nsec -= 1000000000;
    }
    while (result.tv_nsec < 0) {
        result.tv_sec--;
        result.tv_nsec += 1000000000;
    }

    return result;
}
timespec  operator- (const timespec& time1_ts, const timespec& time2_ts){
    timespec result;
    result.tv_sec = time1_ts.tv_sec - time2_ts.tv_sec;
    result.tv_nsec = time1_ts.tv_nsec - time2_ts.tv_nsec;

    if (result.tv_nsec >= 1000000000) {
        result.tv_sec++;
        result.tv_nsec -= 1000000000;
    } else if (result.tv_nsec < 0) {
        result.tv_sec--;
        result.tv_nsec += 1000000000;
    }

    return result;
}
timespec& operator+= (timespec& time_ts, const timespec& delay_ts){
    time_ts = time_ts + delay_ts;
    return time_ts;
}
timespec& operator-= (timespec& time_ts, const timespec& delay_ts){
    time_ts = time_ts - delay_ts;
    return time_ts;
}
bool operator== (const timespec& time1_ts, const timespec& time2_ts){
    return (time1_ts.tv_sec == time2_ts.tv_sec) && (time1_ts.tv_nsec == time2_ts.tv_nsec);
}
bool operator!= (const timespec& time1_ts, const timespec& time2_ts){
    return !(time1_ts == time2_ts);
}
bool operator< (const timespec& time1_ts, const timespec& time2_ts){
    if (time1_ts.tv_sec < time2_ts.tv_sec) return true;
    if (time1_ts.tv_sec > time2_ts.tv_sec) return false;
    return time1_ts.tv_nsec < time2_ts.tv_nsec;
}
bool operator> (const timespec& time1_ts, const timespec& time2_ts){
    if (time1_ts.tv_sec > time2_ts.tv_sec) return true;
    if (time1_ts.tv_sec < time2_ts.tv_sec) return false;
    return time1_ts.tv_nsec > time2_ts.tv_nsec;
}

int main(int argc, char** argv){

    std::cout << "=== TimeSpec Utility Functions Test Suite ===" << std::endl << std::endl;

    // Test 1: timespec_to_ms
    std::cout << "Test 1: timespec_to_ms" << std::endl;
    timespec ts1;
    ts1.tv_sec = 2;
    ts1.tv_nsec = 700000000;
    double ms1 = timespec_to_ms(ts1);
    std::cout << "  Input: " << ts1.tv_sec << "s " << ts1.tv_nsec << "ns" << std::endl;
    std::cout << "  Output: " << ms1 << " ms" << std::endl << std::endl;

    // Test 2: timespec_from_ms
    std::cout << "Test 2: timespec_from_ms" << std::endl;
    double test_ms = 3456.789;
    timespec ts2 = timespec_from_ms(test_ms);
    std::cout << "  Input: " << test_ms << " ms" << std::endl;
    std::cout << "  Output: " << ts2.tv_sec << "s " << ts2.tv_nsec << "ns" << std::endl;
    std::cout << "  Verify: " << timespec_to_ms(ts2) << " ms" << std::endl << std::endl;

    // Test 3: timespec_negate
    std::cout << "Test 3: timespec_negate" << std::endl;
    timespec ts3;
    ts3.tv_sec = 5;
    ts3.tv_nsec = 500000000;
    timespec ts3_neg = timespec_negate(ts3);
    std::cout << "  Input: " << ts3.tv_sec << "s " << ts3.tv_nsec << "ns" << std::endl;
    std::cout << "  Negated: " << ts3_neg.tv_sec << "s " << ts3_neg.tv_nsec << "ns" << std::endl << std::endl;

    // Test 4: Unary minus operator
    std::cout << "Test 4: Unary minus operator" << std::endl;
    timespec ts4;
    ts4.tv_sec = 3;
    ts4.tv_nsec = 250000000;
    timespec ts4_neg = -ts4;
    std::cout << "  Input: " << ts4.tv_sec << "s " << ts4.tv_nsec << "ns" << std::endl;
    std::cout << "  Result (-ts): " << ts4_neg.tv_sec << "s " << ts4_neg.tv_nsec << "ns" << std::endl << std::endl;

    // Test 5: Addition operator
    std::cout << "Test 5: Addition operator" << std::endl;
    timespec ts5a, ts5b;
    ts5a.tv_sec = 1;
    ts5a.tv_nsec = 800000000;
    ts5b.tv_sec = 0;
    ts5b.tv_nsec = 300000000;
    timespec ts5_sum = ts5a + ts5b;
    std::cout << "  A: " << ts5a.tv_sec << "s " << ts5a.tv_nsec << "ns" << std::endl;
    std::cout << "  B: " << ts5b.tv_sec << "s " << ts5b.tv_nsec << "ns" << std::endl;
    std::cout << "  A + B: " << ts5_sum.tv_sec << "s " << ts5_sum.tv_nsec << "ns" << std::endl;
    std::cout << "  (should normalize to 2s 100000000ns)" << std::endl << std::endl;

    // Test 6: Subtraction operator
    std::cout << "Test 6: Subtraction operator" << std::endl;
    timespec ts6a, ts6b;
    ts6a.tv_sec = 5;
    ts6a.tv_nsec = 200000000;
    ts6b.tv_sec = 2;
    ts6b.tv_nsec = 700000000;
    timespec ts6_diff = ts6a - ts6b;
    std::cout << "  A: " << ts6a.tv_sec << "s " << ts6a.tv_nsec << "ns" << std::endl;
    std::cout << "  B: " << ts6b.tv_sec << "s " << ts6b.tv_nsec << "ns" << std::endl;
    std::cout << "  A - B: " << ts6_diff.tv_sec << "s " << ts6_diff.tv_nsec << "ns" << std::endl;
    std::cout << "  (should normalize to 2s 500000000ns)" << std::endl << std::endl;

    // Test 7: += operator
    std::cout << "Test 7: += operator" << std::endl;
    timespec ts7;
    ts7.tv_sec = 1;
    ts7.tv_nsec = 0;
    timespec ts7_add;
    ts7_add.tv_sec = 0;
    ts7_add.tv_nsec = 500000000;
    std::cout << "  Before: " << ts7.tv_sec << "s " << ts7.tv_nsec << "ns" << std::endl;
    ts7 += ts7_add;
    std::cout << "  After += (0s 500000000ns): " << ts7.tv_sec << "s " << ts7.tv_nsec << "ns" << std::endl << std::endl;

    // Test 8: -= operator
    std::cout << "Test 8: -= operator" << std::endl;
    timespec ts8;
    ts8.tv_sec = 3;
    ts8.tv_nsec = 0;
    timespec ts8_sub;
    ts8_sub.tv_sec = 1;
    ts8_sub.tv_nsec = 250000000;
    std::cout << "  Before: " << ts8.tv_sec << "s " << ts8.tv_nsec << "ns" << std::endl;
    ts8 -= ts8_sub;
    std::cout << "  After -= (1s 250000000ns): " << ts8.tv_sec << "s " << ts8.tv_nsec << "ns" << std::endl << std::endl;

    // Test 9: Comparison operators
    std::cout << "Test 9: Comparison operators" << std::endl;
    timespec ts9a, ts9b, ts9c;
    ts9a.tv_sec = 2;
    ts9a.tv_nsec = 500000000;
    ts9b.tv_sec = 2;
    ts9b.tv_nsec = 500000000;
    ts9c.tv_sec = 1;
    ts9c.tv_nsec = 0;
    std::cout << "  A: " << ts9a.tv_sec << "s " << ts9a.tv_nsec << "ns" << std::endl;
    std::cout << "  B: " << ts9b.tv_sec << "s " << ts9b.tv_nsec << "ns" << std::endl;
    std::cout << "  C: " << ts9c.tv_sec << "s " << ts9c.tv_nsec << "ns" << std::endl;
    std::cout << "  A == B: " << (ts9a == ts9b ? "true" : "false") << std::endl;
    std::cout << "  A != C: " << (ts9a != ts9c ? "true" : "false") << std::endl;
    std::cout << "  C < A: " << (ts9c < ts9a ? "true" : "false") << std::endl;
    std::cout << "  A > C: " << (ts9a > ts9c ? "true" : "false") << std::endl << std::endl;

    // Test 10: timespec_now (just show current time)
    std::cout << "Test 10: timespec_now" << std::endl;
    timespec ts_now = timespec_now();
    std::cout << "  Current time: " << ts_now.tv_sec << "s " << ts_now.tv_nsec << "ns" << std::endl;
    std::cout << "  In milliseconds: " << timespec_to_ms(ts_now) << " ms since epoch" << std::endl << std::endl;

    // Test 11: timespec_wait (demonstrate a short delay)
    std::cout << "Test 11: timespec_wait" << std::endl;
    timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 500000000; // 500ms
    std::cout << "  Waiting for 500ms..." << std::endl;
    timespec before = timespec_now();
    timespec_wait(delay);
    timespec after = timespec_now();
    timespec elapsed = after - before;
    std::cout << "  Actual elapsed: " << timespec_to_ms(elapsed) << " ms" << std::endl << std::endl;

    // Test 12: Negative tv_sec values
    std::cout << "Test 12: Negative tv_sec values" << std::endl;

    // Test 12a: timespec_to_ms with negative tv_sec
    timespec ts12a;
    ts12a.tv_sec = -2;
    ts12a.tv_nsec = -500000000;
    double ms12a = timespec_to_ms(ts12a);
    std::cout << "  Input: " << ts12a.tv_sec << "s " << ts12a.tv_nsec << "ns" << std::endl;
    std::cout << "  Output: " << ms12a << " ms (expected: -2500 ms)" << std::endl;

    // Test 12b: timespec_from_ms with negative milliseconds
    double neg_ms = -1500.0;
    timespec ts12b = timespec_from_ms(neg_ms);
    std::cout << "  Input: " << neg_ms << " ms" << std::endl;
    std::cout << "  Output: " << ts12b.tv_sec << "s " << ts12b.tv_nsec << "ns" << std::endl;
    std::cout << "  Verify back to ms: " << timespec_to_ms(ts12b) << " ms" << std::endl;

    // Test 12c: Operations with negative values
    timespec ts12c_pos, ts12c_neg;
    ts12c_pos.tv_sec = 3;
    ts12c_pos.tv_nsec = 0;
    ts12c_neg.tv_sec = -1;
    ts12c_neg.tv_nsec = -500000000;
    timespec ts12c_sum = ts12c_pos + ts12c_neg;
    std::cout << "  Positive: " << ts12c_pos.tv_sec << "s " << ts12c_pos.tv_nsec << "ns" << std::endl;
    std::cout << "  Negative: " << ts12c_neg.tv_sec << "s " << ts12c_neg.tv_nsec << "ns" << std::endl;
    std::cout << "  Sum: " << ts12c_sum.tv_sec << "s " << ts12c_sum.tv_nsec << "ns" << std::endl;
    std::cout << "  (expected: 1s 500000000ns)" << std::endl << std::endl;

    std::cout << "=== All tests completed ===" << std::endl;

    return 0;
}