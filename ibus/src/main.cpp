/* vim:set et sts=4: */

#include <ibus.h>
#include <stdio.h>
#include <dirent.h>

#include "engine.h"
#include "KeyMagicEngine.h"

static IBusBus *bus = NULL;
static IBusFactory *factory = NULL;

/* options */
static gboolean xml = FALSE;
static gboolean ibus = FALSE;
static gboolean verbose = FALSE;

static const GOptionEntry entries[] =
{
    { "xml", 'x', 0, G_OPTION_ARG_NONE, &xml, "generate xml for engines", NULL },
    { "ibus", 'i', 0, G_OPTION_ARG_NONE, &ibus, "component is executed by ibus", NULL },
    { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "verbose", NULL },
    { NULL },
};

static void
ibus_disconnected_cb (IBusBus  *bus,
                      gpointer  user_data)
{
    ibus_quit ();
}

static IBusEngineDesc *
ibus_keymagic_engine_new (gchar*  lang,
					  gchar*  name,
					  gchar*  title,
					  gchar*  icon,
					  gchar*  desc)
{
    IBusEngineDesc *engine;
    gchar *engine_name;
    gchar *engine_longname;
    gchar *engine_title;
    gchar *engine_icon;
    gchar *engine_desc;


    engine_name = g_strdup_printf ("keymagic:%s", name);

    engine_longname = g_strdup_printf ("%s (keymagic)", name);
    engine_title = title;
    engine_icon = icon;
    engine_desc = desc;

    engine = ibus_engine_desc_new (engine_name,
                                   engine_longname,
                                   engine_desc ? engine_desc : "",
                                   lang,
                                   "GPL",
                                   "",
                                   engine_icon ? engine_icon : "",
                                   "us");
    engine->rank = 0;

    g_free (engine_name);
    g_free (engine_longname);
    //g_free (engine_title);
    //g_free (engine_icon);
    //g_free (engine_desc);

    return engine;
}

GList *
GetKeyboardNames()
{
	GList * names = NULL;
	DIR *dir;
	struct dirent *ent;

	dir = opendir ("/home/thantthetkz/.keymagic");

	if (dir != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			std::string s = ent->d_name;
			if (s == "." || s == ".." || !g_str_has_suffix(ent->d_name, ".km2"))
				continue;
			s = s.substr(0, s.length() - 4);
			gchar * name = g_strdup(s.c_str());
			names = g_list_append(names, name);
		}
		closedir (dir);
	} else {
		/* could not open directory */
		perror ("");
		return NULL;
	}

	return names;
}

IBusComponent *
ibus_keymagic_get_component (void)
{
    GList *engines, *p;
    IBusComponent *component;

    ibus_init ();

    component = ibus_component_new ("org.freedesktop.IBus.KeyMagic",
									"KeyMagic",
									"0.1.0",
									"GPL",
									"Thant Thet Khin Zaw <thantthetkz@gmail.com>",
									"http://code.google.com/p/keymagic/",
									"",
									"ibus-keymagic");

    GList *names = GetKeyboardNames();
    for (p = names; p != NULL; p = p->next) {
    	ibus_component_add_engine (component,
    			ibus_keymagic_engine_new ("en", (gchar*)p->data, (gchar*)p->data,
    					PKGDATADIR"/icons/ibus-keymagic.png", "keymagic"));
    	g_free(p->data);
    }

    return component;
}

static void
init (void)
{
    IBusComponent *component;
    GList *engines, *p;

    ibus_init ();

    bus = ibus_bus_new ();
    g_object_ref_sink (bus);
    g_signal_connect (bus, "disconnected", G_CALLBACK (ibus_disconnected_cb), NULL);

    factory = ibus_factory_new (ibus_bus_get_connection (bus));

    //ibus_factory_add_engine (factory, "keymagic", IBUS_TYPE_KEYMAGIC_ENGINE);

    //ibus_bus_request_name (bus, "org.freedesktop.IBus.KeyMagic", 0);

    /*component = ibus_component_new ("org.freedesktop.IBus.KeyMagic",
                                    "KeyMagic",
                                    "0.1.0",
                                    "GPL",
                                    "Thant Thet Khin Zaw <thantthetkz@gmail.com>",
                                    "http://code.google.com/p/keymagic/",
                                    "",
                                    "ibus-keymagic");*/
    component = ibus_keymagic_get_component();

    /*
    ibus_component_add_engine (component,
                               ibus_engine_desc_new ("keymagic",
                                                     "KeyMagic",
                                                     "KeyMagic",
                                                     "us",
                                                     "GPL",
                                                     "Peng Huang <shawn.p.huang@gmail.com>",
                                                     //PKGDATADIR"/icons/ibus-keymagic.png",
                                                     "/icons/ibus-keymagic.png",
                                                     "us"));
	*/
    engines = ibus_component_get_engines (component);

	for (p = engines; p != NULL; p = p->next) {
		IBusEngineDesc *engine = (IBusEngineDesc *)p->data;
		GType type = ibus_keymagic_engine_get_type_for_name (engine->name);

		if (type == G_TYPE_INVALID) {
			g_debug ("Can not create engine type for %s", engine->name);
			continue;
		}
		ibus_factory_add_engine (factory, engine->name, type);
	}

	if (ibus) {
		ibus_bus_request_name (bus, "org.freedesktop.IBus.KeyMagic", 0);
	}
	else {
		ibus_bus_register_component (bus, component);
	}

	g_object_unref (component);
}

static void
print_engines_xml (void)
{
    IBusComponent *component;
    GString *output;

    component = ibus_keymagic_get_component ();
    output = g_string_new ("");

    /*GList *p, *names = GetKeyboardNames();
    for (p = names; p != NULL; p = p->next) {
    	ibus_component_add_engine (component,
    			ibus_keymagic_engine_new ("en", (char*)p->data, (char*)p->data,
    			"/usr/shared/ibus-keymagic/icon/ibus-keymagic.png", "keymagic"));
    }*/

    ibus_component_output_engines(component, output, 0);

    fprintf (stdout, "%s", output->str);

    g_object_unref (component);

    g_string_free (output, TRUE);

}

int main(gint argc, gchar **argv)
{

    GError *error = NULL;
    GOptionContext *context;

    context = g_option_context_new ("- ibus KeyMagic engine component");

    g_option_context_add_main_entries (context, entries, "ibus-keymagic");

    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("Option parsing failed: %s\n", error->message);
        return -1;
    }

    if (xml) {
        print_engines_xml ();
        return 0;
    }

    init ();
    ibus_main ();
    return 0;
}
