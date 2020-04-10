/*
 * Copyright 2020 - Patrick Rohr
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include "config.h"

#define DRIVER_AUTHOR "Patrick Rohr <patrickrohr92@gmail.com>"
#define DRIVER_DESC "Sample WiFi Driver"


// TODO this should be allocated dynamically.
struct pr_device_handle g_handle = {};

static int __init pr_device_init(void)
{
    // TODO: Actual init function here should probably register PCIe device.
    // Some PCIe probe callback should then trigger the initialization of
    // the rest of the driver.    

    // TODO: remove this from here
    return pr_config_init(&g_handle);
}

static void __exit pr_device_exit(void)
{
    // TODO: destroy PCIe device.
    // TODO: remove this from here.
    pr_config_exit(&g_handle);
}

module_init(pr_device_init);
module_exit(pr_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("TODO"); // just for documentation

