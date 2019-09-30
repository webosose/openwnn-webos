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

#ifndef COMPOSINGTEXT_H
#define COMPOSINGTEXT_H

#include <vector>
#include "strsegment.h"

using namespace std;

class ComposingText
{

public:
    ComposingText();
    ~ComposingText();

    enum TextLayer {
        /**
         * Text layer 0.
         * <br>
         * This text layer holds key strokes.<br>
         * (ex) Romaji in Japanese.  Parts of Hangul in Korean.
         */
        LAYER0  = 0,
        /**
         * Text layer 1.
         * <br>
         * This text layer holds the result of the letter converter.<br>
         * (ex) Hiragana in Japanese. Pinyin in Chinese. Hangul in Korean.
         */
        LAYER1  = 1,
        /**
         * Text layer 2.
         * <br>
         * This text layer holds the result of the consecutive clause converter.<br>
         * (ex) the result of Kana-to-Kanji conversion in Japanese,
         *      Pinyin-to-Kanji conversion in Chinese, Hangul-to-Hanja conversion in Korean language.
         */
        LAYER2  = 2,
        /** Maximum number of layers */
        MAX_LAYER = 3
    };

    StrSegment getStrSegment(TextLayer layer, int pos) const;
    wstring toString(TextLayer layer, int from, int to) const;
    wstring toString(TextLayer layer) const;
    void insertStrSegment(TextLayer layer, const StrSegment& str);
    void insertStrSegment(TextLayer layer1, TextLayer layer2, const StrSegment &str);
    void replaceStrSegment(TextLayer layer, const vector<StrSegment> &str, int num = 1);
    void deleteStrSegment(TextLayer layer, int from, int to);
    int deleteAt(TextLayer layer, bool rightside);
    vector<StrSegment> getStringLayer(TextLayer layer) const;
    int included(TextLayer layer, int pos);
    int setCursor(TextLayer layer, int pos);
    int moveCursor(TextLayer layer, int diff);
    int getCursor(TextLayer layer) const;
    int size(TextLayer layer) const;
    void clear();

private:
    void modifyUpper(ComposingText::TextLayer layer, int mod_from, int mod_len, int org_len);
    void deleteStrSegment0(ComposingText::TextLayer layer, int from, int to, int diff);
    void replaceStrSegment0(ComposingText::TextLayer layer, const vector<StrSegment>  &str, int from, int to);
    vector<StrSegment> mStringLayer[ComposingText::MAX_LAYER];
    int mCursor[ComposingText::MAX_LAYER];
};

#endif // COMPOSINGTEXT_H

