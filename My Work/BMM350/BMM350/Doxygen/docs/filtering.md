
@page filtering Angle Filtering System

# 🧠 Angle Filtering Utilities for BMM350

This filter are useful when dealing with noisy or fast-changing magnetic angle outputs. They include an **adaptive low-pass filter**, **angle unwrapping**, and **angle wrapping** logic.

---

@section overview Overview
This module provides adaptive filtering for angular data, specifically designed for:
- Smoothing noisy sensor readings (e.g., magnetometer yaw angles)
- Handling 0°/360° wrap-around transitions
- Automatically adjusting responsiveness based on input changes

@section adaptive_filter Adaptive Angle Filter
```c
 float adaptive_filter(filter_struct* filter, float raw_data)
```
@brief Applies adaptive low-pass filtering to angular data.

@param[in] filter Pointer to filter state structure (maintains previous data and alpha)
@param[in] raw_data New raw angle input (degrees)
@return Filtered angle (0-360°)

@details
- Uses unwrapped angle differences to properly handle 360° transitions
- Dynamically adjusts smoothing factor (alpha) based on angular velocity:
  - Small changes (< CHANGE_THRESHOLD): Strong smoothing (ALPHA_MIN)
  - Large changes (≥ CHANGE_THRESHOLD): Fast response (ALPHA_MAX)

@warning Configuration Notes
- **Alpha Values**:
  ```c
  #define CHANGE_THRESHOLD 1.0f  // noise Degrees (adjust for sensitivity)
  #define ALPHA_MIN  0.001f  // Lower = smoother but slower response
  #define ALPHA_MAX  0.3f    // Higher = faster response but more noise
  ```

* These values were tested for BMM350 magnetic heading application. Do not modify unless fully understanding the tradeoffs. 



---
@author MOHAMMED ABDULALEM | محمد عبدالعالم