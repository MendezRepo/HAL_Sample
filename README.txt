
This is a simple demo on how I would add a periferal driver to android.  I didn't have time to bring it all the way up to the frameworks but it would be simple addition.

The directory stucture for this demo is as follows:

/mnt/c/workspace/HAL_impl/
├── hardware
│   └── libhardware
│       ├── include
│       │   └── adxl345.h
│       └── modules
│           └── input
│               └── adxl345
│                   ├── adxl345.cpp
│                   └── Android.mk
└── kernel
    ├── arch
    │   └── arm
    │       ├── boot
    │       │   └── dts
    │       │       └── imx6q-adx34x.dtsi
    │       └── configs
    │           └── tegra_defconfig
    ├── drivers
    │   └── iio
    │       └── accel
    │           ├── adxl34x.c
    │           ├── adxl34x-i2c.c
    │           └── adxl34x-spi.c
    └── include
        └── input
            └── adxl34x.h
			
			
The kernel driver was added to the iio drivers section.  There was nothing special about this device that I choose just that it had a linux driver source and looked relatively easy to implement.  Also this accelerometer seems to be used quite a lot and has many blogs about it.  I don't actually have this device to test with so there might be some problems with the implementation.  The driver was downloaded from:

https://wiki.analog.com/resources/tools-software/linux-drivers/input-misc/adxl345

Also in this data sheet the proper initialization through the device tree was given for both i2c and spi.  I choose i2c implementation basically on a coin flip.

The HAL implementation is given in the hardware section and this is the part that I wrote myself.  I was not able to fully compile this and test it due to not having the hardware and android has changed the frameworks (yet again) and need to understand where everything has been moved to.  The basics of briging the device up to the HAL layer have been given though

I ran out of time as I mentioned to bring this through to the frameworks layer.

