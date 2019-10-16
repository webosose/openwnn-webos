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

#include "openwnnWrapper.h"
#include "openwnninputmethod.h"

#include <vector>
#include <string>
#include <iostream>

extern "C"
{
    void *initOpenwnn()
    {
        OpenWnnInputMethod* context = new OpenWnnInputMethod();
        return context;
    }

    void setInputMode(void* obj,int mode)
    {
        OpenWnnInputMethod* context = reinterpret_cast<OpenWnnInputMethod*>(obj);
        context->setInputMode((OpenWnnInputMethod::InputMode)mode);
    }

    bool processKey(void* obj, int key, const wstring &text)
    {
        OpenWnnInputMethod* context = reinterpret_cast<OpenWnnInputMethod*>(obj);
        return context->keyEvent(key,text);
    }

    wstring getPreedit(void* obj)
    {
        OpenWnnInputMethod* context = reinterpret_cast<OpenWnnInputMethod*>(obj);
        return context->getPreedit();
    }

    wstring getCommit(void* obj)
    {
        OpenWnnInputMethod* context = reinterpret_cast<OpenWnnInputMethod*>(obj);
        return context->getCommit();
    }

    int candidateListItemCount(void* obj)
    {
        OpenWnnInputMethod* context = reinterpret_cast<OpenWnnInputMethod*>(obj);
        return context->candidateListItemCount();
    }

    void candidateListItemSelected(void* obj,int index)
    {
        OpenWnnInputMethod* context = reinterpret_cast<OpenWnnInputMethod*>(obj);
        context->candidateListItemSelected(index);
    }

    vector<wstring>* getCandidateList(void* obj)
    {
        OpenWnnInputMethod* context = reinterpret_cast<OpenWnnInputMethod*>(obj);
        return context->getCandidateList();
    }

    bool isComposing(void* obj)
    {
        OpenWnnInputMethod* context = reinterpret_cast<OpenWnnInputMethod*>(obj);
        return context->isComposing();
    }

    void reset(void* obj)
    {
        OpenWnnInputMethod* context = reinterpret_cast<OpenWnnInputMethod*>(obj);
        context->reset();
    }

    void close(void* obj)
    {
        OpenWnnInputMethod* context = reinterpret_cast<OpenWnnInputMethod*>(obj);
        if (context) delete context;
    }
}
