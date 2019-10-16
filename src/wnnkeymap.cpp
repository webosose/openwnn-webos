/*
 * C++ implementation of OpenWnn library
 *
 * Copyright (c) 2019 LG Electronics, Inc.
 * Copyright (C) 2008-2012  OMRON SOFTWARE Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <algorithm>
#include <cstring>
#include "util.h"
#include "wnnkeymap.h"

WnnKeyMap::WnnKeyMap(const char **keys, const char **values, const int length) :
    keys(keys),
    values(values),
    length(length)
{
}

const wstring WnnKeyMap::value(const wstring &what) const
{
    string utf8Str = wstring_to_utf8(what);
    const char *whatKey = utf8Str.c_str();
    const char **key = lower_bound(keys, keys + length, whatKey,
                [](const char *lhs, const char *rhs) { return strcmp(lhs, rhs) < 0; });
    if ((key == (keys + length)) || strcmp(whatKey,*key) != 0)
        return wstring();
    int index = key - keys;
    if (index == length)
        return wstring();
    string utf8 = string(values[index]);
    return utf8_to_wstring(utf8);
}
