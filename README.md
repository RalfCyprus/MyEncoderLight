# MyEncoderLight

A simple Arduino Nano-based project to control a 12-volt LED ceiling light using a rotary encoder.

## 🛠️ Requirements

To build this project, you will need:

- Arduino Nano  
- Rotary encoder module  
- Yellow LED  
- 12V LED ceiling light  
- MOSFET (FET)  
- LDR (Light Dependent Resistor)  
- Various resistors  

## ⚙️ How It Works

Once powered, the Arduino Nano boots up and is ready to operate. The rotary encoder controls the brightness of the ceiling light:

- **Rotate the encoder** to increase or decrease brightness.
- **Single press**:
  - If the light is **on**, it dims to 0 and turns off. The yellow LED turns on, and the brightness value is saved.
  - If the light is **off**, it restores the last saved brightness value.
- **Double press** sets the ceiling light to **full brightness**.
- **Long press** activates a timed full brightness mode. The light goes to maximum brightness, stays on for a defined period, then dims to 0 and turns off. You can adjust the delay in the sketch.

## 🌗 Ambient Light Sensing

An LDR is used to detect ambient brightness:

- In bright conditions, the yellow LED is fully lit.
- As the ambient light decreases, the LED dims accordingly.

> *Note: You may need to adjust the LDR sensor value mapping in the code.*

## 🔁 Summary

- **Rotate encoder** → Adjust brightness  
- **Single press** → Toggle saved brightness  
- **Double press** → Max brightness  
- **Long press** → Timed brightness mode  
- **Yellow LED** → Indicates saved brightness value (only when light is off)

## 📄 Sketch


