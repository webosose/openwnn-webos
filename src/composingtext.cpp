/*
 * C++ implementation of OpenWnn library
 *
 * Copyright (c) 2019-2023 LG Electronics, Inc.
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

#include "composingtext.h"

void ComposingText::modifyUpper(ComposingText::TextLayer layer, int mod_from, int mod_len, int org_len)
{
    if (layer >= (ComposingText::MAX_LAYER - 1)) {
        /* no layer above */
        return;
    }

    ComposingText::TextLayer uplayer = (ComposingText::TextLayer)(layer + 1);
    vector<StrSegment> &strUplayer = mStringLayer[uplayer];
    if (strUplayer.size() <= 0) {
        /*
         * if there is no element on above layer,
         * add a element includes whole elements of the lower layer.
         */
        strUplayer.push_back(StrSegment(toString(layer), 0, mStringLayer[layer].size() - 1));
        modifyUpper(uplayer, 0, 1, 0);
        return;
    }

    int mod_to = mod_from + ((mod_len == 0) ? 0 : (mod_len - 1));
    int org_to = mod_from + ((org_len == 0) ? 0 : (org_len - 1));
    StrSegment &last = strUplayer[strUplayer.size() - 1];
    if (last.to < mod_from) {
        /* add at the tail */
        last.to = mod_to;
        last.str = toString(layer, last.from, last.to);
        modifyUpper(uplayer, strUplayer.size()-1, 1, 1);
        return;
    }

    int uplayer_mod_from = -1;
    int uplayer_org_to = -1;
    for (int i = 0; i < strUplayer.size(); i++) {
        const StrSegment &ss = strUplayer.at(i);
        if (ss.from > mod_from) {
            if (ss.to <= org_to) {
                /* the segment is included */
                if (uplayer_mod_from < 0) {
                    uplayer_mod_from = i;
                }
                uplayer_org_to = i;
            } else {
                /* included in this segment */
                uplayer_org_to = i;
                break;
            }
        } else {
            if (org_len == 0 && ss.from == mod_from) {
                /* when an element is added */
                uplayer_mod_from = i - 1;
                uplayer_org_to   = i - 1;
                break;
            } else {
                /* start from this segment */
                uplayer_mod_from = i;
                uplayer_org_to = i;
                if (ss.to >= org_to) {
                    break;
                }
            }
        }
    }

    int diff = mod_len - org_len;
    if (uplayer_mod_from >= 0) {
        /* update an element */
        StrSegment &ss = strUplayer[uplayer_mod_from];
        int last_to = ss.to;
        int next = uplayer_mod_from + 1;
        for (int i = next; i <= uplayer_org_to; i++) {
            const StrSegment &ss2 = strUplayer.at(next);
            if (last_to > ss2.to) {
                last_to = ss2.to;
            }
            strUplayer.erase(strUplayer.begin()+next);
        }
        ss.to = (last_to < mod_to)? mod_to : (last_to + diff);

        ss.str = toString(layer, ss.from, ss.to);

        for (int i = next; i < strUplayer.size(); i++) {
            StrSegment &ss2 = strUplayer[i];
            ss2.from += diff;
            ss2.to   += diff;
        }

        modifyUpper(uplayer, uplayer_mod_from, 1, uplayer_org_to - uplayer_mod_from + 1);
    } else {
        /* add an element at the head */
        strUplayer.insert(strUplayer.begin(), StrSegment(toString(layer, mod_from, mod_to), mod_from, mod_to));
        for (int i = 1; i < strUplayer.size(); i++) {
            StrSegment &ss = strUplayer[i];
            ss.from += diff;
            ss.to   += diff;
        }
        modifyUpper(uplayer, 0, 1, 0);
    }
}

void ComposingText::deleteStrSegment0(ComposingText::TextLayer layer, int from, int to, int diff)
{
    vector<StrSegment> &strLayer = mStringLayer[layer];
    if (diff != 0) {
        for (int i = to + 1; i < strLayer.size(); i++) {
            StrSegment &ss = strLayer[i];
            ss.from -= diff;
            ss.to   -= diff;
        }
    }
    for (int i = from; i <= to; i++) {
        if (from >= strLayer.size())
            break;

        strLayer.erase(strLayer.begin() + from);
    }
}

void ComposingText::replaceStrSegment0(ComposingText::TextLayer layer, const vector<StrSegment> &str, int from, int to)
{
    vector<StrSegment> &strLayer = mStringLayer[layer];

    if (from < 0 || from > strLayer.size()) {
        from = strLayer.size();
    }
    if (to < 0 || to > strLayer.size()) {
        to = strLayer.size();
    }
    for (int i = from; i <= to; i++) {
        strLayer.erase(strLayer.begin()+from);
    }
    for (int i = str.size() - 1; i >= 0; i--) {
        strLayer.insert(strLayer.begin()+from, str.at(i));
    }

    modifyUpper(layer, from, str.size(), to - from + 1);
}

