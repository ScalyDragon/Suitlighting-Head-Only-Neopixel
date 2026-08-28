#include <unity.h>

#include "../../lib/helperStructures.h"
#include "../../lib/LEDStrip/LEDStrip.h"
#include "../../lib/NeopixelAnimator/NeopixelAnimator.h"
#include "../../lib/ButtonHandler/ButtonHandler.h"
#include "../../lib/FanController/FanController.h"
#include "../../lib/RGBStripManager/RGBStripManager.h"

namespace ArduinoStub {
int digitalReadValue = 1;
uint32_t nowMs = 0;
uint32_t ledcDuty = 0;
int ledcPin = -1;
int analogValues[64] = {};
void (*interruptHandler)(void *) = nullptr;
void *interruptArgument = nullptr;

void reset() {
    digitalReadValue = 1;
    nowMs = 0;
    ledcDuty = 0;
    ledcPin = -1;
    for (int &value : analogValues) value = -1;
    interruptHandler = nullptr;
    interruptArgument = nullptr;
}
}

SerialStub Serial;

namespace {

class TestLEDStrip : public LEDStrip {
public:
    TestLEDStrip(int pixelCount, int loopRateHz)
            : LEDStrip(pixelCount, loopRateHz) {}

    void init() override { initialized = true; }

    bool initialized = false;
    int refreshCount = 0;

protected:
    void updateStripFromBuffer() override { refreshCount++; }
};

void assertColor(const Color &expected, const Color &actual, float tolerance = 0.0001f) {
    TEST_ASSERT_FLOAT_WITHIN(tolerance, expected.r, actual.r);
    TEST_ASSERT_FLOAT_WITHIN(tolerance, expected.g, actual.g);
    TEST_ASSERT_FLOAT_WITHIN(tolerance, expected.b, actual.b);
}

void test_saturating_integer_arithmetic() {
    TEST_ASSERT_EQUAL_UINT16(0xFFFF, sadd16(0xFFFF, 1));
    TEST_ASSERT_EQUAL_UINT16(100, sadd16(75, 25));
    TEST_ASSERT_EQUAL_UINT32(0xFFFFFFFF, sadd32(0xFFFFFFFF, 1));
    TEST_ASSERT_EQUAL_UINT32(100, sadd32(75, 25));
    TEST_ASSERT_EQUAL_UINT16(0, ssub16(100, 1000));
    TEST_ASSERT_EQUAL_UINT16(10, ssub16(50, 40));
    TEST_ASSERT_EQUAL_UINT32(0, ssub32(100, 1000));
    TEST_ASSERT_EQUAL_UINT32(10, ssub32(50, 40));
}

void test_float_saturation_and_rounding() {
    TEST_ASSERT_EQUAL_UINT16(0, satf16(-1.0f));
    TEST_ASSERT_EQUAL_UINT16(13, satf16(12.5f));
    TEST_ASSERT_EQUAL_UINT16(0xFFFF, satf16(99999.0f));
    TEST_ASSERT_EQUAL_UINT8(0, satf8(-1.0f));
    TEST_ASSERT_EQUAL_UINT8(13, satf8(12.5f));
    TEST_ASSERT_EQUAL_UINT8(0xFF, satf8(999.0f));
}

void test_color_vector_operations() {
    Color color(3, 4, 0);
    TEST_ASSERT_EQUAL_INT(5, color.length());
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 5.0f, color.lengthf());
    assertColor(Color(0.6f, 0.8f, 0), color.normalized());
    assertColor(Color(4, 6, 3), color.add(Color(1, 2, 3)));
    assertColor(Color(2, 2, -3), color.sub(Color(1, 2, 3)));
    assertColor(Color(1.5f, 2, 0), color.smul(0.5f));
}

void test_color_conversion_clamps_and_formats() {
    Color color(300, 12.5f, -1);
    assertColor(Color(255, 13, 0), color.toInt8());
    TEST_ASSERT_EQUAL_UINT32(0xFF0D00, color.toInt8().getUint32OfColor());
    TEST_ASSERT_EQUAL_STRING("FF0D00", color.toInt8().toUint8String().c_str());
    assertColor(Color(0xAB, 0xCD, 0xEF), color.getColorOfUint32(0xABCDEF));
}

void test_persistence_defaults() {
    Persistence state;
    TEST_ASSERT_FALSE(state.touch);
    TEST_ASSERT_TRUE(state.adaptiveTouch);
    TEST_ASSERT_FALSE(state.savePersistentToEMMC);
    TEST_ASSERT_EQUAL_UINT8(0, state.fanspeed);
    TEST_ASSERT_EQUAL_UINT32(0, state.fanRPM);
    assertColor(Color(0, 0, 255), state.idleColor);
    assertColor(Color(255, 0, 0), state.boopColor);
}

