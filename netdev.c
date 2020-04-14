/*
 * Copyright 2020 - Patrick Rohr
 */

#include "netdev.h"
#include <linux/netdevice.h>

static void _pr_netdev_configure(struct pr_device_handle *handle);

static netdev_tx_t _cb_start_tx(struct sk_buff *skb, struct net_device *dev);

static const struct net_device_ops pr_netdev_ops = {
	.ndo_open = NULL,
	.ndo_stop = NULL,
	.ndo_start_xmit = _cb_start_tx, // required
	.ndo_set_features = NULL,
	.ndo_set_rx_mode = NULL,
};

int pr_netdev_init(struct pr_device_handle *handle)
{
	handle->netdev = alloc_netdev(sizeof(struct pr_device_handle *),
				      "test%d", NET_NAME_ENUM, ether_setup);

	if (!handle->netdev) {
		printk(KERN_ALERT "Failed to allocate netdev\n");
		goto error_alloc;
	}

	_pr_netdev_configure(handle);
	if (register_netdev(handle->netdev)) {
		printk(KERN_ALERT "Failed to register netdev\n");
		goto error_register;
	}

	return 0;

error_register:
	free_netdev(handle->netdev);
error_alloc:
	return 1;
}

void pr_netdev_exit(struct pr_device_handle *handle)
{
	unregister_netdev(handle->netdev);
	free_netdev(handle->netdev);
}

static void _pr_netdev_configure(struct pr_device_handle *handle)
{
	// init private data
	*((struct pr_device_handle **)netdev_priv(handle->netdev)) = handle;

	struct wireless_dev *wdev = &handle->wdev;
	wdev->wiphy = handle->wiphy;
	wdev->netdev = handle->netdev;

	wdev->iftype = NL80211_IFTYPE_STATION;

	handle->netdev->ieee80211_ptr = wdev;
	handle->netdev->netdev_ops = &pr_netdev_ops;
}

static netdev_tx_t _cb_start_tx(struct sk_buff *skb, struct net_device *dev)
{
	return NETDEV_TX_OK;
}
