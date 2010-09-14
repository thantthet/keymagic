//
// ComponentConfig.h
//
//Copyright (C) 2008  KeyMagic Project
//http://keymagic.googlecode.com
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef ComponentConfig_h
#define ComponentConfig_h

// configurables
#define TSMC_VERSION            0x00010000	// 1.0
#define TSMC_VENDOR             'TTKZ'
#define TSMC_NAME               "KeyMagic"
#define TSMC_NAME_LENGTH        $"08"
#define TSMC_BUNDLE_ID          "org.keymagic.KeyMagic-TSM"
#define TSMC_BUNDLE_ID_LENGTH   $"18"
#define TSMC_LOADER_APP_NAME			@"KeyMagicServer.app"
#define TSMC_SERVER_CONNECTION_NAME		@"KeyMagic_1_Connection"
#define TSMC_LOADER_SERVER_BUNDLE_ID	@"org.keymagic.KeyMagicServer"

// change this if you want to do different stuff
#define TSMC_SCRIPT				2           // smTradCinese
#define TSMC_LANGUAGE           19          // langtradChinese
#define TSMC_RESOURCE_ID        16896       // (15872+script_id*512) here script_id=smTradChinese

// derived variables
#define TSMC_PENCIL_MENU_ID		TSMC_RESOURCE_ID + 1
#define TSMC_FLAGS              0x8000 + TSMC_SCRIPT * 0x100 + TSMC_LANGUAGE
#define TARGET_REZ_MAC_PPC          1
#define TARGET_REZ_MAC_X86          1

#endif

//
// Free Tibet
//
