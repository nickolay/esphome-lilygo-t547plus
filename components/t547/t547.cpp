#include "t547.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"
#ifdef USE_ESP32_FRAMEWORK_ARDUINO

#include <esp32-hal-gpio.h>



void invertImage(esphome::display::Image& image) {
    // Parcourir tous les pixels de l'image
    for (int y = 0; y < image.get_height(); y++) {
        for (int x = 0; x < image.get_width(); x++) {
            // Inverser la valeur de chaque pixel
            uint16_t pixel_value = image.get_pixel(x, y);
            image.set_pixel(x, y, ~pixel_value);
        }
    }
}


void invertGrayImage(esphome::display::Image& image) {
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            uint8_t pixel = image.get_grayscale_pixel_(i, j);
            image.set_grayscale_pixel_(i, j, 255 - pixel);
        }
    }
}

namespace esphome {
namespace t547 {

static const char *const TAG = "t574";

void T547::setup() {
  ESP_LOGV(TAG, "Initialize called");
  epd_init();
  uint32_t buffer_size = this->get_buffer_length_();

  if (this->buffer_ != nullptr) {
    free(this->buffer_);  // NOLINT
  }

  this->buffer_ = (uint8_t *) ps_malloc(buffer_size);

  if (this->buffer_ == nullptr) {
    ESP_LOGE(TAG, "Could not allocate buffer for display!");
    this->mark_failed();
    return;
  }

  memset(this->buffer_, 0xFF, buffer_size);
  ESP_LOGV(TAG, "Initialize complete");
}

float T547::get_setup_priority() const { return setup_priority::PROCESSOR; }
size_t T547::get_buffer_length_() {
    return this->get_width_internal() * this->get_height_internal() / 2;
}

void T547::update() {
  this->do_update_();
  this->display();
}

void T547::image(int x, int y, esphome::display::Image *image, bool bNegative, Color color_on, Color color_off)
{
  if (bNegative)
      invertImage(image);

  DisplayBuffer::image(x, y, image, color_on, color_off);
}

void HOT T547::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (x >= this->get_width_internal() || y >= this->get_height_internal() || x < 0 || y < 0)
    return;
  uint8_t gs = 255 - ((color.red * 2126 / 10000) + (color.green * 7152 / 10000) + (color.blue * 722 / 10000));
  epd_draw_pixel(x, y, gs, this->buffer_);
}

void T547::dump_config() {
  LOG_DISPLAY("", "T547", this);
  LOG_UPDATE_INTERVAL(this);
}

void T547::eink_off_() {
  ESP_LOGV(TAG, "Eink off called");
  if (panel_on_ == 0)
    return;
  epd_poweroff();
  panel_on_ = 0;
}

void T547::eink_on_() {
  ESP_LOGV(TAG, "Eink on called");
  if (panel_on_ == 1)
    return;
  epd_poweron();
  panel_on_ = 1;
}

void T547::display() {
  ESP_LOGV(TAG, "Display called");
  uint32_t start_time = millis();

  epd_poweron();
  epd_clear();
  epd_draw_grayscale_image(epd_full_screen(), this->buffer_);
  epd_poweroff();

  ESP_LOGV(TAG, "Display finished (full) (%ums)", millis() - start_time);
}

}  // namespace T547
}  // namespace esphome

#endif  // USE_ESP32_FRAMEWORK_ARDUINO
