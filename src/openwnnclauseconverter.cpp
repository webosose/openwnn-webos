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

#include "openwnnclauseconverter.h"
#include "openwnndictionary.h"
#include "wnnword.h"
#include <map>

class OpenWnnClauseConverterPrivate
{
public:
    OpenWnnClauseConverterPrivate() : mDictionary(nullptr)
    { }

    bool singleClauseConvert(vector<WnnClause> &clauseList, const wstring &input,
                             const WnnPOS &terminal, bool all)
    {
        bool ret = false;

        /* get clauses without ancillary word */
        vector<WnnWord> stems = getIndependentWords(input, all);
        if (!stems.empty()) {
            for (vector<WnnWord>::const_iterator stemsi = stems.begin();
                 stemsi != stems.end(); stemsi++) {
                const WnnWord &stem = *stemsi;
                if (addClause(clauseList, input, stem, NULL, terminal, all)) {
                    ret = true;
                }
            }
        }

        /* get clauses with ancillary word */
        int max = CLAUSE_COST * 2;
        for (int split = 1; split < input.length(); split++) {
            /* get ancillary patterns */
            wstring str = input.substr(split);
            vector<WnnWord> fzks = getAncillaryPattern(str);
            if (fzks.empty()) {
                continue;
            }

            /* get candidates of stem in a clause */
            str = input.substr(0, split);
            stems = getIndependentWords(str, all);
            if (stems.empty()) {
                if (mDictionary->searchWord(OpenWnnDictionary::SEARCH_PREFIX, OpenWnnDictionary::ORDER_BY_FREQUENCY, str) <= 0) {
                    break;
                } else {
                    continue;
                }
            }
            /* make clauses */
            for (vector<WnnWord>::const_iterator stemsi = stems.begin();
                 stemsi != stems.end(); stemsi++) {
                const WnnWord &stem = *stemsi;
                if (all || stem.frequency > max) {
                    for (vector<WnnWord>::const_iterator fzksi = fzks.begin();
                         fzksi != fzks.end(); fzksi++) {
                        const WnnWord &fzk = *fzksi;
                        if (addClause(clauseList, input, stem, &fzk, terminal, all)) {
                            ret = true;
                            max = stem.frequency;
                        }
                    }
                }
            }
        }
        return ret;
    }

    bool addClause(vector<WnnClause> &clauseList, const wstring &input,
                   const WnnWord &stem, const WnnWord *fzk, const WnnPOS &terminal, bool all)
    {
        shared_ptr<WnnClause> clause;
        /* check if the part of speech is valid */
        if (fzk == NULL) {
            if (connectible(stem.partOfSpeech.right, terminal.left)) {
                clause = make_shared<WnnClause>(input, stem);
            }
        } else {
            if (connectible(stem.partOfSpeech.right, fzk->partOfSpeech.left)
                && connectible(fzk->partOfSpeech.right, terminal.left)) {
                clause =  make_shared<WnnClause>(input, stem, *fzk);
            }
        }
        if (clause == NULL) {
            return false;
        }

        /* store to the list */
        if (clauseList.empty()) {
            /* add if the list is empty */
            clauseList.push_back(*clause);
            return true;
        } else {
            if (!all) {
                /* reserve only the best clause */
                const WnnClause &best = clauseList.front();
                if (best.frequency < clause->frequency) {
                    clauseList.insert(clauseList.begin(), *clause);
                    return true;
                }
            } else {
                /* reserve all clauses */
                vector<WnnClause>::iterator clauseListi;
                for (clauseListi = clauseList.begin(); clauseListi != clauseList.end(); clauseListi++) {
                    const WnnClause &clausei = *clauseListi;
                    if (clausei.frequency < clause->frequency) {
                        break;
                    }
                }
                clauseList.insert(clauseListi, *clause);
                return true;
            }
        }
        return false;
    }

    bool connectible(int right, int left)
    {
        return left < mConnectMatrix.size() && right < mConnectMatrix.at(left).size() && mConnectMatrix.at(left)[right];
    }

