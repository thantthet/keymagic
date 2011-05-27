/* vim:set et sts=4: */

#include <ibus.h>
#include <stdlib.h>
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


    engine_name = g_strdup_printf ("%s", name);

    engine_longname = g_strdup_printf ("%s (KeyMagic)", title);
    engine_title = g_strdup(title);
    engine_icon = g_strdup(icon);
    engine_desc = g_strdup(desc);

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
    g_free (engine_title);
    g_free (engine_icon);
    g_free (engine_desc);

    return engine;
}

GList *
keymagic_get_keyboard_list(gchar * path)
{
	GList * names = NULL;
	DIR *dir;
	struct dirent *ent;

	dir = opendir (path);

	if (dir != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			gchar * filename = ent->d_name;
			if (g_str_equal(filename, ".") || g_str_equal(filename, "..") || !g_str_has_suffix(filename, ".km2"))
				continue;
			names = g_list_append(names, g_strdup_printf("%s/%s", path, filename));
		}
		closedir (dir);
	} else {
		/* could not open directory */
		perror ("");
		return NULL;
	}

	return names;
}

gchar * getKeyboardNameOrTitle (const InfoList& infos, const gchar * fileName)
{
	gchar * keyboardName;
	if (infos.find('name') != infos.end()) {
		Info name = infos.find('name')->second;
		keyboardName = g_strndup(name.data, name.size);
	} else {
		keyboardName = g_strdup(basename(fileName));
		keyboardName[strlen(keyboardName) - 4] = '\0';
	}
	return keyboardName;
}

bool ExtractIcon(const InfoList& infos, const gchar * iconFile)
{
	if (infos.find('icon') != infos.end()) {
		Info icon = infos.find('icon')->second;
		FILE * f = fopen(iconFile, "wb");
		if (f == 0) {
			return false;
		}
		fwrite(icon.data, icon.size, 1, f);
		fclose(f);
		return true;
	}

	return false;
}

gchar * getDescription(const InfoList& infos)
{
	gchar * description;
	if (infos.find('desc') != infos.end()) {
		Info desc = infos.find('desc')->second;
		description = g_strndup(desc.data, desc.size);
	} else {
		description = g_strdup("");
	}
	return description;
}

//#define PKGDATADIR ""

gchar * dirname (const char * dir)
{
	gchar * dir_copy = g_strdup(dir);
	const char * fileName = basename(dir);
	dir_copy[strlen(dir_copy) - strlen(fileName)] = '\0';

	return dir_copy;
}

GList *
ibus_keymagic_add_engines(GList * engines, GList * keyboard_list)
{
    GList *p;
    for (p=keyboard_list; p != NULL; p = p->next) {
    	gchar * filename = (gchar *) p->data;
    	InfoList * infos = KeyMagicKeyboard::getInfosFromKeyboardFile(filename);

    	gchar * keyboardName = getKeyboardNameOrTitle(*infos, filename);
    	gchar * desc = getDescription(*infos);
    	gchar * kbDir = dirname(filename);

    	gchar * iconFile = g_strdup_printf("%s/%s.icon", kbDir, keyboardName);

        if (ExtractIcon(*infos, iconFile) == false) {
        	iconFile = g_strdup(PKGDATADIR"/icons/ibus-keymagic.png");
        }

        engines = g_list_append (engines,
        		ibus_keymagic_engine_new ("en", filename, keyboardName,
				iconFile, desc));

        g_free(kbDir);
        g_free(iconFile);
        g_free(desc);
        g_free(keyboardName);
        g_free(p->data);
    }
    return engines;
}

GList *
ibus_keymagic_list_engines (void)
{
    GList *engines = NULL;
    GList *keyboard_list;
    gchar *local_keyboard_path;

    keyboard_list = keymagic_get_keyboard_list("/usr/share/ibus-keymagic");

    engines = ibus_keymagic_add_engines(engines, keyboard_list);

    g_list_free(keyboard_list);

    local_keyboard_path= g_strdup_printf("%s/.keymagic", getenv("HOME"));

    keyboard_list = keymagic_get_keyboard_list(local_keyboard_path);

    engines = ibus_keymagic_add_engines(engines, keyboard_list);

    g_free(local_keyboard_path);

    g_list_free(keyboard_list);

    return engines;
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

    engines = ibus_keymagic_list_engines ();

    for (p = engines; p != NULL; p = p->next) {
		ibus_component_add_engine (component, (IBusEngineDesc *) p->data);
	}

	g_list_free (engines);

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
                                                     PKGDATADIR"/icons/ibus-keymagic.png",
                                                     "us"));
	*/
    engines = ibus_component_get_engines (component);

	for (p = engines; p != NULL; p = p->next) {
		IBusEngineDesc *engine = (IBusEngineDesc *)p->data;
		ibus_factory_add_engine (factory, engine->name, IBUS_TYPE_KEYMAGIC_ENGINE);
	}

	if (ibus) {
		ibus_bus_request_name (bus, "org.freedesktop.IBus.KeyMagic", 0);
	}
	else {
		bool success = ibus_bus_register_component (bus, component);
		g_assert(success != false);
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
