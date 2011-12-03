/* vim:set et sts=4: */

#include "engine.h"
#include <errno.h>
#include <stdio.h>
#include <iostream>

typedef struct _IBusKeymagicEngine IBusKeymagicEngine;
typedef struct _IBusKeymagicEngineClass IBusKeymagicEngineClass;

struct _IBusKeymagicEngine {
	IBusEngine parent;
	KeyMagicEngine * kme;
};

struct _IBusKeymagicEngineClass {
	IBusEngineClass parent;
	gchar * path;
};

/* functions prototype */
static void	ibus_keymagic_engine_class_init	(IBusKeymagicEngineClass	*klass);
static void	ibus_keymagic_engine_init		(IBusKeymagicEngine		*engine);
static void	ibus_keymagic_engine_destroy		(IBusKeymagicEngine		*engine);
static gboolean 
			ibus_keymagic_engine_process_key_event
                                            (IBusEngine             *engine,
                                             guint               	 keyval,
                                             guint               	 keycode,
                                             guint               	 modifiers);
static void ibus_keymagic_engine_focus_in    (IBusEngine             *engine);
static void ibus_keymagic_engine_focus_out   (IBusEngine             *engine);
static void ibus_keymagic_engine_reset       (IBusEngine             *engine);
static void ibus_keymagic_engine_enable      (IBusEngine             *engine);
static void ibus_keymagic_engine_disable     (IBusEngine             *engine);
/*
static void ibus_engine_set_cursor_location (IBusEngine             *engine,
                                             gint                    x,
                                             gint                    y,
                                             gint                    w,
                                             gint                    h);
static void ibus_keymagic_engine_set_capabilities
                                            (IBusEngine             *engine,
                                             guint                   caps);
static void ibus_keymagic_engine_page_up     (IBusEngine             *engine);
static void ibus_keymagic_engine_page_down   (IBusEngine             *engine);
static void ibus_keymagic_engine_cursor_up   (IBusEngine             *engine);
static void ibus_keymagic_engine_cursor_down (IBusEngine             *engine);
static void ibus_keymagic_property_activate  (IBusEngine             *engine,
                                             const gchar            *prop_name,
                                             gint                    prop_state);
static void ibus_keymagic_engine_property_show
											(IBusEngine             *engine,
                                             const gchar            *prop_name);
static void ibus_keymagic_engine_property_hide
											(IBusEngine             *engine,
                                             const gchar            *prop_name);
*/
static void ibus_keymagic_engine_commit_string
											(IBusEngine             *engine,
											 const gchar			*string);
static void ibus_keymagic_engine_commit_from_engine (IBusKeymagicEngine	*ibusKeymagic);
static void ibus_keymagic_engine_update      (IBusKeymagicEngine	*ibusKeymagic);

G_DEFINE_TYPE (IBusKeymagicEngine, ibus_keymagic_engine, IBUS_TYPE_ENGINE)