    vector<WnnWord> getAncillaryPattern(const wstring &input)
    {
        if (input.length() == 0) {
            return vector<WnnWord>();
        }

        if (mFzkPatterns.count(input) > 0) {
            return mFzkPatterns[input];
        }

        /* set dictionaries */
        OpenWnnDictionary *dict = mDictionary;
        dict->clearDictionary();
        dict->clearApproxPattern();
        dict->setDictionary(6, 400, 500);

        for (int start = input.length() - 1; start >= 0; start--) {
            wstring key = input.substr(start);

            if (mFzkPatterns.count(key) > 0) {
                continue;
            }

            vector<WnnWord> fzks;

            /* search ancillary words */
            dict->searchWord(OpenWnnDictionary::SEARCH_EXACT, OpenWnnDictionary::ORDER_BY_FREQUENCY, key);
            shared_ptr<WnnWord> word;
            while ((word = dict->getNextWord()) != NULL) {
                fzks.push_back(*word);
            }

            /* concatenate sequence of ancillary words */
            for (int end = input.length() - 1; end > start; end--) {
                wstring followKey = input.substr(end);
                if (mFzkPatterns.count(followKey) == 0 )
                    continue;
                vector<WnnWord> &followFzks = mFzkPatterns[followKey];
                if (followFzks.empty())
                    continue;
                dict->searchWord(OpenWnnDictionary::SEARCH_EXACT, OpenWnnDictionary::ORDER_BY_FREQUENCY, input.substr(start, end - start));
                while ((word = dict->getNextWord()) != NULL) {
                    for (vector<WnnWord>::const_iterator followFzksi = followFzks.begin();
                         followFzksi != followFzks.end(); followFzksi++) {
                        const WnnWord &follow = *followFzksi;
                        if (connectible(word->partOfSpeech.right, follow.partOfSpeech.left)) {
                            fzks.push_back(WnnWord(key, key, WnnPOS(word->partOfSpeech.left, follow.partOfSpeech.right)));
                        }
                    }
                }
            }

            mFzkPatterns[key] = fzks;
        }
        return mFzkPatterns[input];
    }

    vector<WnnWord> getIndependentWords(const wstring &input, bool all)
    {
        if (input.length() == 0)
            return vector<WnnWord>();

        map<wstring, vector<WnnWord>> &wordBag = all ? mAllIndepWordBag : mIndepWordBag;
        if (wordBag.count(input) == 0) {
            vector<WnnWord> words;

            /* set dictionaries */
            OpenWnnDictionary *dict = mDictionary;
            dict->clearDictionary();
            dict->clearApproxPattern();
            dict->setDictionary(4, 0, 10);
            dict->setDictionary(5, 400, 500);
            dict->setDictionary(OpenWnnDictionary::INDEX_USER_DICTIONARY, FREQ_USER, FREQ_USER);
            dict->setDictionary(OpenWnnDictionary::INDEX_LEARN_DICTIONARY, FREQ_LEARN, FREQ_LEARN);

            shared_ptr<WnnWord> word;
            if (all) {
                dict->searchWord(OpenWnnDictionary::SEARCH_EXACT, OpenWnnDictionary::ORDER_BY_FREQUENCY, input);
                /* store all words */
                while ((word = dict->getNextWord())) {
                    if (input.compare(word->stroke) == 0) {
                        words.push_back(*word);
                    }
                }
            } else {
                dict->searchWord(OpenWnnDictionary::SEARCH_EXACT, OpenWnnDictionary::ORDER_BY_FREQUENCY, input);
                /* store a word which has an unique part of speech tag */
                while ((word = dict->getNextWord()) != NULL) {
                    if (input.compare(word->stroke) == 0) {
                        bool found = false;
                        for (vector<WnnWord>::const_iterator list = words.begin();
                             list != words.end(); list++) {
                            const WnnWord &w = *list;
                            if (w.partOfSpeech.right == word->partOfSpeech.right) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            words.push_back(*word);
                        }
                        if (word->frequency < 400) {
                            break;
                        }
                    }
                }
            }
            addAutoGeneratedCandidates(input, words, all);
            wordBag[input] = words;
        }
        return wordBag[input];
    }

    void addAutoGeneratedCandidates(const wstring &input, vector<WnnWord> &wordList, bool all)
    {
        wordList.push_back(WnnWord(input, input, mPosDefault, (CLAUSE_COST - 1) * input.length()));
    }

    WnnClause defaultClause(const wstring &input)
    {
        return WnnClause(input, input, mPosDefault, (CLAUSE_COST - 1) * input.length());
    }

    /** Score(frequency value) of word in the learning dictionary */
    static const int FREQ_LEARN = 600;
    /** Score(frequency value) of word in the user dictionary */
    static const int FREQ_USER = 500;

    /** Maximum limit length of input */
    static const int MAX_INPUT_LENGTH = 50;

    /** search cache for unique independent words (jiritsugo) */
    map<wstring, vector<WnnWord> > mIndepWordBag;
    /** search cache for all independent words (jiritsugo) */
    map<wstring, vector<WnnWord> > mAllIndepWordBag;
    /** search cache for ancillary words (fuzokugo) */
    map<wstring, vector<WnnWord> > mFzkPatterns;

    /** connect matrix for generating a clause */
    vector<vector<bool>> mConnectMatrix;

    /** dictionaries */
    OpenWnnDictionary* mDictionary;

