/*
 * Copyright 2020 - Patrick Rohr
 */

#include "config.h"
#include "wiphy.h"
#include "netdev.h"


int pr_config_init(struct pr_device_handle* handle)
{
    // TODO: Figure out what to do with error codes... and where to best log.
    if (pr_wiphy_init(handle)) {
        return 1;
    }

    if (pr_netdev_init(handle)) {
        return 1;
    }

    // If an error is returned here, caller will call destroy.
    return 0;
} 

void pr_config_exit(struct pr_device_handle* handle)
{
    // The respective classes should keep track of initialization failures
    // and are supposed to know what needs to be freed and unregistered.
    pr_netdev_exit(handle);
    pr_wiphy_exit(handle);
}
