/*
 * Copyright 2020 - Patrick Rohr
 */

#pragma once

#include <linux/netdevice.h>
#include <net/cfg80211.h>

struct pr_device_handle {
    struct wiphy* wiphy;
    struct net_device* netdev;
    struct wireless_dev wdev;
};

