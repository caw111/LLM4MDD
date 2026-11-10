// Independent, fixed expectations derived from the public requirement/contract.
// Never included in model generation or repair prompts.
#include <cstdio>
static int failures = 0;
static void expect(bool condition, const char* id) {
    if (!condition) { std::printf("FAIL %s\n", id); ++failures; }
}
int main() {
    double temperature, humidity; bool due, high;
    ProgramPage1_Init(&ProgramPage1_instance);
    ProgramPage1(30.0, 42.0, 4999, &temperature, &humidity, &due, &high);
    expect(temperature == 30.0 && humidity == 42.0, "samples_follow_inputs");
    expect(!due, "before_boundary");
    expect(!high, "threshold_equal");
    ProgramPage1(30.1, 51.0, 1, &temperature, &humidity, &due, &high);
    expect(temperature == 30.1 && humidity == 51.0, "samples_follow_inputs");
    expect(due, "at_boundary");
    expect(high, "threshold_above");
    ProgramPage1(20.0, 60.0, 100, &temperature, &humidity, &due, &high);
    expect(!due, "one_step_pulse");
    expect(!high, "threshold_below");
    ProgramPage1(20.0, 60.0, 4999, &temperature, &humidity, &due, &high);
    expect(due, "carry_remainder");
    ProgramPage1(20.0, 60.0, 4900, &temperature, &humidity, &due, &high);
    expect(!due, "carry_remainder_before_boundary");
    ProgramPage1(20.0, 60.0, 1, &temperature, &humidity, &due, &high);
    expect(due, "carry_remainder_at_boundary");
    ProgramPage1_Init(&ProgramPage1_instance);
    ProgramPage1(10.0, 33.0, 1, &temperature, &humidity, &due, &high);
    expect(temperature == 10.0 && humidity == 33.0 && !due && !high, "reset");
    if (!failures) std::printf("PASS\n");
    return failures ? 1 : 0;
}
