<h2 align="center">بسم الله الرحمن الرحيم</h2>

**Note that this documentation does not fully covers the BMM350, so you need to read through company's documentation and API**

Welcome to the **BMM350 API Library** documentation! This library provides a high-level interface for interacting with BMM350 to get Yaw and Earth magnetic feild.

This function will help user to use the library for extracting yaw and magnetic data from the BMM350 sensor.

There are several points that need to be understood before using the library.


# Getting started

# 1. Functions types

!!!!!!!!!!!!!!!!!!!!! Important Note !!!!!!!!!!!!!!!!!!!!!!!!!

 * User type 
    * function that is made for user.
    * \ref user "go to user functoins"

 * Internal & User type
    * functions are not for main use, but can be used for debuging and testing code functionality.
    * \ref user_internal "go to internal & user functions"

 * Internal type 
   * functions are not for user, except for library adjustment, fixing errors and developments.
   * \ref internal "go to internal functions"

# 2.BMM350 Shuttle Board 3.0

The BMM350 is a 3-axis geomagnetic sensor based on TMR (Tunnel Magneto-Resistance) technology. It is designed for high-performance applications in consumer electronics like smartphones, AR/VR headsets, robotics, and indoor navigation systems.

## Primary function
* Measures the Earth's magnetic field in X, Y, and Z axes to:

    * Provide accurate heading/orientation data (e.g., for compasses)
    * Assist in dead-reckoning and positioning (when GPS is unavailable)
    * Support motion tracking and stabilization

* Temperature Compensation

    * Built-in temperature sensor
    * Low TCO (Temperature Coefficient of Offset): ±200 nT/K
    * Low TCS (Temperature Coefficient of Sensitivity): ±0.01 %/K

## Key Features
* Operating Modes

    * Normal mode: Continuous measurements at selectable output data rates (ODR)
    * Forced mode: On-demand measurement (triggered by the host)
    * Suspend mode: Low-power standby mode

* Data Characteristics

    * ±2000 µT measurement range
    * ~0.1 µT resolution
    * 21-bit signed magnetic data format (X, Y, Z)
    * 24-bit unsigned temperature and sensor time

## 📈 BMM350 Sensor Performance Summary

| **Parameter**         | **Typical Value**        |
|-----------------------|--------------------------|
| Magnetic Range        | ±2000 µT per axis        |
| Resolution            | ~0.1 µT                  |
| Noise (X/Y axes)      | ~190 nT RMS @ 100 Hz     |
| Temperature Range     | -40°C to +85°C           |
| Supply Voltage        | VDD: 1.72–1.98V          |
| I/O Voltage           | VDDIO: 1.72–3.6V         |




# Areas for Further Development

* Get angle offset from North.
* 


---
@author MOHAMMED ABDULALEM | محمد عبدالعالم