ComposingText::ComposingText() { }

ComposingText::~ComposingText() { }

StrSegment ComposingText::getStrSegment(TextLayer layer, int pos) const
{
    if (layer < LAYER0 || layer >= MAX_LAYER)
        return StrSegment();

    const vector<StrSegment> &strLayer = mStringLayer[layer];
    if (pos < 0) {
        pos = strLayer.size() - 1;
    }
    if (pos >= strLayer.size() || pos < 0) {
        return StrSegment();
    }
    return strLayer.at(pos);
}

wstring ComposingText::toString(TextLayer layer, int from, int to) const
{

    if (layer < LAYER0 || layer >= MAX_LAYER)
        return wstring();

    wstring buf;
    const vector<StrSegment> &strLayer = mStringLayer[layer];

    for (int i = from; i <= to; i++) {
        const StrSegment &ss = strLayer.at(i);
        buf.append(ss.str);
    }
    return buf;
}

wstring ComposingText::toString(TextLayer layer) const
{
    if (layer < LAYER0 || layer >= MAX_LAYER)
        return wstring();

    return toString(layer, 0, mStringLayer[layer].size() - 1);
}

void ComposingText::insertStrSegment(TextLayer layer, const StrSegment& str)
{

    if (layer < LAYER0 || layer >= MAX_LAYER)
        return;

    int cursor = mCursor[layer];
    if (cursor >= mStringLayer[layer].size())
        return;

    mStringLayer[layer].insert(mStringLayer[layer].begin()+cursor, str);
    modifyUpper(layer, cursor, 1, 0);
    setCursor(layer, cursor + 1);
}

void ComposingText::insertStrSegment(TextLayer layer1, TextLayer layer2, const StrSegment &str)
{

    if (layer1 < LAYER0 || layer1 >= MAX_LAYER || layer2 < LAYER0 || layer2 >= MAX_LAYER)
        return;

    if (mCursor[layer1] >= mStringLayer[layer1].size())
        return;

    mStringLayer[layer1].insert(mStringLayer[layer1].begin()+mCursor[layer1], str);
    mCursor[layer1]++;

    for (int i = (int)layer1 + 1; i <= (int)layer2; i++) {
        int pos = mCursor[i - 1] - 1;
        StrSegment tmp(str.str, pos, pos);
        vector<StrSegment> &strLayer = mStringLayer[i];
        strLayer.insert(strLayer.begin() + mCursor[i], tmp);
        mCursor[i]++;
        for (int j = mCursor[i]; j < strLayer.size(); j++) {
            StrSegment &ss = strLayer[j];
            ss.from++;
            ss.to++;
        }
    }
    int cursor = mCursor[layer2];
    modifyUpper(layer2, cursor - 1, 1, 0);
    setCursor(layer2, cursor);
}

void ComposingText::replaceStrSegment(TextLayer layer, const vector<StrSegment> &str, int num)
{

    if (layer < LAYER0 || layer >= MAX_LAYER)
        return;

    int cursor = mCursor[layer];
    replaceStrSegment0(layer, str, cursor - num, cursor - 1);
    setCursor(layer, cursor + str.size() - num);
}

void ComposingText::deleteStrSegment(TextLayer layer, int from, int to)
{
    if (layer < LAYER0 || layer >= MAX_LAYER)
        return;

    int fromL[3] = { -1, -1, -1 };
    int toL[3]   = { -1, -1, -1 };

    vector<StrSegment> &strLayer2 = mStringLayer[LAYER2];
    vector<StrSegment> &strLayer1 = mStringLayer[LAYER1];

    if (layer == LAYER2) {
        fromL[LAYER2] = from;
        toL[LAYER2]   = to;
        fromL[LAYER1] = strLayer2.at(from).from;
        toL[LAYER1]   = strLayer2.at(to).to;
        fromL[LAYER0] = strLayer1.at(fromL[LAYER1]).from;
        toL[LAYER0]   = strLayer1.at(toL[LAYER1]).to;
    } else if (layer == LAYER1) {
        fromL[LAYER1] = from;
        toL[LAYER1]   = to;
        fromL[LAYER0] = strLayer1.at(from).from;
        toL[LAYER0]   = strLayer1.at(to).to;
    } else {
        fromL[LAYER0] = from;
        toL[LAYER0]   = to;
    }

    int diff = to - from + 1;
    for (int lv = LAYER0; lv < MAX_LAYER; lv++) {
        if (fromL[lv] >= 0) {
            deleteStrSegment0((TextLayer)lv, fromL[lv], toL[lv], diff);
        } else {
            int boundary_from = -1;
            int boundary_to   = -1;
            vector<StrSegment> &strLayer = mStringLayer[lv];
            for (int i = 0; i < strLayer.size(); i++) {
                const StrSegment &ss = strLayer.at(i);
                if ((ss.from >= fromL[lv-1] && ss.from <= toL[lv-1]) ||
                    (ss.to >= fromL[lv-1] && ss.to <= toL[lv-1]) ) {
                    if (fromL[lv] < 0) {
                        fromL[lv] = i;
                        boundary_from = ss.from;
                    }
                    toL[lv] = i;
                    boundary_to = ss.to;
                } else if (ss.from <= fromL[lv-1] && ss.to >= toL[lv-1]) {
                    boundary_from = ss.from;
                    boundary_to   = ss.to;
                    fromL[lv] = i;
                    toL[lv] = i;
                    break;
                } else if (ss.from > toL[lv-1]) {
                    break;
                }
            }
            if (boundary_from != fromL[lv-1] || boundary_to != toL[lv-1]) {
                deleteStrSegment0((TextLayer)lv, fromL[lv] + 1, toL[lv], diff);
                boundary_to -= diff;
                vector<StrSegment> tmp = vector<StrSegment>();
                tmp.push_back(StrSegment(toString((TextLayer)(lv - 1)), boundary_from, boundary_to));
                replaceStrSegment0((TextLayer)lv, tmp, fromL[lv], fromL[lv]);
                return;
            } else {
                deleteStrSegment0((TextLayer)lv, fromL[lv], toL[lv], diff);
            }
        }
        diff = toL[lv] - fromL[lv] + 1;
    }
}