/*static gboolean
ibus_keymagic_scan_engine_name (const gchar *engine_name,
                            gchar      **lang,
                            gchar      **name)
{
    gchar **strv;

    g_return_val_if_fail (g_str_has_prefix (engine_name, "keymagic:"), FALSE);
    strv = g_strsplit (engine_name, ":", 2);

    if (g_strv_length (strv) != 2) {
        g_strfreev (strv);
        g_return_val_if_reached (FALSE);
    }

    *lang = g_strdup("en");
    *name = strv[1];

    g_free (strv[0]);
    g_free (strv);

    return TRUE;
}

static gboolean
ibus_keymagic_scan_class_name (const gchar *class_name,
                           gchar      **lang,
                           gchar      **name)
{
	gchar **strv;

	g_return_val_if_fail (g_str_has_prefix (class_name, "KeyMagic_"), FALSE);
	strv = g_strsplit (class_name, "_", 4);

	if (g_strv_length (strv) != 4) {
		g_strfreev (strv);
		g_return_val_if_reached (FALSE);
	}

	*lang = strv[1];
	*name = strv[2];

	g_free (strv[0]);
	g_free (strv);

	return TRUE;
}

GType
ibus_keymagic_engine_get_type_for_name (const gchar *engine_name)
{
    GType type;
    gchar *type_name, *lang = NULL, *name = NULL;

    GTypeInfo type_info = {
        sizeof (IBusKeymagicEngineClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) ibus_keymagic_engine_class_init,
        (GClassFinalizeFunc) NULL,
        NULL,
        sizeof (IBusKeymagicEngine),
        0,
        (GInstanceInitFunc) ibus_keymagic_engine_init,
    };

    if (!ibus_keymagic_scan_engine_name (engine_name, &lang, &name)) {
        g_free (lang);
        g_free (name);
        return G_TYPE_INVALID;
    }
    //lang[0] = g_ascii_toupper (lang[0]);
    //name[0] = g_ascii_toupper (name[0]);
    type_name = g_strdup_printf ("KeyMagic_%s_%s_Engine", lang, name);
    g_free (lang);
    g_free (name);

    type = g_type_from_name (type_name);
    g_assert (type == 0 || g_type_is_a (type, IBUS_TYPE_ENGINE));

    if (type == 0) {
        type = g_type_register_static(IBUS_TYPE_ENGINE,
									  type_name,
                                      &type_info,
                                      (GTypeFlags)0);
    }
    g_free (type_name);

    return type;
}
*/
static IBusEngineClass *parent_class = NULL;

static GObject*
ibus_keymagic_engine_constructor (GType                   type,
                              guint                   n_construct_params,
                              GObjectConstructParam  *construct_params)
{
	const gchar *engine_name;
	IBusKeymagicEngine * ibusKeymagic;

	ibusKeymagic = (IBusKeymagicEngine *) G_OBJECT_CLASS (parent_class)->constructor (type, n_construct_params, construct_params);

    engine_name = ibus_engine_get_name((IBusEngine*)ibusKeymagic);
    g_assert (engine_name);

    ibusKeymagic->kme->loadKeyboardFile(engine_name);

    return (GObject*)ibusKeymagic;
}

static void
ibus_keymagic_engine_class_init (IBusKeymagicEngineClass *klass)
{
	GObjectClass * object_class = G_OBJECT_CLASS(klass);
	IBusObjectClass *ibus_object_class = IBUS_OBJECT_CLASS (klass);
	IBusEngineClass *engine_class = IBUS_ENGINE_CLASS (klass);
	
    if (parent_class == NULL)
        parent_class = (IBusEngineClass *) g_type_class_peek_parent (klass);

	ibus_object_class->destroy = (IBusObjectDestroyFunc) ibus_keymagic_engine_destroy;
	object_class->constructor = ibus_keymagic_engine_constructor;

    engine_class->process_key_event = ibus_keymagic_engine_process_key_event;
    engine_class->focus_in = ibus_keymagic_engine_focus_in;
    engine_class->focus_out = ibus_keymagic_engine_focus_out;
    engine_class->reset = ibus_keymagic_engine_reset;
    engine_class->enable = ibus_keymagic_engine_enable;
    engine_class->disable = ibus_keymagic_engine_disable;
}


static void
ibus_keymagic_engine_init (IBusKeymagicEngine *ibusKeymagic)
{
	ibusKeymagic->kme = new KeyMagicEngine();
}

static void
ibus_keymagic_engine_focus_in (IBusEngine *engine)
{
	ibus_keymagic_engine_update((IBusKeymagicEngine*)engine);
	parent_class->focus_in(engine);
}

static void
ibus_keymagic_engine_focus_out (IBusEngine *engine)
{
	parent_class->focus_out(engine);
}

static void
ibus_keymagic_engine_reset (IBusEngine *engine)
{
	IBusKeymagicEngine *ibusKeymagic = (IBusKeymagicEngine *)engine;
	ibus_keymagic_engine_commit_from_engine(ibusKeymagic);
	parent_class->reset(engine);
}

