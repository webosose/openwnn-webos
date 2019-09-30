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

#include "openwnnenginejajp.h"
#include "openwnndictionary.h"
#include "openwnnclauseconverter.h"
#include "wnnword.h"
#include "kanaconverter.h"
#include <map>

class OpenWnnEngineJAJPPrivate
{
public:
    OpenWnnEngineJAJPPrivate() :
        mDictType(OpenWnnEngineJAJP::DIC_LANG_INIT),
        mKeyboardType(OpenWnnEngineJAJP::KEYBOARD_QWERTY),
        mOutputNum(0),
        mGetCandidateFrom(0),
        mExactMatchMode(false),
        mSingleClauseMode(false)
    {
        /* clear dictionary settings */
        mDictionaryJP.clearDictionary();
        mDictionaryJP.clearApproxPattern();

        mClauseConverter.setDictionary(&mDictionaryJP);
        mKanaConverter.setDictionary(&mDictionaryJP);
        initReplaceTable();
    }
    //Dakuon & Handakuon replacement.
    void initReplaceTable()
    {
        //Dakuon
        mDakuonTable[L"か"] = L"が";
        mDakuonTable[L"が"] = L"か";
        mDakuonTable[L"さ"] = L"ざ";
        mDakuonTable[L"ざ"] = L"さ";
        mDakuonTable[L"た"] = L"だ";
        mDakuonTable[L"だ"] = L"た";
        mDakuonTable[L"は"] = L"ば";
        mDakuonTable[L"ば"] = L"は";
        mDakuonTable[L"き"] = L"ぎ";
        mDakuonTable[L"ぎ"] = L"き";
        mDakuonTable[L"し"] = L"じ";
        mDakuonTable[L"じ"] = L"し";
        mDakuonTable[L"ち"] = L"ぢ";
        mDakuonTable[L"ぢ"] = L"ち";
        mDakuonTable[L"ひ"] = L"び";
        mDakuonTable[L"び"] = L"ひ";
        mDakuonTable[L"く"] = L"ぐ";
        mDakuonTable[L"ぐ"] = L"く";
        mDakuonTable[L"す"] = L"ず";
        mDakuonTable[L"ず"] = L"す";
        mDakuonTable[L"つ"] = L"づ";
        mDakuonTable[L"づ"] = L"つ";
        mDakuonTable[L"ふ"] = L"ぶ";
        mDakuonTable[L"ぶ"] = L"ふ";
        mDakuonTable[L"け"] = L"げ";
        mDakuonTable[L"げ"] = L"け";
        mDakuonTable[L"せ"] = L"ぜ";
        mDakuonTable[L"ぜ"] = L"せ";
        mDakuonTable[L"て"] = L"で";
        mDakuonTable[L"で"] = L"て";
        mDakuonTable[L"へ"] = L"べ";
        mDakuonTable[L"べ"] = L"へ";
        mDakuonTable[L"ヘ"] = L"ベ";
        mDakuonTable[L"ベ"] = L"ヘ";
        mDakuonTable[L"こ"] = L"ご";
        mDakuonTable[L"ご"] = L"こ";
        mDakuonTable[L"そ"] = L"ぞ";
        mDakuonTable[L"ぞ"] = L"そ";
        mDakuonTable[L"と"] = L"ど";
        mDakuonTable[L"ど"] = L"と";
        mDakuonTable[L"ほ"] = L"ぼ";
        mDakuonTable[L"ぼ"] = L"ほ";
        mDakuonTable[L"ウ"] = L"ヴ";
        mDakuonTable[L"ヴ"] = L"ウ";
        mDakuonTable[L"う"] = L"ゔ";
        mDakuonTable[L"ゔ"] = L"う";
        mDakuonTable[L"カ"] = L"ガ";
        mDakuonTable[L"ガ"] = L"カ";
        mDakuonTable[L"キ"] = L"ギ";
        mDakuonTable[L"ギ"] = L"キ";
        mDakuonTable[L"ク"] = L"グ";
        mDakuonTable[L"グ"] = L"ク";
        mDakuonTable[L"ケ"] = L"ゲ";
        mDakuonTable[L"ゲ"] = L"ケ";
        mDakuonTable[L"コ"] = L"ゴ";
        mDakuonTable[L"ゴ"] = L"コ";
        mDakuonTable[L"サ"] = L"ザ";
        mDakuonTable[L"ザ"] = L"サ";
        mDakuonTable[L"シ"] = L"ジ";
        mDakuonTable[L"ジ"] = L"シ";
        mDakuonTable[L"ス"] = L"ズ";
        mDakuonTable[L"ズ"] = L"ス";
        mDakuonTable[L"セ"] = L"ゼ";
        mDakuonTable[L"ゼ"] = L"セ";
        mDakuonTable[L"ソ"] = L"ゾ";
        mDakuonTable[L"ゾ"] = L"ソ";
        mDakuonTable[L"タ"] = L"ダ";
        mDakuonTable[L"ダ"] = L"タ";
        mDakuonTable[L"チ"] = L"ヂ";
        mDakuonTable[L"ヂ"] = L"チ";
        mDakuonTable[L"ツ"] = L"ヅ";
        mDakuonTable[L"ヅ"] = L"ツ";
        mDakuonTable[L"テ"] = L"デ";
        mDakuonTable[L"デ"] = L"テ";
        mDakuonTable[L"ト"] = L"ド";
        mDakuonTable[L"ド"] = L"ト";
        mDakuonTable[L"ハ"] = L"バ";
        mDakuonTable[L"バ"] = L"ハ";
        mDakuonTable[L"ヒ"] = L"ビ";
        mDakuonTable[L"ビ"] = L"ヒ";
        mDakuonTable[L"フ"] = L"ブ";
        mDakuonTable[L"ブ"] = L"フ";
        mDakuonTable[L"ホ"] = L"ボ";
        mDakuonTable[L"ボ"] = L"ホ";

        //Handakuon
        mHandakuonTable[L"は"] = L"ぱ";
        mHandakuonTable[L"ぱ"] = L"は";
        mHandakuonTable[L"ひ"] = L"ぴ";
        mHandakuonTable[L"ぴ"] = L"ひ";
        mHandakuonTable[L"ふ"] = L"ぷ";
        mHandakuonTable[L"ぷ"] = L"ふ";
        mHandakuonTable[L"へ"] = L"ぺ";
        mHandakuonTable[L"ぺ"] = L"へ";
        mHandakuonTable[L"ヘ"] = L"ペ";
        mHandakuonTable[L"ペ"] = L"ヘ";
        mHandakuonTable[L"ほ"] = L"ぽ";
        mHandakuonTable[L"ぽ"] = L"ほ";
        mHandakuonTable[L"ハ"] = L"パ";
        mHandakuonTable[L"パ"] = L"ハ";
        mHandakuonTable[L"ヒ"] = L"ピ";
        mHandakuonTable[L"ピ"] = L"ヒ";
        mHandakuonTable[L"フ"] = L"プ";
        mHandakuonTable[L"プ"] = L"フ";
        mHandakuonTable[L"ホ"] = L"ポ";
        mHandakuonTable[L"ポ"] = L"ホ";
    }

