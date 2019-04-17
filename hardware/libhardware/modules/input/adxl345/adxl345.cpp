
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <adxl345.h>

struct *adxl345_device;

#define ADXL345_SYSFS_PATH "/sys/devices/system/adxl34x"
#define ADXL_SYSFS_DISABLE   ADXL345_SYSFS_PATH "/disable"
#define ADXL_SYSFS_CALIBRATE ADXL345_SYSFS_PATH "/calibrate"
#define ADXL_SYSFS_RATE      ADXL345_SYSFS_PATH "/rate"
#define ADXL_SYSFS_POSITION  ADXL345_SYSFS_PATH "/position"
#define ADXL_SYSFS_SLEEP     ADXL345_SYSFS_PATH "/autosleep"

#define ADXL_I2C_ADDRESS 0x53
#define ADXL_BUS "/dev/i2c-1"
#define SUCCESS 0
#define FAIL -1

int adxl345_fd = -1;

//https://www.ctrlinux.com/blog/?p=38
//https://github.com/adanaila/ctrlinux/tree/master/i2c_uspace_app
//https://wiki.analog.com/resources/tools-software/linux-drivers/input-misc/adxl345
using namespace std;

static string readSysfs(string path) {
    string output;
    ifstream ifile(path);
    ifile >> output;
    ifile.close();
    return output;
}

static void writeSysfs(string path, string value) {
    ofstream ofile(path);
    ofile << value;
    ofile.close();
}

static uint64_t timestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    uint64_t ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}

static int  adxl345_init() {

    adxl345_fd = open(ADXL_BUS, O_RDWR);
    if(adxl345_fd == FAIL) {
        ALOGE("Could not open %s", ADXL_BUS)
        return FAIL;
    }

    int ret_val = ioctl(adxl345_fd, I2C_SLAVE, ADXL_I2C_ADDRESS);
    if (ret_val < 0) {
        ALOGE("Could not set I2C_SLAVE.");
        return FAIL;
    }
}

/*
 * gets the position from the sysfs.  From the driver
 * it will be returned as ... "(%d, %d, %d)\n"
 */
static int adxl345_getPosition(adxl345_device *dev, position *pos) {
    position output = {0};
    string strPosition = readSysfs(ADXL_SYSFS_POSITION);
    int32_t x, y, z;

    sscanf(strPosition.c_str(), "(%d, %d, %d)\n", &x, &y, &z);
    pos->x = x;
    pos->y = y;
    pos->z = z;

    pos->time = timestamp();

    return 0;
}

/*
 * gets that sample rate ...from driver it will be returned as "%u\n"
 */
static int adxl345_getRate(struct adxl345_device *dev, uint32_t *rate) {
    string strRate = readSysfs(ADXL_SYSFS_RATE);
    sscanf(strRate.c_str(), "%u\n", rate);
    return 0;
}


static int adxl345_setRate(struct adxl345_device *dev, uint32_t rate) {
    char strRate[32] = {0};
    sprintf(strRate, "%u", rate);
    writeSysfs(ADXL_SYSFS_RATE, strRate);
    return 0;
}

static struct hw_module_methods_t adxl345_methods =
{
     .open = adxl345_open
};

static int adxl345_close(hw_device_t *dev)
{
    if (dev) {
        free(dev);
        return SUCCESS;
    } else {
        return FAIL;
    }
}
static int adxl345_open(const hw_module_t* module, const char *id, hw_device_t** device) {

    int result = adxl345_init();
    if(result != SUCCESS) {
        ALOGE("Could not initialize the adxl345");
        return FAIL;
    }

    adxl345_device *dev = malloc(sizeof(adxl345_device));
    if(dev == NULL) {
        ALOGE("Could not allocate the adxl345 device object");
        return FAIL;
    }

    memset(dev, 0, sizeof(adxl345_device));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = ADXL345_API_VERSION_2_0;
    dev->common.module = (struct hw_module_t*) module;
    dev->common.close = adxl345_close;

    dev->getPosition = adxl345_getPosition;
    dev->getRate = adxl345_getRate;
    dev->setRate = adxl345_setRate;

    *device = (hw_device_t*) dev;
    return 0;
}


adxl345_device HAL_MODULE_INFO_SYM = {
    .common = {
        .tag                = HARDWARE_MODULE_TAG,
        .module_api_version = HARDWARE_MODULE_API_VERSION(1, 0),
        .hal_api_version    = HARDWARE_HAL_API_VERSION,
        .id                 = "adxl345_accelerometer",
        .name               = "Demo Fingerprint HAL",
        .author             = "The Android Open Source Project",
        .methods            = &adxl345_methods,
    },
};

