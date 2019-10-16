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

#ifndef STRSEGMENT_H
#define STRSEGMENT_H

#include <string>
#include <memory>
#include "wnnword.h"

using namespace std;

class StrSegment
{
public:
    StrSegment(const wstring &s = L"", int from = -1, int to = -1) :
        str(s),
        from(from),
        to(to)
    { }

    StrSegment(const WnnClause &clause, int from, int to) :
        str(clause.candidate),
        from(from),
        to(to),
        clause(new WnnClause(clause))
    { }

    wstring str;
    int from;
    int to;
    shared_ptr<WnnClause> clause;
};

#endif // STRSEGMENT_H



