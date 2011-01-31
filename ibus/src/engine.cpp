/* vim:set et sts=4: */

#include <iconv.h>
#include <enchant.h>
#include "engine.h"
#include "KeyMagicEngine.h"
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

static void ibus_keymagic_engine_commit_string
											(IBusEngine             *engine,
											 const gchar			*string);
static void ibus_keymagic_engine_commit_from_engine (IBusKeymagicEngine	*ibusKeymagic);
static void ibus_keymagic_engine_update      (IBusKeymagicEngine	*ibusKeymagic);

G_DEFINE_TYPE (IBusKeymagicEngine, ibus_keymagic_engine, IBUS_TYPE_ENGINE)

static void
ibus_keymagic_engine_class_init (IBusKeymagicEngineClass *klass)
{
	IBusObjectClass *ibus_object_class = IBUS_OBJECT_CLASS (klass);
	IBusEngineClass *engine_class = IBUS_ENGINE_CLASS (klass);
	
	ibus_object_class->destroy = (IBusObjectDestroyFunc) ibus_keymagic_engine_destroy;

    engine_class->process_key_event = ibus_keymagic_engine_process_key_event;
}

static void
ibus_keymagic_engine_init (IBusKeymagicEngine *ibusKeymagic)
{
	ibusKeymagic->kme = new KeyMagicEngine();
    ibusKeymagic->kme->loadKeyboardFile("/home/thantthetkz/workspace/KeyMagic/Debug/myWin.km2");
}

static void
ibus_keymagic_engine_destroy (IBusKeymagicEngine *ibusKeymagic)
{
	((IBusObjectClass *) ibus_keymagic_engine_parent_class)->destroy ((IBusObject *)ibusKeymagic);
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

	KeyMagicString * context = ibusKeymagic->kme->getContextText();
	gchar * utf8str = new gchar[context->length() * 4];
	ucs2_to_utf8_string(utf8str, context);
	ibus_keymagic_engine_commit_string((IBusEngine*)ibusKeymagic, utf8str);
	delete[] utf8str;
	ibusKeymagic->kme->reset();
}

static void
ibus_keymagic_engine_update (IBusKeymagicEngine *ibusKeymagic)
{
	IBusText *text;

	KeyMagicString * context = ibusKeymagic->kme->getContextText();
	gchar * utf8str = new gchar[context->length() * 4];
	ucs2_to_utf8_string(utf8str, context);

	text = ibus_text_new_from_string (utf8str);
	text->attrs = ibus_attr_list_new ();

	ibus_attr_list_append (text->attrs,
			ibus_attr_underline_new (IBUS_ATTR_UNDERLINE_SINGLE, 0, ibusKeymagic->kme->getContextText()->length()));

	ibus_engine_update_preedit_text ((IBusEngine *)ibusKeymagic,
									 text,
									 ibusKeymagic->kme->getContextText()->length(),
									 TRUE);
	delete[] utf8str;
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

    std::cerr << std::hex << modifiers << std::endl;
    std::cerr << "ctrl=" << (modifiers & IBUS_CONTROL_MASK) << std::endl;
    std::cerr << "shift=" << (modifiers & IBUS_SHIFT_MASK) << std::endl;
    std::cerr << "alt=" << (modifiers & IBUS_MOD1_MASK) << std::endl;

    int km_modifier = 0;
    if (modifiers & IBUS_CONTROL_MASK) {
    	km_modifier |= KeyMagicEngine::CTRL_MASK;
    }
    if (modifiers & IBUS_SHIFT_MASK) {
    	km_modifier |= KeyMagicEngine::SHIFT_MASK;
    }
    if (modifiers & IBUS_MOD1_MASK) {
		km_modifier |= KeyMagicEngine::ALT_MASK;
	}

    IBusKeymap * keymap = ibus_keymap_get("us");
    int k;
    switch (keyval) {
    case IBUS_BackSpace:
    	k = 0x8;
    	keyval = 0x8;
    	break;
    case IBUS_Shift_L:
    case IBUS_Shift_R:
    	k = 0x10;
    	keyval = 0x10;
    	break;
    case IBUS_Control_L:
    case IBUS_Control_R:
    	k= 0x11;
    	keyval = 0x11;
    case IBUS_Alt_L:
    case IBUS_Alt_R:
    	k = 0x12;
    	keyval = 0x12;
    	break;
    case IBUS_Return:
    	k = 0x0D;
    	keyval = 0x0D;
    	break;
    default:
    	k = ibus_keymap_lookup_keysym(keymap, keycode, 2);
    	break;
    }
	if (k > 0xff) {
    	std::cerr << "Key won't be handled-" << keycode << std::endl;
   	} else {
   		bool y = ibusKeymagic->kme->processKeyEvent(keyval, k, km_modifier);

		if (y) {
			ibus_keymagic_engine_update(ibusKeymagic);
			return true;
		} else if (k == 0x20 || k == 0x0D) {
			ibus_keymagic_engine_commit_from_engine(ibusKeymagic);
		} else if ((modifiers & IBUS_CONTROL_MASK) || (modifiers & IBUS_MOD1_MASK)) {
			ibus_keymagic_engine_commit_from_engine(ibusKeymagic);
	    }
   	}
    return false;
}
