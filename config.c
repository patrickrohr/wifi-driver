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
        goto error;
    }

    if (pr_netdev_init(handle)) {
        goto error;
    }

    // If an error is returned here, caller will call destroy.
    return 0;

error:
    return 1;
} 

void pr_config_exit(struct pr_device_handle* handle)
{
    pr_netdev_exit(handle);
    pr_wiphy_exit(handle);
}
