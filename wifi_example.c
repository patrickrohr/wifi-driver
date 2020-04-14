/* 
 * Copyright 2020 - Patrick Rohr
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <net/cfg80211.h>

// Data Structure Declarations
struct wl_handle {
	struct wiphy *wiphy;
	struct net_device *netdev;
};

struct netdev_priv_data {
	struct wireless_dev wireless_dev;
};

// Function Declarations
static int _cb_add_key(struct wiphy *wiphy, struct net_device *netdev,
		       u8 key_index, bool pairwise, const u8 *mac_addr,
		       struct key_params *params);

static int _cb_get_key(struct wiphy *wiphy, struct net_device *netdev,
		       u8 key_index, bool pairwise, const u8 *mac_addr,
		       void *cookie,
		       void (*callback)(void *cookie, struct key_params *));

static int _cb_set_default_key(struct wiphy *wiphy, struct net_device *netdev,
			       u8 key_index, bool unicast, bool multicast);

static int _cb_del_key(struct wiphy *wiphy, struct net_device *netdev,
		       u8 key_index, bool pairwise, const u8 *mac_addr);

static int _cb_get_station(struct wiphy *wiphy, struct net_device *netdev,
			   const u8 *mac, struct station_info *sinfo);

static int _cb_dump_station(struct wiphy *wiphy, struct net_device *netdev,
			    int idx, u8 *mac, struct station_info *sinfo);

static int _cb_scan(struct wiphy *wiphy, struct cfg80211_scan_request *request);

static int _cb_connect(struct wiphy *wiphy, struct net_device *netdev,
		       struct cfg80211_connect_params *sme);

static int _cb_disconnect(struct wiphy *wiphy, struct net_device *netdev,
			  u16 reason_code);

static int _cb_set_power_mgmt(struct wiphy *wiphy, struct net_device *netdev,
			      bool enabled, int timeout);

static void _cb_mgmt_frame_register(struct wiphy *wiphy,
				    struct wireless_dev *wdev, u16 frame_type,
				    bool reg);

static void wl_wiphy_init(struct wiphy *wiphy);

static void wl_netdev_init(struct net_device *netdev);

static netdev_tx_t wl_start_tx(struct sk_buff *skb, struct net_device *dev);

static struct netdev_priv_data *netdev_get_priv(struct net_device *netdev);

// Definitions
#define DRIVER_AUTHOR "Patrick Rohr <patrickrohr92@gmail.com>"
#define DRIVER_DESC "A sample WiFi driver"

static struct wl_handle g_handle = {};

// Data Structure Definitions
static const struct net_device_ops wl_netdev_ops = {
	.ndo_open = NULL,
	.ndo_stop = NULL,
	.ndo_start_xmit = wl_start_tx, // required
	.ndo_set_features = NULL,
	.ndo_set_rx_mode = NULL,
};

// Function Definitions
static int _cb_add_key(struct wiphy *wiphy, struct net_device *netdev,
		       u8 key_index, bool pairwise, const u8 *mac_addr,
		       struct key_params *params)
{
	printk(KERN_INFO "_cb_add_key called\n");
	return 0;
}

static int _cb_get_key(struct wiphy *wiphy, struct net_device *netdev,
		       u8 key_index, bool pairwise, const u8 *mac_addr,
		       void *cookie,
		       void (*callback)(void *cookie, struct key_params *))
{
	printk(KERN_INFO "_cb_get_key called\n");
	return 0;
}

static int _cb_del_key(struct wiphy *wiphy, struct net_device *netdev,
		       u8 key_index, bool pairwise, const u8 *mac_addr)
{
	printk(KERN_INFO "_cb_del_key called\n");
	return 0;
}

static int _cb_set_default_key(struct wiphy *wiphy, struct net_device *netdev,
			       u8 key_index, bool unicast, bool multicast)
{
	printk(KERN_INFO "_cb_set_default_key called\n");
	return 0;
}

static int _cb_get_station(struct wiphy *wiphy, struct net_device *netdev,
			   const u8 *mac, struct station_info *sinfo)
{
	printk(KERN_INFO "_cb_get_station called\n");
	return 0;
}

static int _cb_dump_station(struct wiphy *wiphy, struct net_device *netdev,
			    int idx, u8 *mac, struct station_info *sinfo)
{
	printk(KERN_INFO "_cb_dump_station called\n");
	return 0;
}

static int _cb_scan(struct wiphy *wiphy, struct cfg80211_scan_request *request)
{
	printk(KERN_INFO "_cb_scan called\n");
	return 0;
}

static int _cb_connect(struct wiphy *wiphy, struct net_device *netdev,
		       struct cfg80211_connect_params *sme)
{
	printk(KERN_INFO "_cb_connect called\n");
	return 0;
}

static int _cb_disconnect(struct wiphy *wiphy, struct net_device *netdev,
			  u16 reason_code)
{
	printk(KERN_INFO "_cb_disconnect called\n");
	return 0;
}

static int _cb_set_power_mgmt(struct wiphy *wiphy, struct net_device *netdev,
			      bool enabled, int timeout)
{
	printk(KERN_INFO "set_power mgmt called\n");
	return 0;
}

static void _cb_mgmt_frame_register(struct wiphy *wiphy,
				    struct wireless_dev *wdev, u16 frame_type,
				    bool reg)
{
	printk(KERN_INFO "mgmt frame register called\n");
}

static netdev_tx_t wl_start_tx(struct sk_buff *skb, struct net_device *dev)
{
	// TODO: implement
	printk(KERN_ALERT "Unimplemented wl_start_tx called\n");
	return NETDEV_TX_OK; // or NETDEV_TX_BUSY
}

// TODO: probably shouldn't be executed inside __init, but in some kind of
// probe function.
// TODO: refactor
// At this point we know we have a netdevice, that contains a wdev structure
// and a wiphy structure. So I should be able to create something like a
// netdev_helper / netdev_factory, a wdev_helper, and a wiphy_helper.
//
// Looking at ath6kl:
// - netdev is initialized and registered inside iface_add inside some
// cfg80211 callback.
// - wiphy is created inside the cfg80211 create function which is called from
// core create, which in turn is called from a sdio / usb probe. core create also
// receives the device.
//
// - there is also a core init function, which seems to turn on the device
// (ath6kl_hif_power_on?) and load the firmware.
//
// usb.c and sdio.c contain function related to interacting with device.
//
// Looks like all of this starts inside sdio.c / usb.c where the sdio driver
// is registered (inside an __init).
//
// My intel card is connected via PCIe. Looks like that could be a tiny bit more
// complex. :)
static int __init wl_init(void)
{
	printk(KERN_INFO "wifi init called\n");

	// for now, we are not using private data inside the wiphy struct.
	g_handle.wiphy = wiphy_new(&g_config_ops, 0);
	if (!g_handle.wiphy) {
		printk(KERN_ALERT "Could not allocate wiphy device\n");
		goto error;
	}

	wl_wiphy_init(g_handle.wiphy);

	// at this point, we should be ready to register the wiphy
	if (wiphy_register(g_handle.wiphy)) {
		printk(KERN_ALERT "Could not register wiphy\n");
		goto error_wiphy_register;
	}

	// wiphy is registered! Yay! On to allocating a net_device.
	// TODO: use #defines here
	g_handle.netdev = alloc_netdev(sizeof(struct netdev_priv_data),
				       "test%d", NET_NAME_ENUM, ether_setup);
	if (g_handle.netdev == NULL) {
		printk(KERN_ALERT "Could not allocate netdev\n");
		goto error_netdev_alloc;
	}

	wl_netdev_init(g_handle.netdev);
	if (register_netdev(g_handle.netdev)) {
		printk(KERN_ALERT "Could not register netdev\n");
		goto error_netdev_register;
	}
	// SUCCESS! Should be good to go.

	printk(KERN_INFO "YAY! we made it here\n");
	return 0;

error_netdev_register:
	free_netdev(g_handle.netdev);
error_netdev_alloc:
	wiphy_unregister(g_handle.wiphy);
error_wiphy_register:
	wiphy_free(g_handle.wiphy);
error:
	// TODO: return actual error code
	return 1;
}

static void __exit wl_exit(void)
{
	printk(KERN_INFO "wl_exit called\n");

	// TODO: currently there is still a warning when exiting regarding an existing scan request.
	unregister_netdev(g_handle.netdev);
	free_netdev(g_handle.netdev);
	wiphy_unregister(g_handle.wiphy);
	wiphy_free(g_handle.wiphy);
}

module_init(wl_init);
module_exit(wl_exit);

/* For some reason this tends to be put right after module_init and exit */
MODULE_LICENSE("GPL");

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("TODO"); // just for documentation