int ComposingText::deleteAt(TextLayer layer, bool rightside)
{
    if (layer < LAYER0 || layer >= MAX_LAYER)
        return 0;

    int cursor = mCursor[layer];
    vector<StrSegment> &strLayer = mStringLayer[layer];

    if (!rightside && cursor > 0) {
        deleteStrSegment(layer, cursor - 1, cursor - 1);
        setCursor(layer, cursor - 1);
    } else if (rightside && cursor < strLayer.size()) {
        deleteStrSegment(layer, cursor, cursor);
        setCursor(layer, cursor);
    }
    return strLayer.size();
}

vector<StrSegment> ComposingText::getStringLayer(TextLayer layer) const
{
    if (layer < LAYER0 || layer >= MAX_LAYER)
        return vector<StrSegment>();

    return mStringLayer[layer];
}

int ComposingText::included(TextLayer layer, int pos)
{
    if (layer < LAYER0 || layer >= MAX_LAYER - 1)
        return 0;

    if (pos == 0) {
        return 0;
    }
    int uplayer = (TextLayer)(layer + 1);
    int i;
    vector<StrSegment> &strLayer = mStringLayer[uplayer];
    for (i = 0; i < strLayer.size(); i++) {
        const StrSegment &ss = strLayer.at(i);
        if (ss.from <= pos && pos <= ss.to) {
            break;
        }
    }
    return i;
}

int ComposingText::setCursor(TextLayer layer, int pos)
{
    if (layer < LAYER0 || layer >= MAX_LAYER)
        return -1;

    if (pos > mStringLayer[layer].size()) {
        pos = mStringLayer[layer].size();
    }
    if (pos < 0) {
        pos = 0;
    }
    if (layer == ComposingText::LAYER0) {
        mCursor[ComposingText::LAYER0] = pos;
        mCursor[ComposingText::LAYER1] = included(ComposingText::LAYER0, pos);
        mCursor[ComposingText::LAYER2] = included(ComposingText::LAYER1, mCursor[ComposingText::LAYER1]);
    } else if (layer == ComposingText::LAYER1) {
        mCursor[ComposingText::LAYER2] = included(ComposingText::LAYER1, pos);
        mCursor[ComposingText::LAYER1] = pos;
        mCursor[ComposingText::LAYER0] = (pos > 0) ? mStringLayer[ComposingText::LAYER1].at(pos - 1).to + 1 : 0;
    } else {
        mCursor[ComposingText::LAYER2] = pos;
        mCursor[ComposingText::LAYER1] = (pos > 0) ? mStringLayer[ComposingText::LAYER2].at(pos - 1).to + 1 : 0;
        mCursor[ComposingText::LAYER0] = (mCursor[ComposingText::LAYER1] > 0) ? mStringLayer[ComposingText::LAYER1].at(mCursor[ComposingText::LAYER1] - 1).to + 1 : 0;
    }
    return pos;
}

int ComposingText::moveCursor(TextLayer layer, int diff)
{
    if (layer < LAYER0 || layer >= MAX_LAYER)
        return -1;

    int c = mCursor[layer] + diff;

    return setCursor(layer, c);
}

int ComposingText::getCursor(TextLayer layer) const
{
    if (layer < LAYER0 || layer >= MAX_LAYER)
        return -1;

    return mCursor[layer];
}

int ComposingText::size(TextLayer layer) const
{
    if (layer < LAYER0 || layer >= MAX_LAYER)
        return 0;

    return mStringLayer[layer].size();
}

void ComposingText::clear()
{
    for (int i = 0; i < MAX_LAYER; i++) {
        mStringLayer[i].clear();
        mCursor[i] = 0;
    }
}
