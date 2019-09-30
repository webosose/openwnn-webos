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
#include "kanaconverter.h"
#include "wnnkeymap.h"
#include "util.h"

static const int halfKatakanaLength = 83;
static const int fullKatakanaLength = 83;
static const int fullAlphabetLength = 52;

static const char *halfKatakanaKey[] = {
"ぁ", "あ", "ぃ", "い", "ぅ", "う", "ぇ", "え", "ぉ", "お", "か", "が", "き", "ぎ", "く", "ぐ",
"け", "げ", "こ", "ご", "さ", "ざ", "し", "じ", "す", "ず", "せ", "ぜ", "そ", "ぞ", "た", "だ",
"ち", "ぢ", "っ", "つ", "づ", "て", "で", "と", "ど", "な", "に", "ぬ", "ね", "の", "は", "ば",
"ぱ", "ひ", "び", "ぴ", "ふ", "ぶ", "ぷ", "へ", "べ", "ぺ", "ほ", "ぼ", "ぽ", "ま", "み", "む",
"め", "も", "ゃ", "や", "ゅ", "ゆ", "ょ", "よ", "ら", "り", "る", "れ", "ろ", "ゎ", "わ", "を",
"ん", "ヴ", "ー"
};

static const char *halfKatakanaValue[] = {
"ｧ", "ｱ", "ｨ", "ｲ", "ｩ", "ｳ", "ｪ", "ｴ", "ｫ", "ｵ", "ｶ", "ｶﾞ", "ｷ", "ｷﾞ", "ｸ", "ｸﾞ",
"ｹ", "ｹﾞ", "ｺ", "ｺﾞ", "ｻ", "ｻﾞ", "ｼ", "ｼﾞ", "ｽ", "ｽﾞ", "ｾ", "ｾﾞ", "ｿ", "ｿﾞ", "ﾀ", "ﾀﾞ",
"ﾁ", "ﾁﾞ", "ｯ", "ﾂ", "ﾂﾞ", "ﾃ", "ﾃﾞ", "ﾄ", "ﾄﾞ", "ﾅ", "ﾆ", "ﾇ", "ﾈ", "ﾉ", "ﾊ", "ﾊﾞ",
"ﾊﾟ", "ﾋ", "ﾋﾞ", "ﾋﾟ", "ﾌ", "ﾌﾞ", "ﾌﾟ", "ﾍ", "ﾍﾞ", "ﾍﾟ", "ﾎ", "ﾎﾞ", "ﾎﾟ", "ﾏ", "ﾐ", "ﾑ",
"ﾒ", "ﾓ", "ｬ", "ﾔ", "ｭ", "ﾕ", "ｮ", "ﾖ", "ﾗ", "ﾘ", "ﾙ", "ﾚ", "ﾛ", "ﾜ", "ﾜ", "ｦ",
"ﾝ", "ｳﾞ", "ｰ"
};

static const char *fullKatakanaKey[] = {
"ぁ", "あ", "ぃ", "い", "ぅ", "う", "ぇ", "え", "ぉ", "お", "か", "が", "き", "ぎ", "く", "ぐ",
"け", "げ", "こ", "ご", "さ", "ざ", "し", "じ", "す", "ず", "せ", "ぜ", "そ", "ぞ", "た", "だ",
"ち", "ぢ", "っ", "つ", "づ", "て", "で", "と", "ど", "な", "に", "ぬ", "ね", "の", "は", "ば",
"ぱ", "ひ", "び", "ぴ", "ふ", "ぶ", "ぷ", "へ", "べ", "ぺ", "ほ", "ぼ", "ぽ", "ま", "み", "む",
"め", "も", "ゃ", "や", "ゅ", "ゆ", "ょ", "よ", "ら", "り", "る", "れ", "ろ", "ゎ", "わ", "を",
"ん", "ヴ", "ー"
};

static const char *fullKatakanaValue[] = {
"ァ", "ア", "ィ", "イ", "ゥ", "ウ", "ェ", "エ", "ォ", "オ", "カ", "ガ", "キ", "ギ", "ク", "グ",
"ケ", "ゲ", "コ", "ゴ", "サ", "ザ", "シ", "ジ", "ス", "ズ", "セ", "ゼ", "ソ", "ゾ", "タ", "ダ",
"チ", "ヂ", "ッ", "ツ", "ヅ", "テ", "デ", "ト", "ド", "ナ", "ニ", "ヌ", "ネ", "ノ", "ハ", "バ",
"パ", "ヒ", "ビ", "ピ", "フ", "ブ", "プ", "ヘ", "ベ", "ペ", "ホ", "ボ", "ポ", "マ", "ミ", "ム",
"メ", "モ", "ャ", "ヤ", "ュ", "ユ", "ョ", "ヨ", "ラ", "リ", "ル", "レ", "ロ", "ヮ", "ワ", "ヲ",
"ン", "ヴ", "ー"
};

