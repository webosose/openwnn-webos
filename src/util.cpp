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

#include "util.h"

wstring toLower(const wstring str)
{
    wstring data;
    for (int i=0; i<str.size(); ++i) {
        data += std::towlower(str.at(i));
    }
    return data;
}

wstring toUpper(const wstring str)
{
    wstring data;
    for (int i=0; i<str.size(); ++i) {
         data += std::towupper(str.at(i));
    }
    return data;
}

// convert UTF-8 string to wstring
wstring utf8_to_wstring (const string& str)
{
    wstring_convert<codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
string wstring_to_utf8 (const wstring& str)
{
    wstring_convert<codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