void test_led_strip_clamps_pixel_area() {
    TestLEDStrip strip(4, 100);
    strip.setPixelArea(-10, 2, Color(10, 20, 30));
    strip.setPixelArea(3, 99, Color(40, 50, 60));
    assertColor(Color(10, 20, 30), strip.getTargetColorBuffer()[0]);
    assertColor(Color(10, 20, 30), strip.getTargetColorBuffer()[1]);
    assertColor(Color(), strip.getTargetColorBuffer()[2]);
    assertColor(Color(40, 50, 60), strip.getTargetColorBuffer()[3]);
}

void test_led_strip_ignores_empty_and_reversed_areas() {
    TestLEDStrip strip(2, 100);
    strip.setPixelArea(1, 1, Color(255, 0, 0));
    strip.setPixelArea(2, 1, Color(255, 0, 0));
    assertColor(Color(), strip.getTargetColorBuffer()[0]);
    assertColor(Color(), strip.getTargetColorBuffer()[1]);
}

void test_led_strip_direct_mode_reaches_target_in_one_tick() {
    TestLEDStrip strip(2, 100);
    strip.setMode(0);
    strip.setPixelArea(0, 2, Color(20, 30, 40));
    strip.loopHandler();
    assertColor(Color(20, 30, 40), strip.getCurrentColorBuffer()[0]);
    assertColor(Color(20, 30, 40), strip.getCurrentColorBuffer()[1]);
    TEST_ASSERT_EQUAL_INT(1, strip.refreshCount);
}

void test_led_strip_smooth_mode_uses_vector_stepwidth() {
    TestLEDStrip strip(1, 100);
    strip.setMode(1);
    strip.setSmoothStepwidth(5.0f);
    strip.setPixelArea(0, 1, Color(3, 4, 0));
    strip.loopHandler();
    assertColor(Color(3, 4, 0), strip.getCurrentColorBuffer()[0]);

    strip.setPixelArea(0, 1, Color(9, 12, 0));
    strip.loopHandler();
    assertColor(Color(6, 8, 0), strip.getCurrentColorBuffer()[0]);
}

void test_led_strip_wait_mode_delays_update() {
    TestLEDStrip strip(1, 10);
    strip.setMode(2);
    strip.setColorWait(0.3f);
    strip.setPixelArea(0, 1, Color(100, 0, 0));
    strip.loopHandler();
    assertColor(Color(), strip.getCurrentColorBuffer()[0]);
    strip.loopHandler();
    assertColor(Color(), strip.getCurrentColorBuffer()[0]);
    strip.loopHandler();
    assertColor(Color(100, 0, 0), strip.getCurrentColorBuffer()[0]);
}

void test_animator_selects_idle_and_boop_colors() {
    Persistence state;
    state.idleColor = Color(1, 2, 3);
    state.boopColor = Color(10, 20, 30);
    state.fadespeed = 10;
    TestLEDStrip strip(32, 1000);
    NeopixelAnimator animator(&strip, &state);

    animator.init();
    state.touch = false;
    animator.loopHandler();
    assertColor(state.idleColor, strip.getTargetColorBuffer()[0]);

    state.touch = true;
    animator.loopHandler();
    assertColor(state.boopColor, strip.getTargetColorBuffer()[0]);
}

void test_animator_button_release_toggles_blackout() {
    Persistence state;
    state.idleColor = Color(1, 2, 3);
    state.fadespeed = 10;
    TestLEDStrip strip(32, 1000);
    NeopixelAnimator animator(&strip, &state);

    state.buttonPressed = false;
    animator.loopHandler();
    state.buttonPressed = true;
    animator.loopHandler();
    assertColor(Color(), strip.getTargetColorBuffer()[0]);

    state.buttonPressed = false;
    animator.loopHandler();
    state.buttonPressed = true;
    animator.loopHandler();
    assertColor(state.idleColor, strip.getTargetColorBuffer()[0]);
}

void test_animator_color_setters_update_persistence() {
    Persistence state;
    TestLEDStrip strip(32, 1000);
    NeopixelAnimator animator(&strip, &state);
    animator.setIdleColor(Color(4, 5, 6));
    animator.setBoopColor(Color(7, 8, 9));
    assertColor(Color(4, 5, 6), state.idleColor);
    assertColor(Color(7, 8, 9), state.boopColor);
    TEST_ASSERT_TRUE(state.try_lock());
    state.unlock();
}

void test_button_handler_debounces_press_and_release() {
    ArduinoStub::reset();
    Persistence state;
    ButtonHandler button(&state, 12);
    button.init();
    TEST_ASSERT_FALSE(state.configMode);

    ArduinoStub::digitalReadValue = 0;
    for (int i = 0; i < 10; i++) button.loopHandler();
    TEST_ASSERT_TRUE(state.buttonPressed);

    ArduinoStub::digitalReadValue = 1;
    for (int i = 0; i < 20; i++) button.loopHandler();
    TEST_ASSERT_FALSE(state.buttonPressed);
    TEST_ASSERT_TRUE(state.try_lock());
    state.unlock();
}

