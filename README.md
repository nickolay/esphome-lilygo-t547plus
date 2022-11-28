# esphome custom component(s) 

This repository contains a Display component to support the [LILYGO T5 4.7" E-paper display](http://www.lilygo.cn/prod_view.aspx?TypeId=50061&Id=1384&FId=t3:50061:3).
for [ESPHome](https://esphome.io/).  
For more info in the display components, see the [ESP Home Display Documentation](https://esphome.io/#display-components)

## 1. Usage

Use with [ESPHome](https://esphome.io/) (at least v1.18.0)

```yaml
# The Basic Display Definition in ESPhome

external_components:
  - source: github://tiaanv/esphome-components
    components: ["t547"]

display:
- platform: t547
  id: t5_display
  update_interval: 30s
```
