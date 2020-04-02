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
static int wl_cfg_add_key(struct wiphy *wiphy, struct net_device *netdev,
			  u8 key_index, bool pairwise, const u8 *mac_addr,
			  struct key_params *params);

static int wl_cfg_get_key(struct wiphy *wiphy, struct net_device *netdev,
			  u8 key_index, bool pairwise, const u8 *mac_addr,
			  void *cookie,
			  void (*callback)(void *cookie, struct key_params *));

static int wl_cfg_set_default_key(struct wiphy *wiphy,
				  struct net_device *netdev, u8 key_index,
				  bool unicast, bool multicast);

static int wl_cfg_del_key(struct wiphy *wiphy, struct net_device *netdev,
			  u8 key_index, bool pairwise, const u8 *mac_addr);

static int wl_cfg_get_station(struct wiphy *wiphy, struct net_device *netdev,
			      const u8 *mac, struct station_info *sinfo);

static int wl_cfg_dump_station(struct wiphy *wiphy, struct net_device *netdev,
			       int idx, u8 *mac, struct station_info *sinfo);

static int wl_cfg_scan(struct wiphy *wiphy,
		       struct cfg80211_scan_request *request);

static int wl_cfg_connect(struct wiphy *wiphy, struct net_device *netdev,
			  struct cfg80211_connect_params *sme);

static int wl_cfg_disconnect(struct wiphy *wiphy, struct net_device *netdev,
			     u16 reason_code);

static int wl_cfg_set_power_mgmt(struct wiphy *wiphy, struct net_device *netdev,
				 bool enabled, int timeout);

static void wl_cfg_mgmt_frame_register(struct wiphy *wiphy,
				       struct wireless_dev *wdev,
				       u16 frame_type, bool reg);

static void wl_wiphy_init(struct wiphy *wiphy);

static void wl_netdev_init(struct net_device *netdev);

static netdev_tx_t wl_start_tx(struct sk_buff *skb, struct net_device *dev);

static struct netdev_priv_data *netdev_get_priv(struct net_device *netdev);

// Definitions
#define DRIVER_AUTHOR "Patrick Rohr <patrickrohr92@gmail.com>"
#define DRIVER_DESC "A sample WiFi driver"

static struct wl_handle g_handle = {};

// Data Structure Definitions
static struct cfg80211_ops g_config_ops = {
	.add_key = wl_cfg_add_key,
	.get_key = wl_cfg_get_key,
	.del_key = wl_cfg_del_key,
	.set_default_key = wl_cfg_set_default_key,
	.get_station = wl_cfg_get_station,
	.dump_station = wl_cfg_dump_station,
	.scan = wl_cfg_scan,
	.connect = wl_cfg_connect,
	.disconnect = wl_cfg_disconnect,
	.set_power_mgmt = wl_cfg_set_power_mgmt,
	.mgmt_frame_register = wl_cfg_mgmt_frame_register,
};

static struct ieee80211_channel wl_channels_2ghz[] = { {
	.band = NL80211_BAND_2GHZ,
	.hw_value = 6,
	.center_freq = 2437,
} };

/* Array of supported channel bitrates. Minimum requirements for 2GHz. */
/* clang-format off */
static struct ieee80211_rate wl_bitrates_2ghz[] = {
        {
                .bitrate = 10,
                .hw_value = 0x1,
        },
        {
                .bitrate = 20,
                .hw_value = 0x2,
        },
        {
                .bitrate = 55,
                .hw_value = 0x4,
        },
        {
                .bitrate = 110,
                .hw_value = 0x8,
        }
};
/* clang-format on */

static struct ieee80211_supported_band wl_band_2ghz = {
	.ht_cap.cap = IEEE80211_HT_CAP_SGI_20, /* 20 width band */
	.ht_cap.ht_supported = false,

	.channels = wl_channels_2ghz,
	.n_channels = ARRAY_SIZE(wl_channels_2ghz),

	.bitrates = wl_bitrates_2ghz,
	.n_bitrates = ARRAY_SIZE(wl_bitrates_2ghz),
};

static const struct net_device_ops wl_netdev_ops = {
	.ndo_open = NULL,
	.ndo_stop = NULL,
	.ndo_start_xmit = wl_start_tx, // required
	.ndo_set_features = NULL,
	.ndo_set_rx_mode = NULL,
};

// Function Definitions
static int wl_cfg_add_key(struct wiphy *wiphy, struct net_device *netdev,
			  u8 key_index, bool pairwise, const u8 *mac_addr,
			  struct key_params *params)
{
	return 0;
}

static int wl_cfg_get_key(struct wiphy *wiphy, struct net_device *netdev,
			  u8 key_index, bool pairwise, const u8 *mac_addr,
			  void *cookie,
			  void (*callback)(void *cookie, struct key_params *))
{
	return 0;
}

static int wl_cfg_del_key(struct wiphy *wiphy, struct net_device *netdev,
			  u8 key_index, bool pairwise, const u8 *mac_addr)
{
	return 0;
}

static int wl_cfg_set_default_key(struct wiphy *wiphy,
				  struct net_device *netdev, u8 key_index,
				  bool unicast, bool multicast)
{
	return 0;
}

static int wl_cfg_get_station(struct wiphy *wiphy, struct net_device *netdev,
			      const u8 *mac, struct station_info *sinfo)
{
	return 0;
}

static int wl_cfg_dump_station(struct wiphy *wiphy, struct net_device *netdev,
			       int idx, u8 *mac, struct station_info *sinfo)
{
	return 0;
}

static int wl_cfg_scan(struct wiphy *wiphy,
		       struct cfg80211_scan_request *request)
{
	return 0;
}

static int wl_cfg_connect(struct wiphy *wiphy, struct net_device *netdev,
			  struct cfg80211_connect_params *sme)
{
	return 0;
}

static int wl_cfg_disconnect(struct wiphy *wiphy, struct net_device *netdev,
			     u16 reason_code)
{
	return 0;
}

static int wl_cfg_set_power_mgmt(struct wiphy *wiphy, struct net_device *netdev,
				 bool enabled, int timeout)
{
	return 0;
}

static void wl_cfg_mgmt_frame_register(struct wiphy *wiphy,
				       struct wireless_dev *wdev,
				       u16 frame_type, bool reg)
{
}

static netdev_tx_t wl_start_tx(struct sk_buff *skb, struct net_device *dev)
{
	// TODO: implement
	printk(KERN_ALERT "Unimplemented wl_start_tx called\n");
	return NETDEV_TX_OK; // or NETDEV_TX_BUSY
}

// TODO: probably shouldn't be executed inside __init, but in some kind of probe function.
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
