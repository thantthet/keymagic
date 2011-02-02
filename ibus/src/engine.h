/* vim:set et sts=4: */
#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <ibus.h>
#include <string.h>
#include "KeyMagicEngine.h"

//#define IBUS_TYPE_KEYMAGIC_ENGINE	\
//	(ibus_keymagic_engine_get_type ())

//GType   ibus_keymagic_engine_get_type    (void);
GType   ibus_keymagic_engine_get_type_for_name (const gchar *name);

inline int ucs2_to_utf8 (int ucs2, unsigned char * utf8)
{
    if (ucs2 < 0x80) {
        utf8[0] = ucs2;
        utf8[1] = '\0';
        return 1;
    }
    if (ucs2 >= 0x80  && ucs2 < 0x800) {
        utf8[0] = (ucs2 >> 6)   | 0xC0;
        utf8[1] = (ucs2 & 0x3F) | 0x80;
        utf8[2] = '\0';
        return 2;
    }
    if (ucs2 >= 0x800 && ucs2 < 0xFFFF) {
        utf8[0] = ((ucs2 >> 12)       ) | 0xE0;
        utf8[1] = ((ucs2 >> 6 ) & 0x3F) | 0x80;
        utf8[2] = ((ucs2      ) & 0x3F) | 0x80;
        utf8[3] = '\0';
        return 3;
    }
    return -1;
}

inline int ucs2_to_utf8_string (gchar * output, KeyMagicString * input) {
	*output = '\0';

	for (KeyMagicString::iterator i = input->begin(); i != input->end(); i++) {
		int usc2 = *i;
		char utf8[4];
		ucs2_to_utf8(usc2, (unsigned char*)&utf8);
		strcat(output, utf8);
	}
}

#endif