/**
 * This function configures the wiphy structure.
 * Currently, this is kept to an absolute minimum
 */
static void wl_wiphy_init(struct wiphy *wiphy)
{
	// TODO: apparently gotta set the device object as the parent
	// set_wiphy_dev(wiphy, dev);

	// For this exercise, supporting station mode will do.
	wiphy->interface_modes = BIT(NL80211_IFTYPE_STATION);

	// 2GHz only for now.
	wiphy->bands[NL80211_BAND_2GHZ] = &wl_band_2ghz;

	wiphy->max_scan_ssids = 42;
}

static void wl_netdev_init(struct net_device *netdev)
{
	struct netdev_priv_data *priv_data = netdev_get_priv(netdev);

	// initialize private data.(wireless_dev)
	struct wireless_dev *wdev = &priv_data->wireless_dev;
	wdev->wiphy = g_handle.wiphy;
	wdev->netdev = netdev;
	// TODO: what's the difference between wdev.iftype and wiphy.interface_modes?
	wdev->iftype = NL80211_IFTYPE_STATION;

	// finally, set ieee802111_ptr
	netdev->ieee80211_ptr = wdev;

	// at this point we would have to set the device object for the net device.
	// SET_NETDEV_DEV(g_handle.netdev, wiphy_dev(g_handle.wiphy));

	// Set netdev operations
	netdev->netdev_ops = &wl_netdev_ops;

	// TODO: proper net_device initialization.
	// What is missing?

	// net device should show up under ip a
}

static struct netdev_priv_data *netdev_get_priv(struct net_device *netdev)
{
	return (struct netdev_priv_data *)netdev_priv(netdev);
}
