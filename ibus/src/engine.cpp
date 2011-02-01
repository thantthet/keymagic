/* vim:set et sts=4: */

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

static IBusEngineClass *parent_class = NULL;

static void
ibus_keymagic_engine_class_init (IBusKeymagicEngineClass *klass)
{
	IBusObjectClass *ibus_object_class = IBUS_OBJECT_CLASS (klass);
	IBusEngineClass *engine_class = IBUS_ENGINE_CLASS (klass);
	
    if (parent_class == NULL)
        parent_class = (IBusEngineClass *) g_type_class_peek_parent (klass);

	ibus_object_class->destroy = (IBusObjectDestroyFunc) ibus_keymagic_engine_destroy;

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
    ibusKeymagic->kme->loadKeyboardFile("/home/thantthetkz/workspace/KeyMagic/Debug/Zawgyi L - Unicode.km2");
}

static void
ibus_keymagic_engine_focus_in (IBusEngine *engine)
{
	std::cerr << "ibus_keymagic_engine_focus_in" << std::endl;
	ibus_keymagic_engine_update((IBusKeymagicEngine*)engine);
	parent_class->focus_in(engine);
}

static void
ibus_keymagic_engine_focus_out (IBusEngine *engine)
{
	std::cerr << "ibus_keymagic_engine_focus_out" << std::endl;
	parent_class->focus_out(engine);
}

static void
ibus_keymagic_engine_reset (IBusEngine *engine)
{
	std::cerr << "ibus_keymagic_engine_reset" << std::endl;

	IBusKeymagicEngine *ibusKeymagic = (IBusKeymagicEngine *)engine;
	ibus_keymagic_engine_commit_from_engine(ibusKeymagic);
	parent_class->reset(engine);
}

static void
ibus_keymagic_engine_enable (IBusEngine *engine)
{
	std::cerr << "ibus_keymagic_engine_enable" << std::endl;

	IBusKeymagicEngine *ibusKeymagic = (IBusKeymagicEngine *)engine;
	parent_class->enable(engine);
}

static void
ibus_keymagic_engine_disable (IBusEngine *engine)
{
	std::cerr << "ibus_keymagic_engine_disable" << std::endl;

	IBusKeymagicEngine *ibusKeymagic = (IBusKeymagicEngine *)engine;
	ibusKeymagic->kme->reset();
	parent_class->disable(engine);
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
	if (context->length()) {
		gchar * utf8str = new gchar[context->length() * 4];
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

    std::cerr << std::hex << "keyval=" << keyval << '(' << (char)keyval << ')' << ";";
    std::cerr << "keycode=" << keycode << std::endl;

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

    int kx_code;
    if (!iBusKeycodeToKeyMagic(keycode, &kx_code)) {
    	std::cerr << "Key won't be handled-" << keycode << std::endl;
    	return false;
    }

	bool y = ibusKeymagic->kme->processKeyEvent(keyval, kx_code, km_modifier);

	if (y) {
		ibus_keymagic_engine_update(ibusKeymagic);
		return true;
	} else if (kx_code == 0x20 || kx_code == 0x0D) {
		ibus_keymagic_engine_commit_from_engine(ibusKeymagic);
	} else if ((modifiers & IBUS_CONTROL_MASK) || (modifiers & IBUS_MOD1_MASK)) {
		//ibus_keymagic_engine_commit_from_engine(ibusKeymagic);
	}

    return false;
}
