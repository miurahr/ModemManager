/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "mm-plugin.h"

const char *
mm_plugin_get_name (MMPlugin *plugin)
{
    g_return_val_if_fail (MM_IS_PLUGIN (plugin), NULL);

    return MM_PLUGIN_GET_INTERFACE (plugin)->get_name (plugin);
}

guint32
mm_plugin_supports_port (MMPlugin *plugin,
                         const char *subsys,
                         const char *name)
{
    g_return_val_if_fail (MM_IS_PLUGIN (plugin), FALSE);
    g_return_val_if_fail (subsys != NULL, FALSE);
    g_return_val_if_fail (name != NULL, FALSE);

    return MM_PLUGIN_GET_INTERFACE (plugin)->supports_port (plugin, subsys, name);
}

MMModem *
mm_plugin_grab_port (MMPlugin *plugin,
                     const char *subsys,
                     const char *name,
                     GError **error)
{
    g_return_val_if_fail (MM_IS_PLUGIN (plugin), FALSE);
    g_return_val_if_fail (subsys != NULL, FALSE);
    g_return_val_if_fail (name != NULL, FALSE);

    return MM_PLUGIN_GET_INTERFACE (plugin)->grab_port (plugin, subsys, name, error);
}

/*****************************************************************************/

static void
mm_plugin_init (gpointer g_iface)
{
}

GType
mm_plugin_get_type (void)
{
    static GType plugin_type = 0;

    if (!G_UNLIKELY (plugin_type)) {
        const GTypeInfo plugin_info = {
            sizeof (MMPlugin), /* class_size */
            mm_plugin_init,   /* base_init */
            NULL,       /* base_finalize */
            NULL,
            NULL,       /* class_finalize */
            NULL,       /* class_data */
            0,
            0,              /* n_preallocs */
            NULL
        };

        plugin_type = g_type_register_static (G_TYPE_INTERFACE,
                                             "MMPlugin",
                                             &plugin_info, 0);

        g_type_interface_add_prerequisite (plugin_type, G_TYPE_OBJECT);
    }

    return plugin_type;
}
