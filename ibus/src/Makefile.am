# vim:set noet ts=4:
#
# ibus-anthy - The Anthy engine for IBus
#
# Copyright (c) 2007-2008 Huang Peng <shawn.p.huang@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


libexec_PROGRAMS = ibus-engine-keymagic

ibus_engine_keymagic_SOURCES = \
	main.cpp \
	engine.cpp \
	engine.h \
	KeyMagicEngine.h \
	KeyMagicEngine.cpp \
	KeyMagicString.h \
	KeyMagicString.cpp \
	KeyMagicKeyboard.h \
	KeyMagicKeyboard.cpp \
	LogFileWriter.h \
	LogFileWriter.cpp \
	KeyMagicLogger.h \
	KeyMagicLogger.cpp \
	KeyMagicErrorLogger.h \
	KeyMagicErrorLogger.cpp \
	RuleInfo.h \
	RuleInfo.cpp \
	KeyCodes.h \
	KeyCodes.cpp \
	KeyMagicTypes.h \
	$(NULL)
ibus_engine_keymagic_CXXFLAGS = \
	@IBUS_CFLAGS@ \
	-DPKGDATADIR=\"$(pkgdatadir)\"
	$(NULL)
ibus_engine_keymagic_LDFLAGS = \
	@IBUS_LIBS@ \
	$(NULL)

component_DATA = \
	keymagic.xml \
	$(NULL)
componentdir = @datadir@/ibus/component

EXTRA_DIST = \
	keymagic.xml.in \
	$(NULL)

CLEANFILES = \
	keymagic.xml \
	$(NULL)

keymagic.xml: keymagic.xml.in
	$(AM_V_GEN) \
	( \
		libexecdir=${libexecdir}; \
		pkgdatadir=${pkgdatadir}; \
		s=`cat $<`; \
		eval "echo \"$${s}\""; \
	) > $@

test: ibus-engine-keymagic
	$(builddir)/ibus-engine-keymagic
