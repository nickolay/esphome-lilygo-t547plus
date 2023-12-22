#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/version.h"
#include "esphome/components/display/display_buffer.h"

#include "epd_driver.h"

#ifdef USE_ESP32_FRAMEWORK_ARDUINO

namespace esphome {
namespace t547 {

#if ESPHOME_VERSION_CODE >= VERSION_CODE(2023, 12, 0)
class T547 : public display::DisplayBuffer {
#else
class T547 : public PollingComponent, public display::DisplayBuffer {
#endif  // VERSION_CODE(2023, 12, 0)
 public:
  void set_greyscale(bool greyscale) {
    this->greyscale_ = greyscale;
  }

  float get_setup_priority() const override;

  void dump_config() override;

  void display();
  void clean();
  void update() override;

  void setup() override;

  uint8_t get_panel_state() { return this->panel_on_; }
  bool get_greyscale() { return this->greyscale_; }

#if ESPHOME_VERSION_CODE >= VERSION_CODE(2022,6,0)
  display::DisplayType get_display_type() override {
    return get_greyscale() ? display::DisplayType::DISPLAY_TYPE_GRAYSCALE : display::DisplayType::DISPLAY_TYPE_BINARY;
  }
#endif

 protected:
  void draw_absolute_pixel_internal(int x, int y, Color color) override;

  void eink_off_();
  void eink_on_();


  int get_width_internal() override { return 960; }

  int get_height_internal() override { return 540; }

  size_t get_buffer_length_();


  uint8_t panel_on_ = 0;
  uint8_t temperature_;

  bool greyscale_;

};

}  // namespace T547
}  // namespace esphome

#endif  // USE_ESP32_FRAMEWORK_ARDUINO
