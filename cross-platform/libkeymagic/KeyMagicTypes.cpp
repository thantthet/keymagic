/*
 * KeyMagicTypes.m
 *
 * Copyright (C) 2008  KeyMagic Project
 * http://keymagic.googlecode.com
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "KeyMagicTypes.h"

namespace libkm {

using namespace std;

    Info::Info () {
        size = 0;
        data = 0;
    }

    Info::Info (const Info& info) {
        size = 0;
        data = 0;

        SetData(info.data, info.size);
    }

    void Info::SetData(char * d, short s) {
        if (data != 0) {
            delete[] data;
        }
        
        data = new char[s];
        memcpy(data, d, sizeof(char) * s);
        size = s;
    }

    short Info::Size () {
        return size;
    }

    const char * Info::Data () {
        return data;
    }

    Info::~Info () {
        if (data != 0) {
            delete[] data;
        }
    }
}