static void
ibus_keymagic_engine_enable (IBusEngine *engine)
{
	parent_class->enable(engine);
}

static void
ibus_keymagic_engine_disable (IBusEngine *engine)
{
	IBusKeymagicEngine *ibusKeymagic = (IBusKeymagicEngine *)engine;
	ibusKeymagic->kme->reset();
	parent_class->disable(engine);
}

static void
ibus_keymagic_engine_destroy (IBusKeymagicEngine *ibusKeymagic)
{
	((IBusObjectClass *) parent_class)->destroy ((IBusObject *)ibusKeymagic);
}

static void
ibus_keymagic_engine_commit_string (IBusEngine *engine, const gchar *string){
    IBusText *text;

    text = ibus_text_new_from_string(string);
    ibus_engine_commit_text(engine, text);
}

static void
ibus_keymagic_engine_commit_from_engine(IBusKeymagicEngine *ibusKeymagic) {
	ibus_engine_hide_preedit_text((IBusEngine*)ibusKeymagic);

	KeyMagicString context = ibusKeymagic->kme->getContextText();
	if (context.length()) {
		gchar * utf8str = new gchar[context.length() * 4];
		ucs2_to_utf8_string(utf8str, context);
		ibus_keymagic_engine_commit_string((IBusEngine*)ibusKeymagic, utf8str);
		delete[] utf8str;
	}
	ibusKeymagic->kme->reset();
}

static void
ibus_keymagic_engine_update (IBusKeymagicEngine *ibusKeymagic)
{
	IBusText *text;

	KeyMagicString context = ibusKeymagic->kme->getContextText();
	gchar * utf8str = new gchar[context.length() * 4];
	ucs2_to_utf8_string(utf8str, context);

	text = ibus_text_new_from_string (utf8str);
	text->attrs = ibus_attr_list_new ();

        unsigned int context_length = ibusKeymagic->kme->getContextText().length();

	ibus_attr_list_append (text->attrs,
			ibus_attr_underline_new (IBUS_ATTR_UNDERLINE_SINGLE, 0, context_length));

	ibus_engine_update_preedit_text ((IBusEngine *)ibusKeymagic,
									 text,
									 context_length,
									 TRUE);
	delete[] utf8str;
}

#define RETURNVAL(b, c) \
	case b: \
	*kx_code = c;\
	return true