    wstring getDakuon(wstring c) {
        if (mDakuonTable.count(c))
            return mDakuonTable[c];
        return L"";
    }

    wstring getHandakuon(wstring c) {
        if (mHandakuonTable.count(c))
            return mHandakuonTable[c];
        return L"";
    }

    void setDictionaryForPrediction(int strlen)
    {
        OpenWnnDictionary &dict = mDictionaryJP;

        dict.clearDictionary();

        if (mDictType != OpenWnnEngineJAJP::DIC_LANG_JP_EISUKANA) {
            dict.clearApproxPattern();
            if (strlen == 0) {
                dict.setDictionary(2, 245, 245);
                dict.setDictionary(3, 100, 244);

                dict.setDictionary(OpenWnnDictionary::INDEX_LEARN_DICTIONARY, OpenWnnEngineJAJP::FREQ_LEARN, OpenWnnEngineJAJP::FREQ_LEARN);
            } else {
                dict.setDictionary(0, 100, 400);
                if (strlen > 1) {
                    dict.setDictionary(1, 100, 400);
                }
                dict.setDictionary(2, 245, 245);
                dict.setDictionary(3, 100, 244);

                dict.setDictionary(OpenWnnDictionary::INDEX_USER_DICTIONARY, OpenWnnEngineJAJP::FREQ_USER, OpenWnnEngineJAJP::FREQ_USER);
                dict.setDictionary(OpenWnnDictionary::INDEX_LEARN_DICTIONARY, OpenWnnEngineJAJP::FREQ_LEARN, OpenWnnEngineJAJP::FREQ_LEARN);
                if (mKeyboardType != OpenWnnEngineJAJP::KEYBOARD_QWERTY) {
                    dict.setApproxPattern(OpenWnnDictionary::APPROX_PATTERN_JAJP_12KEY_NORMAL);
                }
            }
        }
    }