void test_button_handler_reads_config_mode_only_during_init() {
    ArduinoStub::reset();
    ArduinoStub::digitalReadValue = 0;
    Persistence state;
    ButtonHandler button(&state, 12);
    button.init();
    TEST_ASSERT_TRUE(state.configMode);
    ArduinoStub::digitalReadValue = 1;
    for (int i = 0; i < 20; i++) button.loopHandler();
    TEST_ASSERT_TRUE(state.configMode);
}

void test_fan_controller_clamps_speed_and_writes_pwm() {
    ArduinoStub::reset();
    Persistence state;
    FanController fan(&state, 19);
    fan.init();
    TEST_ASSERT_EQUAL_INT(19, ArduinoStub::ledcPin);
    TEST_ASSERT_EQUAL_UINT32(0, ArduinoStub::ledcDuty);
    TEST_ASSERT_FALSE(fan.hasTacho());

    fan.setSpeedPercent(50);
    TEST_ASSERT_EQUAL_UINT8(50, fan.getSpeedPercent());
    TEST_ASSERT_EQUAL_UINT32(127, ArduinoStub::ledcDuty);
    TEST_ASSERT_EQUAL_UINT8(50, state.fanspeed);
    TEST_ASSERT_TRUE(state.savePersistentToEMMC);

    fan.setSpeedPercent(200);
    TEST_ASSERT_EQUAL_UINT8(100, fan.getSpeedPercent());
    TEST_ASSERT_EQUAL_UINT32(255, ArduinoStub::ledcDuty);
}

void test_fan_controller_supports_inverted_output() {
    ArduinoStub::reset();
    Persistence state;
    state.fanspeed = 25;
    FanController fan(&state, 19, -1, true);
    fan.init();
    TEST_ASSERT_EQUAL_UINT32(192, ArduinoStub::ledcDuty);
}

void test_fan_controller_calculates_rpm_from_tacho_pulses() {
    ArduinoStub::reset();
    Persistence state;
    FanController fan(&state, 19, 18);
    fan.init();
    TEST_ASSERT_TRUE(fan.hasTacho());
    TEST_ASSERT_NOT_NULL(ArduinoStub::interruptHandler);
    for (int i = 0; i < 20; i++) {
        ArduinoStub::interruptHandler(ArduinoStub::interruptArgument);
    }
    ArduinoStub::nowMs = 1000;
    fan.loopHandler();
    TEST_ASSERT_EQUAL_UINT32(600, fan.getRPM());
    TEST_ASSERT_EQUAL_UINT32(600, state.fanRPM);
}

void test_rgb_strip_writes_normal_and_inverted_values() {
    ArduinoStub::reset();
    RGBStripManager normal(1, 2, 3, 1000, false);
    normal.init();
    normal.setMode(0);
    normal.setPixelArea(0, 1, Color(10, 20, 30));
    normal.loopHandler();
    TEST_ASSERT_EQUAL_INT(10, ArduinoStub::analogValues[1]);
    TEST_ASSERT_EQUAL_INT(20, ArduinoStub::analogValues[2]);
    TEST_ASSERT_EQUAL_INT(30, ArduinoStub::analogValues[3]);

    RGBStripManager inverted(4, 5, 6, 1000, true);
    inverted.init();
    inverted.setMode(0);
    inverted.setPixelArea(0, 1, Color(10, 20, 30));
    inverted.loopHandler();
    TEST_ASSERT_EQUAL_INT(245, ArduinoStub::analogValues[4]);
    TEST_ASSERT_EQUAL_INT(235, ArduinoStub::analogValues[5]);
    TEST_ASSERT_EQUAL_INT(225, ArduinoStub::analogValues[6]);
}

}

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_saturating_integer_arithmetic);
    RUN_TEST(test_float_saturation_and_rounding);
    RUN_TEST(test_color_vector_operations);
    RUN_TEST(test_color_conversion_clamps_and_formats);
    RUN_TEST(test_persistence_defaults);
    RUN_TEST(test_led_strip_clamps_pixel_area);
    RUN_TEST(test_led_strip_ignores_empty_and_reversed_areas);
    RUN_TEST(test_led_strip_direct_mode_reaches_target_in_one_tick);
    RUN_TEST(test_led_strip_smooth_mode_uses_vector_stepwidth);
    RUN_TEST(test_led_strip_wait_mode_delays_update);
    RUN_TEST(test_animator_selects_idle_and_boop_colors);
    RUN_TEST(test_animator_button_release_toggles_blackout);
    RUN_TEST(test_animator_color_setters_update_persistence);
    RUN_TEST(test_button_handler_debounces_press_and_release);
    RUN_TEST(test_button_handler_reads_config_mode_only_during_init);
    RUN_TEST(test_fan_controller_clamps_speed_and_writes_pwm);
    RUN_TEST(test_fan_controller_supports_inverted_output);
    RUN_TEST(test_fan_controller_calculates_rpm_from_tacho_pulses);
    RUN_TEST(test_rgb_strip_writes_normal_and_inverted_values);
    return UNITY_END();
}
