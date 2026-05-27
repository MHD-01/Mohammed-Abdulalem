
# Errors

### This page outlines the potential errors that may occur when interfacing with the BMM350 sensor.

The library uses the bmm350_status_e type (typically referred to as rslt) as the return status for most functions. This approach provides improved traceability, enhances reliability, and facilitates a smoother debugging process.


**Note: Internal errors are extremely rare, as the library has been thoroughly tested. Such errors are unlikely to occur unless there have been modifications to internal functions or the bmm350_init() function.**

## Error Handling Strategy

After each operation, the status is checked. If an issue is detected, the function will return an appropriate error code. Below are some of the expected errors and their possible causes:


* BMM35_E_NULL                  -> There is no returned value, maybe the status variable weren't initialized correctly, or the functions did not run.
* BMM350_E_NULL_PTR             -> The device structure is NULL, not initialized.
* BMM350_E_PMU_CMD_S_NULL_PTR   -> This is internal error The bmm350_pmu_cmd_status_s structure is NULL when the function bmm350_get_pmu_cmd_status \ref bmm350_get_pmu_cmd_status(), This is an internal error, never edit any thing except you understand the function.
* Bmm350_DEV_NOT_READY          -> This means I2C communication failed.
* BMM350_E_W_COM_FAIL           -> This internal error happens when writing value into a register fails, this might be for i2c communication error or delay time in initializing process is not wrong.
* BMM350_E_R_COM_FAIL           -> This internal error happens when reading from a register fails, this might be caused by i2c communication error or delay time in initializing process is wrong.
* BMM350_E_DEV_NOT_FOUND        -> The chip address obtained is wrong, probably communication error check wiring and power.
* BMM350_E_INVALID_CONFIG       -> This is internal error, invalid power mode.
* The following errors happened from OTP reading and all of them are internal errors, if the repeated and the chip never works, there is a small chance that a manufacture error or burned or not powered.
    * BMM350_E_OTP_BOOT
    * BMM350_E_OTP_PAGE_RD
    * BMM350_E_OTP_PAGE_PRG
    * BMM350_E_OTP_SIGN
    * BMM350_E_OTP_INV_CMD
    * BMM350_E_OTP_UNDEFINED

* BMM350_E_ALL_AXIS_DISABLED        -> This means that the Register (0x05) PMU_CMD_AXIS_EN was not initialize to enable to any axis, enabling axis happens in \ref bmm350_init() "bmm350_init()" function.
* BMM350_E_PMU_CMD_VALUE            -> This is internal error, caused in the \ref bmm350_magnetic_reset_and_wait() "bmm350_magnetic_reset_and_wait()" function, because error the power mode for \ref BMM350_PMU_CMD_BR "BMM350_PMU_CMD_BR" and \ref BMM350_PMU_CMD_FGR "BMM350_PMU_CMD_BR" were not set or the reading fails.
* BMM350_E_INV_PWR_MODE                 -> This is internal error, caused in \ref set_powermode() "set_powermode()" function, and as it says the power mode entered is invalid.
* BMM350_E_INV_ODR_AVG                  -> This error happens when using the function \ref bmm350_odr_avg_config() "bmm350_odr_avg_config()" which is also used in the \ref bmm350_init() "bmm350_init()" function, and as the error says the ODR and or AVG value is invalid.
* BMM350_E_SUSPEND_MODE                 -> This error happens when setting the power mode to suspend, it is not really an error, but in suspend mode you can not get data.
* BMM350_E_INTR_INV                     -> This is internal error, caused by the function \ref interrupt_config() "interrupt_config()", which indicates that the configuration was not set or reading from register was not successful.
* BMM350_INT_DATA_READY                 -> This enum indicates that the data is ready to be read.
* BMM350_INT_DATA_NOT_READY             -> This enum indicates that the data is not ready to be read.


## Final Notes

The library has been thoroughly tested and no internal errors were encountered during validation. If you face issues, ensure that external factors (e.g., power, wiring, or improper configuration) are not the root cause. Avoid modifying internal library functions unless you fully understand their behavior. Always follow UTM Robocon official guidelines when updating or adjusting the library.







---
@author MOHAMMED ABDULALEM | محمد عبدالعالم