    shared_ptr<WnnWord> getCandidate(int index)
    {
        shared_ptr<WnnWord> word;

        if (mGetCandidateFrom == 0) {
            if (mDictType == OpenWnnEngineJAJP::DIC_LANG_JP_EISUKANA) {
                /* skip to Kana conversion if EISU-KANA conversion mode */
                mGetCandidateFrom = 2;
            } else if (mSingleClauseMode) {
                /* skip to single clause conversion if single clause conversion mode */
                mGetCandidateFrom = 1;
            } else {
                if (mConvResult.size() < OpenWnnEngineJAJP::PREDICT_LIMIT) {
                    /* get prefix matching words from the dictionaries */
                    while (index >= mConvResult.size()) {
                        if ((word = mDictionaryJP.getNextWord()) == NULL) {
                            mGetCandidateFrom = 1;
                            break;
                        }
                        if (!mExactMatchMode || mInputHiragana.compare(word->stroke) == 0) {
                            addCandidate(word);
                            if (mConvResult.size() >= OpenWnnEngineJAJP::PREDICT_LIMIT) {
                                mGetCandidateFrom = 1;
                                break;
                            }
                        }
                    }
                } else {
                    mGetCandidateFrom = 1;
                }
            }
        }

        /* get candidates by single clause conversion */
        if (mGetCandidateFrom == 1) {
            vector<WnnClause> convResult = mClauseConverter.convert(mInputHiragana);
            if (!convResult.empty()) {
                for (vector<WnnClause>::const_iterator it = convResult.begin();
                     it != convResult.end(); it++) {
                    addCandidate(make_shared<WnnWord>(*it));
                }
            }
            /* end of candidates by single clause conversion */
            mGetCandidateFrom = 2;
        }

        /* get candidates from Kana converter */
        if (mGetCandidateFrom == 2) {
            vector<WnnWord> addCandidateList = mKanaConverter.createPseudoCandidateList(mInputHiragana, mInputRomaji);

            for (vector<WnnWord>::const_iterator it = addCandidateList.begin();
                 it != addCandidateList.end(); it++) {
                addCandidate(make_shared<WnnWord>(*it));
            }

            mGetCandidateFrom = 3;
        }

        if (index >= mConvResult.size()) {
            return 0;
        }
        return mConvResult.at(index);
    }

    bool addCandidate(shared_ptr<WnnWord> word)
    {
        if (!word || word->candidate.empty() || mCandTable.count(word->candidate) > 0
                || word->candidate.length() > OpenWnnEngineJAJP::MAX_OUTPUT_LENGTH) {
            return false;
        }
        mCandTable.insert({word->candidate, word});
        mConvResult.push_back(word);
        return true;
    }

    void clearCandidates()
    {
        mConvResult.clear();
        mCandTable.clear();
        mOutputNum = 0;
        mInputHiragana.clear();
        mInputRomaji.clear();
        mGetCandidateFrom = 0;
        mSingleClauseMode = false;
    }

    int setSearchKey(const ComposingText &text, int maxLen)
    {
        wstring input = text.toString(ComposingText::LAYER1);
        if (0 <= maxLen && maxLen <= input.length()) {
            input = input.substr(0, maxLen);
            mExactMatchMode = true;
        } else {
            mExactMatchMode = false;
        }

        if (input.length() == 0) {
            mInputHiragana = L"";
            mInputRomaji = L"";
            return 0;
        }

        mInputHiragana = input;
        mInputRomaji = text.toString(ComposingText::LAYER0);

        return input.length();
    }

    void clearPreviousWord()
    {
        mPreviousWord.reset();
    }

    OpenWnnEngineJAJP::DictionaryType mDictType;
    OpenWnnEngineJAJP::KeyboardType mKeyboardType;
    OpenWnnDictionary mDictionaryJP;
    vector<shared_ptr<WnnWord> > mConvResult;
    map<wstring, shared_ptr<WnnWord> > mCandTable;
    wstring mInputHiragana;
    wstring mInputRomaji;
    int mOutputNum;
    int mGetCandidateFrom;
    shared_ptr<WnnWord> mPreviousWord;
    OpenWnnClauseConverter mClauseConverter;
    KanaConverter mKanaConverter;
    bool mExactMatchMode;
    bool mSingleClauseMode;
    shared_ptr<WnnSentence> mConvertSentence;
    map<wstring,wstring> mDakuonTable;
    map<wstring,wstring> mHandakuonTable;
};

OpenWnnEngineJAJP::OpenWnnEngineJAJP()
{
    p.reset(new OpenWnnEngineJAJPPrivate());
}

OpenWnnEngineJAJP::~OpenWnnEngineJAJP()
{
}

bool OpenWnnEngineJAJP::setDictionary(DictionaryType type)
{
    p->mDictType = type;
    return true;
}

wstring OpenWnnEngineJAJP::getDakuon(wstring c) {
    return p->getDakuon(c);
}

wstring OpenWnnEngineJAJP::getHandakuon(wstring c) {
    return p->getHandakuon(c);
}