static gboolean
iBusKeycodeToKeyMagic(int keycode, int * kx_code) {
	switch (keycode) {
	RETURNVAL(14, 0x8);
	RETURNVAL(15, 0x9);
	RETURNVAL(28, 0xd);
	RETURNVAL(42, 0x10);
	RETURNVAL(54, 0x10);
	RETURNVAL(29, 0x11);
	RETURNVAL(97, 0x11);
	RETURNVAL(56, 0x12);
	RETURNVAL(100, 0x12);
	RETURNVAL(119, 0x13);
	RETURNVAL(58, 0x14);
	RETURNVAL(1, 0x1b);
	RETURNVAL(57, 0x20);
	RETURNVAL(104, 0x21);
	RETURNVAL(109, 0x22);
	RETURNVAL(83, 0x2e);

	RETURNVAL(11, 0x30);
	case 2 ... 10:
	*kx_code = keycode - 2 + 0x31;
	return true;
	//A - Z
	RETURNVAL(30, 0x41);
	RETURNVAL(48, 0x42);
	RETURNVAL(46, 0x43);
	RETURNVAL(32, 0x44);
	RETURNVAL(18, 0x45);
	RETURNVAL(33, 0x46);
	RETURNVAL(34, 0x47);
	RETURNVAL(35, 0x48);
	RETURNVAL(23, 0x49);
	RETURNVAL(36, 0x4a);
	RETURNVAL(37, 0x4b);
	RETURNVAL(38, 0x4c);
	RETURNVAL(50, 0x4d);
	RETURNVAL(49, 0x4e);
	RETURNVAL(24, 0x4f);
	RETURNVAL(25, 0x50);
	RETURNVAL(16, 0x51);
	RETURNVAL(19, 0x52);
	RETURNVAL(31, 0x53);
	RETURNVAL(20, 0x54);
	RETURNVAL(22, 0x55);
	RETURNVAL(47, 0x56);
	RETURNVAL(17, 0x57);
	RETURNVAL(45, 0x58);
	RETURNVAL(21, 0x59);
	RETURNVAL(44, 0x5a);
	//F1 ... F10
	case 59 ... 68:
	*kx_code = keycode - 59 + 0x70;
	return true;
	//F11
	RETURNVAL(87, 0x7a);
	//F12
	RETURNVAL(88, 0x7b);

	RETURNVAL(55, 0x6a);
	RETURNVAL(98, 0x6f);
	RETURNVAL(39, 0xba);
	RETURNVAL(13, 0xbb);
	RETURNVAL(51, 0xbc);
	RETURNVAL(12, 0xbd);
	RETURNVAL(52, 0xbe);
	RETURNVAL(53, 0xbf);
	RETURNVAL(41, 0xc0);
	RETURNVAL(26, 0xdb);
	RETURNVAL(43, 0xdc);
	RETURNVAL(27, 0xdd);
	RETURNVAL(40, 0xde);
	default:
		return false;
	}
	return false;
}

static gboolean 
ibus_keymagic_engine_process_key_event (IBusEngine *engine,
                                       guint       keyval,
                                       guint       keycode,
                                       guint       modifiers)
{
    IBusKeymagicEngine *ibusKeymagic = (IBusKeymagicEngine *)engine;

    if (modifiers & IBUS_RELEASE_MASK)
        return FALSE;

    std::cout << std::hex << "keyval=" << keyval << '(' << (char)keyval << ')' << ";";
    std::cout << "keycode=" << keycode << std::endl;

    int km_modifier = 0;

    unsigned char kbStates[256] = {0};
 
   if (modifiers & IBUS_CONTROL_MASK) {
    	km_modifier |= KeyMagicEngine::CTRL_MASK;
        kbStates[VK_CONTROL] = 0x80;
    }

    if (modifiers & IBUS_SHIFT_MASK) {
    	km_modifier |= KeyMagicEngine::SHIFT_MASK;
        kbStates[VK_SHIFT] = 0x80;
    }

    if (modifiers & IBUS_MOD1_MASK) {
        km_modifier |= KeyMagicEngine::ALT_MASK;
        kbStates[VK_MENU] = 0x80;
    }

    int kx_code;
    if (!iBusKeycodeToKeyMagic(keycode, &kx_code)) {
    	std::cerr << "Key won't be handled-" << keycode << std::endl;
        ibus_keymagic_engine_commit_from_engine(ibusKeymagic);
    	return false;
    }

	ibusKeymagic->kme->setKeyStates(kbStates);

        IBusText *text;
        guint pos = 0;
        ibus_engine_get_surrounding_text((IBusEngine*)ibusKeymagic, &text, &pos);

        std::cout << "text=" << ibus_text_get_text(text) << "pos=" << pos << std::endl;

        g_object_unref(text);
        
	bool success = ibusKeymagic->kme->processKeyEvent(keyval, kx_code, km_modifier);

	if (success) {
		ibus_keymagic_engine_update(ibusKeymagic);
		return true;
	} else if (kx_code == 0x20 || kx_code == 0x0D) {
		ibus_keymagic_engine_commit_from_engine(ibusKeymagic);
	} else if ((modifiers & IBUS_CONTROL_MASK) || (modifiers & IBUS_MOD1_MASK)) {
		ibus_keymagic_engine_commit_from_engine(ibusKeymagic);
	}

    return false;
}