static const char *fullAlphabetKey[] = {
"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P",
"Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f",
"g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v",
"w", "x", "y", "z"
};

static const char *fullAlphabetValue[] = {
"Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "Ｇ", "Ｈ", "Ｉ", "Ｊ", "Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ", "Ｐ",
"Ｑ", "Ｒ", "Ｓ", "Ｔ", "Ｕ", "Ｖ", "Ｗ", "Ｘ", "Ｙ", "Ｚ", "ａ", "ｂ", "ｃ", "ｄ", "ｅ", "ｆ",
"ｇ", "ｈ", "ｉ", "ｊ", "ｋ", "ｌ", "ｍ", "ｎ", "ｏ", "ｐ", "ｑ", "ｒ", "ｓ", "ｔ", "ｕ", "ｖ",
"ｗ", "ｘ", "ｙ", "ｚ"
};

static const WnnKeyMap halfKatakanaTable = WnnKeyMap(halfKatakanaKey,halfKatakanaValue,halfKatakanaLength);
static const WnnKeyMap fullKatakanaTable = WnnKeyMap(fullKatakanaKey,fullKatakanaValue,fullKatakanaLength);
static const WnnKeyMap fullAlphabetTableQwerty = WnnKeyMap(fullAlphabetKey,fullAlphabetValue,fullAlphabetLength);

static bool createCandidateString(const wstring &input, const WnnKeyMap &map, wstring &outBuf)
{
    outBuf.clear();
    for (int index = 0, length = input.length(); index < length; index++) {
        wstring out = map.value(input.substr(index, 1));
        if (out.empty())
            return false;
        outBuf.append(out);
    }
    return true;
}

static wstring convertCaps(const wstring &moji)
{
    wstring tmp;
    if (!moji.empty()) {
        tmp.append(toUpper(moji.substr(0,1)));
        tmp.append(toLower(moji.substr(1)));
    }
    return tmp;
}

KanaConverter::KanaConverter() {}

KanaConverter::~KanaConverter(){}

void KanaConverter::createPseudoCandidateListForQwerty(vector<WnnWord> &list, const wstring &inputHiragana, const wstring &inputRomaji)
{
    /* Create pseudo candidates for half width alphabet */
    wstring convHanEijiLower = toLower(inputRomaji);
    list.push_back(WnnWord(inputRomaji, inputHiragana, mPosDefault));
    list.push_back(WnnWord(convHanEijiLower, inputHiragana, mPosSymbol));
    list.push_back(WnnWord(convertCaps(convHanEijiLower), inputHiragana, mPosSymbol));
    list.push_back(WnnWord(toUpper(inputRomaji), inputHiragana, mPosSymbol));

    /* Create pseudo candidates for the full width alphabet */
    wstring convZenEiji;
    if (createCandidateString(inputRomaji, fullAlphabetTableQwerty, convZenEiji)) {
        wstring convZenEijiLower = toLower(convZenEiji);
        list.push_back(WnnWord(convZenEiji, inputHiragana, mPosSymbol));
        list.push_back(WnnWord(convZenEijiLower, inputHiragana, mPosSymbol));
        list.push_back(WnnWord(convertCaps(convZenEijiLower), inputHiragana, mPosSymbol));
        list.push_back(WnnWord(toUpper(convZenEiji), inputHiragana, mPosSymbol));
    }
}

void KanaConverter::setDictionary(OpenWnnDictionary *dict)
{
    /* get part of speech tags */
    mPosDefault  = dict->getPOS(OpenWnnDictionary::POS_TYPE_MEISI);
    mPosNumber   = dict->getPOS(OpenWnnDictionary::POS_TYPE_SUUJI);
    mPosSymbol   = dict->getPOS(OpenWnnDictionary::POS_TYPE_KIGOU);
}

vector<WnnWord> KanaConverter::createPseudoCandidateList(const wstring &inputHiragana, const wstring &inputRomaji)
{
    vector<WnnWord> list;

    if (inputHiragana.length() == 0) {
        return list;
    }

    /* Create pseudo candidates for all keyboard type */
    /* Hiragana(reading) / Full width katakana / Half width katakana */
    list.push_back(WnnWord(inputHiragana, inputHiragana));
    wstring stringBuff;
    if (createCandidateString(inputHiragana, fullKatakanaTable, stringBuff)) {
        list.push_back(WnnWord(stringBuff, inputHiragana, mPosDefault));
    }
    if (createCandidateString(inputHiragana, halfKatakanaTable, stringBuff)) {
        list.push_back(WnnWord(stringBuff, inputHiragana, mPosDefault));
    }

    /* Create pseudo candidates for Qwerty keyboard */
    createPseudoCandidateListForQwerty(list, inputHiragana, inputRomaji);
    return list;
}
