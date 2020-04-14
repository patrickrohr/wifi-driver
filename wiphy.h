/*
 * Copyright 2020 - Patrick Rohr
 */

#pragma once

#include "device.h"

int pr_wiphy_init(struct pr_device_handle *handle);

void pr_wiphy_exit(struct pr_device_handle *handle);
