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

#ifndef OPENWNNINPUTMETHOD_H
#define OPENWNNINPUTMETHOD_H

#include <string>
#include <vector>
#include <memory>

using namespace std;

class OpenWnnInputMethodPrivate;
class OpenWnnInputMethod
{
public:
     OpenWnnInputMethod();
    ~OpenWnnInputMethod();

    enum InputMode {
        Hiragana,
        Katakana,
        FullwidthLatin,
        Latin,
    };

    bool setInputMode(InputMode inputMode);
    bool keyEvent(int key, const wstring &text);
    int candidateListItemCount();
    void candidateListItemSelected(int index);
    vector<wstring>* getCandidateList();

    wstring getPreedit();
    wstring getCommit();
    bool isComposing();

    void reset();
    void update();

private:
    unique_ptr<OpenWnnInputMethodPrivate> p;
};

#endif

