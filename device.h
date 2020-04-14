/*
 * Copyright 2020 - Patrick Rohr
 */

#pragma once

#include <linux/netdevice.h>
#include <net/cfg80211.h>

struct pr_device_ops {
    void (*load_firmware)(void); /* Loads the firmware */
};

struct pr_device_handle {
    struct wiphy* wiphy;
    struct net_device* netdev;
    struct device_ops* device_ops;
    struct wireless_dev wdev;
};

