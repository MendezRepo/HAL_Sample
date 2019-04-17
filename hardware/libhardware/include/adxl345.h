#ifndef __ADXL345_h__
#define __ADXL345_h__

#include <hardware/hardware.h>
#include <stdint.h>

typedef struct position {
    int32_t  x;
    int32_t  y;
    int32_t  z;
    uint64_t time;
};


typedef struct adxl345_device {

    struct hw_device_t common;

    /*
     * Get position from the adxl345
     */
    int (*getPosition)(struct adxl345_device *dev, struct position *pos);

    /*
     * get the sample rate
     */
    int (*getRate)(struct adxl345_device *dev, uint32_t *rate);

    /*
     * set sample rate
     */
    int (*setRate)(struct adxl345_device *dev, uint32_t rate);


};



#endif