    /** part of speech (default) */
    WnnPOS mPosDefault;
    /** part of speech (end of clause/not end of sentence) */
    WnnPOS mPosEndOfClause1;
    /** part of speech (end of clause/any place) */
    WnnPOS mPosEndOfClause2;
    /** part of speech (end of sentence) */
    WnnPOS mPosEndOfClause3;

    /** cost value of a clause */
    static const int CLAUSE_COST = -1000;
};

OpenWnnClauseConverter::OpenWnnClauseConverter()
{
    p.reset(new OpenWnnClauseConverterPrivate());
}

OpenWnnClauseConverter::~OpenWnnClauseConverter()
{
}

void OpenWnnClauseConverter::setDictionary(OpenWnnDictionary *dict)
{
    /* get connect matrix */
    p->mConnectMatrix = dict->getConnectMatrix();

    /* clear dictionary settings */
    p->mDictionary = dict;
    dict->clearDictionary();
    dict->clearApproxPattern();

    /* clear work areas */
    p->mIndepWordBag.clear();
    p->mAllIndepWordBag.clear();
    p->mFzkPatterns.clear();

    /* get part of speech tags */
    p->mPosDefault      = dict->getPOS(OpenWnnDictionary::POS_TYPE_MEISI);
    p->mPosEndOfClause1 = dict->getPOS(OpenWnnDictionary::POS_TYPE_V1);
    p->mPosEndOfClause2 = dict->getPOS(OpenWnnDictionary::POS_TYPE_V2);
    p->mPosEndOfClause3 = dict->getPOS(OpenWnnDictionary::POS_TYPE_V3);
}

vector<WnnClause> OpenWnnClauseConverter::convert(const wstring &input)
{
    vector<WnnClause> convertResult;

    /* do nothing if no dictionary is specified */
    if (p->mConnectMatrix.empty() || p->mDictionary == NULL)
        return convertResult;

    /* do nothing if the length of input exceeds the limit */
    if (input.length() > OpenWnnClauseConverterPrivate::MAX_INPUT_LENGTH)
        return convertResult;

    /* try single clause conversion */
    p->singleClauseConvert(convertResult, input, p->mPosEndOfClause2, true);

    return convertResult;
}

shared_ptr<WnnSentence> OpenWnnClauseConverter::consecutiveClauseConvert(const wstring &input)
{
    vector<WnnClause> clauses;

    /* clear the cache which is not matched */
    vector<shared_ptr<WnnSentence> > sentence;
    for (int i = 0; i < input.length(); i++) {
        sentence.push_back(0);
    }
      /* consecutive clause conversion */
    for (int start = 0; start < input.length(); start++) {
        if (start != 0 && sentence[start - 1] == NULL) {
            continue;
        }

        /* limit the length of a clause */
        int end = input.length();
        if (end > start + 20) {
            end = start + 20;
        }
        /* make clauses */
        for ( ; end > start; end--) {
            int idx = end - 1;

            /* cutting a branch */
            if (sentence[idx] != NULL) {
                if (start != 0) {
                    if (sentence[idx]->frequency > sentence[start - 1]->frequency + OpenWnnClauseConverterPrivate::CLAUSE_COST + OpenWnnClauseConverterPrivate::FREQ_LEARN) {
                        /* there may be no way to be the best sequence from the 'start' */
                        break;
                    }
                } else {
                    if (sentence[idx]->frequency > OpenWnnClauseConverterPrivate::CLAUSE_COST + OpenWnnClauseConverterPrivate::FREQ_LEARN) {
                        /* there may be no way to be the best sequence from the 'start' */
                        break;
                    }
                }
            }

            wstring key = input.substr(start, end - start);
            clauses.clear();
            if (end == input.length()) {
                /* get the clause which can be the end of the sentence */
                p->singleClauseConvert(clauses, key, p->mPosEndOfClause1, false);
            } else {
                /* get the clause which is not the end of the sentence */
                p->singleClauseConvert(clauses, key, p->mPosEndOfClause3, false);
            }
            WnnClause bestClause(clauses.empty() ? p->defaultClause(key) : clauses.front());

            /* make a sub-sentence */
            shared_ptr<WnnSentence> ws(start == 0 ? new WnnSentence(key, bestClause) : new WnnSentence(*sentence[start - 1], bestClause));
            ws->frequency += OpenWnnClauseConverterPrivate::CLAUSE_COST;

            /* update the best sub-sentence on the cache buffer */
            if (sentence[idx] == NULL || (sentence[idx]->frequency < ws->frequency)) {
                sentence[idx] = ws;
            }
        }
    }

    /* return the result of the consecutive clause conversion */
    if (sentence[input.length() - 1] != NULL) {
        return sentence[input.length() - 1];
    }
    return 0;
}

