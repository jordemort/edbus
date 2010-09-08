#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <E_Ukit.h>
#include "e_ukit_private.h"

#define E_UKIT_BUS E_UDISKS_BUS
#define E_UKIT_PATH E_UDISKS_PATH
#define E_UKIT_INTERFACE E_UDISKS_INTERFACE

const char *e_udisks_iface="org.freedesktop.UDisks.Device";

#if 0
static void cb_device_get_property(void *data, DBusMessage *msg, DBusError *err);
static void cb_device_get_all_properties(void *data, DBusMessage *msg, DBusError *err);
static void cb_device_query_capability(void *data, DBusMessage *msg, DBusError *err);
#endif

/* Properties.Get */
EAPI DBusPendingCall *
e_udisks_get_property(E_DBus_Connection *conn, const char *udi, const char *property, E_DBus_Callback_Func cb_func, void *data)
{
   DBusMessage *msg;
   DBusPendingCall *ret;

   msg = e_ukit_property_call_new(udi, "Get");
   dbus_message_append_args(msg, DBUS_TYPE_STRING, &e_udisks_iface, DBUS_TYPE_STRING, &property, DBUS_TYPE_INVALID);
   ret = e_dbus_method_call_send(conn, msg, unmarshal_property, cb_func, free_property, -1, data);
   dbus_message_unref(msg);
   return ret;
}

/* Properties.GetAll */
EAPI DBusPendingCall *
e_udisks_get_all_properties(E_DBus_Connection *conn, const char *udi, E_DBus_Callback_Func cb_func, void *data)
{
   DBusMessage *msg;
   DBusPendingCall *ret;

   msg = e_ukit_property_call_new(udi, "GetAll");
   dbus_message_append_args(msg, DBUS_TYPE_STRING, &e_udisks_iface, DBUS_TYPE_INVALID);
   ret = e_dbus_method_call_send(conn, msg, unmarshal_device_get_all_properties, cb_func, free_device_get_all_properties, -1, data);
   dbus_message_unref(msg);
   return ret;
}



/* void FilesystemMount(string fstype, array{string}options) */

/**
 * @brief Mount a Filesystem
 *
 * @param conn the E_DBus_Connection
 * @param udi the udi of the device object
 * @param fstype the fstype of the device (e.g. volume.fstype property)
 * @param options a list of additional options (not sure... fstype dependent?)
 * @param cb_func an optional callback to call when the mount is done
 * @param data custom data pointer for the callback function
 *
 * @return mount point of fs or error
 */
EAPI DBusPendingCall *
e_udisks_volume_mount(E_DBus_Connection *conn, const char *udi, const char *fstype, Eina_List *options, E_DBus_Callback_Func cb_func, void *data)
{
   DBusMessage *msg;
   DBusMessageIter iter, subiter;
   Eina_List *l;
   DBusPendingCall *ret;

   msg = e_ukit_device_call_new(udi, "FilesystemMount");

   dbus_message_iter_init_append(msg, &iter);
   dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &fstype);
   dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING, &subiter);

   if (options)
     {
        const char *opt;

        EINA_LIST_FOREACH(options, l, opt)
          dbus_message_iter_append_basic(&subiter, DBUS_TYPE_STRING, &opt);
     }
   dbus_message_iter_close_container(&iter, &subiter) ;

   ret = e_dbus_method_call_send(conn, msg, unmarshal_property, cb_func, free_property, -1, data);
   dbus_message_unref(msg);
   return ret;
}

/* void Unmount(array{string} options) */

/**
 * @brief Unmount a Volume
 *
 * @param conn the E_DBus_Connection
 * @param udi the udi of the device object
 * @param options a list of additional options (currently only 'force' is supported)
 * @param cb_func an optional callback to call when the unmount is done
 * @param data custom data pointer for the callback function
 */
EAPI DBusPendingCall *
e_udisks_volume_unmount(E_DBus_Connection *conn, const char *udi, Eina_List *options, E_DBus_Callback_Func cb_func, void *data)
{
   DBusMessage *msg;
   DBusMessageIter iter, subiter;
   Eina_List *l;
   DBusPendingCall *ret;

   msg = e_ukit_device_call_new(udi, "FilesystemUnmount");

   dbus_message_iter_init_append(msg, &iter);
   dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING, &subiter);
   if (options)
     {
        const char *opt;

        EINA_LIST_FOREACH(options, l, opt)
          dbus_message_iter_append_basic(&subiter, DBUS_TYPE_STRING, &opt);
     }
   dbus_message_iter_close_container(&iter, &subiter) ;

   ret = e_dbus_method_call_send(conn, msg, NULL, cb_func, NULL, -1, data);
   dbus_message_unref(msg);
   return ret;
}

/**
 * @brief Eject a Volume
 *
 * @param conn the E_DBus_Connection
 * @param udi the udi of the device object
 * @param options a list of additional options (none currently supported)
 * @param cb_func an optional callback to call when the eject is done
 * @param data cuatom data pointer for the callback function
 */
EAPI DBusPendingCall *
e_udisks_volume_eject(E_DBus_Connection *conn, const char *udi, Eina_List *options, E_DBus_Callback_Func cb_func, void *data)
{
   DBusMessage *msg;
   DBusMessageIter iter, subiter;
   Eina_List *l;
   DBusPendingCall *ret;

   msg = e_ukit_device_call_new(udi, "DriveEject");

   dbus_message_iter_init_append(msg, &iter);
   dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING, &subiter);
   if (options)
     {
        const char *opt;

        EINA_LIST_FOREACH(options, l, opt)
          dbus_message_iter_append_basic(&subiter, DBUS_TYPE_STRING, &opt);
     }
   dbus_message_iter_close_container(&iter, &subiter) ;

   ret = e_dbus_method_call_send(conn, msg, NULL, cb_func, NULL, -1, data);
   dbus_message_unref(msg);
   return ret;
}

/* EnumerateDevices */
EAPI DBusPendingCall *
e_udisks_get_all_devices(E_DBus_Connection *conn, E_DBus_Callback_Func cb_func, void *data)
{
   DBusMessage *msg;
   DBusPendingCall *ret;

   msg = e_ukit_call_new(E_UKIT_PATH, "EnumerateDevices");
   ret = e_dbus_method_call_send(conn, msg, unmarshal_string_list, cb_func, free_string_list, -1, data);
   dbus_message_unref(msg);
   return ret;
}


/* Manager.FindDeviceByCapability */
/*
 * not implemented in udisks yet...
 * 
EAPI DBusPendingCall *
e_udisks_find_device_by_capability(E_DBus_Connection *conn, const char *capability, E_DBus_Callback_Func cb_func, void *data)
{
   DBusMessage *msg;
   DBusPendingCall *ret;

   msg = e_udisks_call_new("FindDeviceByCapability");
   dbus_message_append_args(msg, DBUS_TYPE_STRING, &capability, DBUS_TYPE_INVALID);
   ret = e_dbus_method_call_send(conn, msg, unmarshal_string_list, cb_func, free_string_list, -1, data);
   dbus_message_unref(msg);
   return ret;
}
*/
