

# 🧭 Application and Use 

This page shows how to use the BMM350 driver to get YAW in the main.c file.


* **The BMM350 driver uses the DWT.c source file which is inside the BIOS folder, for delays in microseconds. Therefore it is needed to use the function DWT_init.c in our initialization file, (e.g in set()).**

* Before using the driver make sure that the function DWT_init() was set at initialization.

---
## Getting YAW data from the sensor:
    
First we need to call the function bmm350_init(..) once to configure the BMM350 chip, best place is in the main(). Best practice is to store the returned value of the function to a bmm350_status_e variable type, the returned value will clarify whether the initialization went OK or faced an error. 

Secondly, to get the YAW data, call the function bmm350_read_yaw(...) **periodically**, best is each 5ms. Because the function will create a delay inside the while loop if called continuously with full clock speed. The bmm350_read_yaw(...) has 3 arguments:
    - dev:      Which is a pointer to the device structure. **already declared inside BMM350.h as -BMM350-**
    - uint:     An enum that indicates whether you want the returned reading to be in Radian or Degrees.
        possible values: RADIAN, DEGREE.
    - range:    Same enum which holds also the range of angle **if Degrees is selected**. In case of radian just put NON.
        possible values: FROM_0_TO_360, FROM_NEG_180_TO_POS_180, NON.



## main.c example

```c
bmm350_status_e rslt; // bmm350_status_e type to store the result of BMM350_ini()
float angle = 0.0f;

int main(){
    int timer = 0; // Timer to make non-blocking delay
    rslt = BMM350_init(&BMM350, &hi2c2); // BMM350 is declared in BMM350.h

    while(1){
        if(HAL_GetTick() - timer > 5){
            angle = bmm350_read_yaw(&BMM350, DEGREE, FROM_0_TO_360); // Get YAW angle
            timer = HAL_GetTick();
        }
        
    }
}
```






---
@author MOHAMMED ABDULALEM | محمد عبدالعالم