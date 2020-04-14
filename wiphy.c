/*
 * Copyright 2020 - Patrick Rohr
 */

#include "wiphy.h"
#include <net/cfg80211.h>

static void _pr_wiphy_configure(struct wiphy *wiphy);

// cfg80211 callback functions
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

static struct cfg80211_ops g_config_ops = {
	.add_key = _cb_add_key,
	.get_key = _cb_get_key,
	.del_key = _cb_del_key,
	.set_default_key = _cb_set_default_key,
	.get_station = _cb_get_station,
	.dump_station = _cb_dump_station,
	.scan = _cb_scan,
	.connect = _cb_connect,
	.disconnect = _cb_disconnect,
	.set_power_mgmt = _cb_set_power_mgmt,
	.mgmt_frame_register = _cb_mgmt_frame_register,
};

// Should probably double check all this before actually using a real device.
// *Just* to make sure I am not breaking the law.
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

int pr_wiphy_init(struct pr_device_handle *handle)
{
	unsigned priv_size = 0;
	handle->wiphy = wiphy_new(&g_config_ops, priv_size);

	if (!handle->wiphy) {
		printk(KERN_ALERT "Could not allocate wiphy device\n");
		goto error_alloc;
	}

	_pr_wiphy_configure(handle->wiphy);

	if (wiphy_register(handle->wiphy)) {
		printk(KERN_ALERT "Could not register wiphy\n");
		goto error_register;
	}
	// success
	return 0;

error_register:
	wiphy_free(handle->wiphy);
error_alloc:
	return 1;
}

void pr_wiphy_exit(struct pr_device_handle *handle)
{
	wiphy_unregister(handle->wiphy);
	wiphy_free(handle->wiphy);
}

static void _pr_wiphy_configure(struct wiphy *wiphy)
{
	wiphy->interface_modes = BIT(NL80211_IFTYPE_STATION);
	wiphy->bands[NL80211_BAND_2GHZ] = &wl_band_2ghz;
	// Finding the answer to the world the universe and everything
	// one SSID at a time.
	wiphy->max_scan_ssids = 42;
}

// cfg80211 callbacks
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
