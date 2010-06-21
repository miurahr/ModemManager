/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details:
 *
 * Copyright (C) 2008 - 2009 Novell, Inc.
 * Copyright (C) 2009 - 2010 Red Hat, Inc.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define G_UDEV_API_IS_SUBJECT_TO_CHANGE
#include <gudev/gudev.h>

#include "mm-modem-anydata-gsm.h"
#include "mm-modem-gsm-network.h"
#include "mm-errors.h"
#include "mm-callback-info.h"
#include "mm-at-serial-port.h"
#include "mm-serial-parsers.h"

static void modem_init (MMModem *modem_class);
static void modem_gsm_network_init (MMModemGsmNetwork *gsm_network_class);

G_DEFINE_TYPE_EXTENDED (MMModemAnydataGsm, mm_modem_anydata_gsm, MM_TYPE_GENERIC_GSM, 0,
                        G_IMPLEMENT_INTERFACE (MM_TYPE_MODEM, modem_init)
                        G_IMPLEMENT_INTERFACE (MM_TYPE_MODEM_GSM_NETWORK, modem_gsm_network_init))


MMModem *
mm_modem_anydata_gsm_new (const char *device,
                         const char *driver,
                         const char *plugin)
{
    g_return_val_if_fail (device != NULL, NULL);
    g_return_val_if_fail (driver != NULL, NULL);
    g_return_val_if_fail (plugin != NULL, NULL);

    return MM_MODEM (g_object_new (MM_TYPE_MODEM_ANYDATA_GSM,
                                   MM_MODEM_MASTER_DEVICE, device,
                                   MM_MODEM_DRIVER, driver,
                                   MM_MODEM_PLUGIN, plugin,
                                   NULL));
}

/*****************************************************************************/

static gboolean
grab_port (MMModem *modem,
           const char *subsys,
           const char *name,
           MMPortType suggested_type,
           gpointer user_data,
           GError **error)
{
    MMGenericGsm *gsm = MM_GENERIC_GSM (modem);
    MMPortType ptype = MM_PORT_TYPE_IGNORED;
    const char *sys[] = { "tty", NULL };
    GUdevClient *client;
    GUdevDevice *device = NULL;
    MMPort *port = NULL;
    int usbif;

    client = g_udev_client_new (sys);
    if (!client) {
        g_set_error (error, 0, 0, "Could not get udev client.");
        return FALSE;
    }

    device = g_udev_client_query_by_subsystem_and_name (client, subsys, name);
    if (!device) {
        g_set_error (error, 0, 0, "Could not get udev device.");
        goto out;
    }

    usbif = g_udev_device_get_property_as_int (device, "ID_USB_INTERFACE_NUM");
    if (usbif < 0) {
        g_set_error (error, 0, 0, "Could not get USB device interface number.");
        goto out;
    }

    if (usbif == 0) {
        if (!mm_generic_gsm_get_at_port (gsm, MM_PORT_TYPE_PRIMARY))
            ptype = MM_PORT_TYPE_PRIMARY;
    } else if (suggested_type == MM_PORT_TYPE_SECONDARY) {
        if (!mm_generic_gsm_get_at_port (gsm, MM_PORT_TYPE_SECONDARY))
            ptype = MM_PORT_TYPE_SECONDARY;
    }

    port = mm_generic_gsm_grab_port (gsm, subsys, name, ptype, error);

    if (port && MM_IS_AT_SERIAL_PORT (port)) {
        g_object_set (G_OBJECT (port), MM_PORT_CARRIER_DETECT, FALSE, NULL);
    } 
out:
    if (device)
        g_object_unref (device);
    g_object_unref (client);
    return !!port;
}

/*****************************************************************************/

static void
modem_init (MMModem *modem_class)
{
    modem_class->grab_port = grab_port;
}

static void
modem_gsm_network_init (MMModemGsmNetwork *class)
{
}

static void
mm_modem_anydata_gsm_init (MMModemAnydataGsm *self)
{
}

static void
mm_modem_anydata_gsm_class_init (MMModemAnydataGsmClass *klass)
{
} 
