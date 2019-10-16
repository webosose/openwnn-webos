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

#ifndef WNNWORD_H
#define WNNWORD_H

#include <string>
#include <vector>

using namespace std;

class WnnPOS
{
public:
    WnnPOS() :
        left(0),
        right(0)
    { }

    WnnPOS(int left, int right) :
        left(left),
        right(right)
    { }

    /** The part of speech at left side */
    int left;

    /** The part of speech at right side */
    int right;
};

class WnnWord
{
public:
    WnnWord() :
        id(0),
        candidate(),
        stroke(),
        frequency(0),
        partOfSpeech(),
        attribute(0)
    { }

    WnnWord(const wstring &candidate, const wstring &stroke) :
        id(0),
        candidate(candidate),
        stroke(stroke),
        frequency(0),
        partOfSpeech(),
        attribute(0)
    { }

    WnnWord(const wstring &candidate, const wstring &stroke, int frequency) :
        id(0),
        candidate(candidate),
        stroke(stroke),
        frequency(frequency),
        partOfSpeech(),
        attribute(0)
    { }

    WnnWord(const wstring &candidate, const wstring &stroke, const WnnPOS &posTag) :
        id(0),
        candidate(candidate),
        stroke(stroke),
        frequency(0),
        partOfSpeech(posTag),
        attribute(0)
    { }

    WnnWord(const wstring &candidate, const wstring &stroke, const WnnPOS &posTag, int frequency) :
        id(0),
        candidate(candidate),
        stroke(stroke),
        frequency(frequency),
        partOfSpeech(posTag),
        attribute(0)
    { }

    WnnWord(int id, const wstring &candidate, const wstring &stroke, const WnnPOS &posTag, int frequency) :
        id(id),
        candidate(candidate),
        stroke(stroke),
        frequency(frequency),
        partOfSpeech(posTag),
        attribute(0)
    { }

    WnnWord(int id, const wstring &candidate, const wstring &stroke, const WnnPOS &posTag, int frequency, int attribute) :
        id(id),
        candidate(candidate),
        stroke(stroke),
        frequency(frequency),
        partOfSpeech(posTag),
        attribute(attribute)
    { }

    virtual ~WnnWord()
    { }

    virtual bool isClause() const
    {
        return false;
    }

    virtual bool isSentence() const
    {
        return false;
    }

    int      id;
    wstring   candidate;
    wstring   stroke;
    int      frequency;
    WnnPOS   partOfSpeech;
    int      attribute;
};

class WnnClause : public WnnWord
{
public:

    WnnClause(const WnnClause& copy):
        WnnWord(copy.id, copy.candidate, copy.stroke, copy.partOfSpeech, copy.frequency, copy.attribute)
    { }

    WnnClause(const wstring &candidate, const wstring &stroke, const WnnPOS &posTag, int frequency) :
        WnnWord(candidate, stroke, posTag, frequency)
    { }

    WnnClause(const wstring &stroke, const WnnWord& stem) :
        WnnWord(stem.id, stem.candidate, stroke, stem.partOfSpeech, stem.frequency)
    { }

    WnnClause(const wstring &stroke, const WnnWord& stem, const WnnWord& fzk) :
        WnnWord(stem.id, stem.candidate + fzk.candidate, stroke, WnnPOS(stem.partOfSpeech.left, fzk.partOfSpeech.right), stem.frequency, 1)
    { }

    bool isClause() const
    {
        return true;
    }
};

class WnnSentence : public WnnWord
{
public:

    WnnSentence(): WnnWord() {}

    WnnSentence(const wstring &input, const vector<WnnClause> &clauses) :
        WnnWord()
    {
        if (!clauses.empty()) {
            elements = clauses;
            const WnnClause &headClause = clauses.front();

            if (clauses.size() == 1) {
                id = headClause.id;
                candidate = headClause.candidate;
                stroke = input;
                frequency = headClause.frequency;
                partOfSpeech = headClause.partOfSpeech;
                attribute = headClause.attribute;
            } else {
                wstring candidateBuilder;
                for (vector<WnnClause>::const_iterator ci = clauses.begin();
                     ci != clauses.end(); ci++) {
                    candidateBuilder.append(ci->candidate);
                }
                const WnnClause &lastClause = (WnnClause)clauses.back();

                id = headClause.id;
                candidate = candidateBuilder;
                stroke = input;
                frequency = headClause.frequency;
                partOfSpeech = WnnPOS(headClause.partOfSpeech.left, lastClause.partOfSpeech.right);
                attribute = 2;
            }
        }
    }

    WnnSentence(const wstring &input, const WnnClause &clause) :
        WnnWord(clause.id, clause.candidate, input, clause.partOfSpeech, clause.frequency, clause.attribute)
    {
        elements.push_back(clause);
    }

    WnnSentence(const WnnSentence &prev, const WnnClause &clause) :
        WnnWord(prev.id, prev.candidate + clause.candidate, prev.stroke + clause.stroke, WnnPOS(prev.partOfSpeech.left, clause.partOfSpeech.right), prev.frequency + clause.frequency, prev.attribute)
    {
        elements.assign(prev.elements.begin(),prev.elements.end());
        elements.push_back(clause);
    }

    WnnSentence(const WnnClause &head, const WnnSentence *tail = 0) :
        WnnWord()
    {
        if (tail == 0) {
            // single clause
            id = head.id;
            candidate = head.candidate;
            stroke = head.stroke;
            frequency = head.frequency;
            partOfSpeech = head.partOfSpeech;
            attribute = head.attribute;
            elements.push_back(head);
        } else {
            // consecutive clauses
            id = head.id;
            candidate = head.candidate + tail->candidate;
            stroke = head.stroke + tail->stroke;
            frequency = head.frequency + tail->frequency;
            partOfSpeech = WnnPOS(head.partOfSpeech.left, tail->partOfSpeech.right);
            attribute = 2;

            elements.push_back(head);
            elements.assign(tail->elements.begin(),tail->elements.end());
        }
    }

    bool isSentence() const
    {
        return true;
    }

    vector<WnnClause> elements;
};

#endif // WNNWORD_H

