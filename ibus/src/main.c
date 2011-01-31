/* vim:set et sts=4: */

#include <ibus.h>
#include "engine.h"

static IBusBus *bus = NULL;
static IBusFactory *factory = NULL;

static void
ibus_disconnected_cb (IBusBus  *bus,
                      gpointer  user_data)
{
    ibus_quit ();
}


static void
init (void)
{
    IBusComponent *component;

    ibus_init ();

    bus = ibus_bus_new ();
    g_object_ref_sink (bus);
    g_signal_connect (bus, "disconnected", G_CALLBACK (ibus_disconnected_cb), NULL);
	
    factory = ibus_factory_new (ibus_bus_get_connection (bus));
    g_object_ref_sink (factory);
    ibus_factory_add_engine (factory, "enchant", IBUS_TYPE_ENCHANT_ENGINE);

    ibus_bus_request_name (bus, "org.freedesktop.IBus.Enchant", 0);

    component = ibus_component_new ("org.freedesktop.IBus.Enchant",
                                    "English Writer",
                                    "0.1.0",
                                    "GPL",
                                    "Peng Huang <shawn.p.huang@gmail.com>",
                                    "http://code.google.com/p/ibus/",
                                    "",
                                    "ibus-tmpl");
    ibus_component_add_engine (component,
                               ibus_engine_desc_new ("enchant",
                                                     "English Writer",
                                                     "English Writer",
                                                     "en",
                                                     "GPL",
                                                     "Peng Huang <shawn.p.huang@gmail.com>",
                                                     PKGDATADIR"/icon/ibus-enchant.svg",
                                                     "en"));
    ibus_bus_register_component (bus, component);
}

int main()
{

    init ();
    ibus_main ();
}
