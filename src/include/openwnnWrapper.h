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

#ifndef OPENWNNWRAPPER_H
#define OPENWNNWRAPPER_H

#include <vector>
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
void *initOpenwnn();
void setInputMode(void* obj,int mode);
bool processKey(void* obj, int key, const wstring &text);
wstring getPreedit(void* obj);
wstring getCommit(void* obj);
int candidateListItemCount(void* obj);
void candidateListItemSelected(void* obj,int index);
vector<wstring>* getCandidateList(void* obj);
bool isComposing(void* obj);
void reset(void* obj);
void close(void* obj);

#ifdef __cplusplus
}
#endif

#endif /* OPENWNNWRAPPER_H */