int OpenWnnEngineJAJP::predict(const ComposingText &text, int minLen, int maxLen)
{
    p->clearCandidates();

    /* set mInputHiragana and mInputRomaji */
    int len = p->setSearchKey(text, maxLen);

    /* set dictionaries by the length of input */
    p->setDictionaryForPrediction(len);

    /* search dictionaries */
    p->mDictionaryJP.setInUseState(true);

    if (len == 0) {
        /* search by previously selected word */
        if (!p->mPreviousWord)
            return -1;
        return p->mDictionaryJP.searchWord(OpenWnnDictionary::SEARCH_LINK, OpenWnnDictionary::ORDER_BY_FREQUENCY,
                                           p->mInputHiragana, *p->mPreviousWord);
    } else {
        if (p->mExactMatchMode) {
            /* exact matching */
            p->mDictionaryJP.searchWord(OpenWnnDictionary::SEARCH_EXACT, OpenWnnDictionary::ORDER_BY_FREQUENCY,
                                        p->mInputHiragana);
        } else {
            /* prefix matching */
            p->mDictionaryJP.searchWord(OpenWnnDictionary::SEARCH_PREFIX, OpenWnnDictionary::ORDER_BY_FREQUENCY,
                                        p->mInputHiragana);
        }
        return 1;
    }
}

int OpenWnnEngineJAJP::convert(ComposingText &text)
{
    p->clearCandidates();

    p->mDictionaryJP.setInUseState(true);

    int cursor = text.getCursor(ComposingText::LAYER1);
    wstring input;
    shared_ptr<WnnClause> head;
    if (cursor > 0) {
        /* convert previous part from cursor */
        input = text.toString(ComposingText::LAYER1, 0, cursor - 1);
        vector<WnnClause> headCandidates = p->mClauseConverter.convert(input);
        if (headCandidates.empty()) {
            return 0;
        }
        head = make_shared<WnnClause>(input, headCandidates.front());

        /* set the rest of input string */
        input = text.toString(ComposingText::LAYER1, cursor, text.size(ComposingText::LAYER1) - 1);
    } else {
        /* set whole of input string */
        input = text.toString(ComposingText::LAYER1);
    }

    shared_ptr<WnnSentence> sentence;
    if (input.length() != 0) {
        sentence = p->mClauseConverter.consecutiveClauseConvert(input);
    }
    if (head) {
        sentence = make_shared<WnnSentence>(*head, sentence.get());
    }
    if (!sentence) {
        return 0;
    }

    vector<StrSegment> ss;
    int pos = 0;
    for (vector<WnnClause>::const_iterator it = sentence->elements.begin();
         it != sentence->elements.end(); it++) {
        const WnnClause &clause = *it;
        int len = clause.stroke.length();
        ss.push_back(StrSegment(clause, pos, pos + len - 1));
        pos += len;
    }
    text.setCursor(ComposingText::LAYER2, text.size(ComposingText::LAYER2));
    text.replaceStrSegment(ComposingText::LAYER2, ss,
                           text.getCursor(ComposingText::LAYER2));
    p->mConvertSentence = sentence;
    return 0;
}

shared_ptr<WnnWord> OpenWnnEngineJAJP::getNextCandidate()
{
    if (p->mInputHiragana.empty()) {
        return 0;
    }
    shared_ptr<WnnWord> word = p->getCandidate(p->mOutputNum);
    if (word) {
        p->mOutputNum++;
    }
    return word;
}

bool OpenWnnEngineJAJP::learn(WnnWord &word)
{
    int ret = -1;
    if (word.partOfSpeech.right == 0) {
        word.partOfSpeech = p->mDictionaryJP.getPOS(OpenWnnDictionary::POS_TYPE_MEISI);
    }

    OpenWnnDictionary &dict = p->mDictionaryJP;
    if (word.isSentence()) {
        const WnnSentence *sentence = static_cast<const WnnSentence *>(&word);
        for (vector<WnnClause>::const_iterator clauses = sentence->elements.begin();
             clauses != sentence->elements.end(); clauses++) {
            const WnnWord &wd = *clauses;
            ret = dict.learnWord(wd, p->mPreviousWord.get());
            p->mPreviousWord.reset(static_cast<WnnWord*>(new WnnSentence(*sentence)));
            if (ret != 0) {
                break;
            }
        }
    } else {
        ret = dict.learnWord(word, p->mPreviousWord.get());
        p->mPreviousWord = make_shared<WnnWord>(word);
        p->mClauseConverter.setDictionary(&dict);
    }

    return (ret == 0);
}


void OpenWnnEngineJAJP::breakSequence()
{
    p->clearPreviousWord();
}

int OpenWnnEngineJAJP::makeCandidateListOf(int clausePosition)
{
    p->clearCandidates();

    if ((p->mConvertSentence == NULL) || (p->mConvertSentence->elements.size() <= clausePosition)) {
        return 0;
    }
    p->mSingleClauseMode = true;
    const WnnClause &clause = p->mConvertSentence->elements.at(clausePosition);
    p->mInputHiragana = clause.stroke;
    p->mInputRomaji = clause.candidate;

    return 1;
}



