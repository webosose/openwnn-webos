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

#include "openwnninputmethod.h"
#include "openwnnenginejajp.h"
#include "composingtext.h"
#include "romkan.h"
#include "romkanfullkatakana.h"
#include "romkanhalfkatakana.h"
#include "util.h"
#include "fullRoman.h"
#include "halfRoman.h"

#include <cwctype>
#include <iostream>

static const int KEY_ESC       = 0x01000000;
static const int KEY_LEFT      = 0x01000012;
static const int KEY_RIGHT     = 0x01000014;
static const int KEY_BACKSPACE = 0x01000003;
static const int KEY_SPACE     = 0x20;
static const int KEY_ENTER     = 0x01000005;
static const int KEY_RETURN    = 0x01000004;

static const wstring KEY_DAKUON    = L"\u309B";
static const wstring KEY_HANDAKUON = L"\u309C";

class OpenWnnInputMethodPrivate
{
public:
    enum EngineMode {
        ENGINE_MODE_DEFAULT,
        ENGINE_MODE_DIRECT,
        ENGINE_MODE_NO_LV2_CONV,
        ENGINE_MODE_FULL_ROMAN,
        ENGINE_MODE_FULL_KATAKANA,
        ENGINE_MODE_HALF_KATAKANA,
    };

    enum ConvertType {
        CONVERT_TYPE_NONE = 0,
        CONVERT_TYPE_RENBUN = 1,
    };

    enum {
        MAX_COMPOSING_TEXT = 30
    };

    OpenWnnInputMethodPrivate()
    {
        init();
    }

    void init() {
        converter = 0;
        activeConvertType = CONVERT_TYPE_NONE;
        enablePrediction = true;
        enableConverter = false;
        disableUpdate = false;
        commitCount = 0;
        targetLayer = ComposingText::LAYER1;
        activeWordIndex = -1;
        commitedText.clear();
        commitCount = 0;
        changeEngineMode(ENGINE_MODE_DEFAULT);
        inputMode = OpenWnnInputMethod::Hiragana;
        resetScreen();
    }

    void changeEngineMode(EngineMode mode)
    {
        switch (mode) {
        case ENGINE_MODE_DIRECT:
            setDictionary(OpenWnnEngineJAJP::DIC_LANG_JP);
            enableConverter = true;
            converter = &converterJAJP;
            preConverter.reset(new HalfRoman());
            break;
        case ENGINE_MODE_FULL_ROMAN:
            setDictionary(OpenWnnEngineJAJP::DIC_LANG_JP);
            enableConverter = true;
            converter = &converterJAJP;
            preConverter.reset(new FullRoman());
            break;
        case ENGINE_MODE_NO_LV2_CONV:
            enableConverter = false;
            converter = NULL;
            preConverter.reset(new Romkan());
            break;

        case ENGINE_MODE_FULL_KATAKANA:
            setDictionary(OpenWnnEngineJAJP::DIC_LANG_JP);
            enableConverter = true;
            converter = &converterJAJP;
            preConverter.reset(new RomkanFullKatakana());
            break;

        case ENGINE_MODE_HALF_KATAKANA:
            converter = NULL;
            preConverter.reset(new RomkanHalfKatakana());
            break;

        default:
            /* HIRAGANA input mode */
            setDictionary(OpenWnnEngineJAJP::DIC_LANG_JP);
            enableConverter = true;
            converter = &converterJAJP;
            preConverter.reset(new Romkan());
            break;
        }
    }

    void setDictionary(OpenWnnEngineJAJP::DictionaryType mode)
    {
        converterJAJP.setDictionary(mode);
    }

    void breakSequence()
    {
        converterJAJP.breakSequence();
    }

    bool isEnableL2Converter()
    {
        return converter != NULL && enableConverter;
    }

    void startConvert(ConvertType convertType)
    {
        if (!isEnableL2Converter())
            return;

        if (activeConvertType != convertType) {
            if (!exactMatchMode) {
                if (convertType == CONVERT_TYPE_RENBUN) {
                    /* not specify */
                    composingText.setCursor(ComposingText::LAYER1, 0);
                } else {
                    if (activeConvertType == CONVERT_TYPE_RENBUN) {
                        exactMatchMode = true;
                    } else {
                        /* specify all range */
                        composingText.setCursor(ComposingText::LAYER1,
                                                composingText.size(ComposingText::LAYER1));
                    }
                }
            }

            if (convertType == CONVERT_TYPE_RENBUN)
                /* clears variables for the prediction */
                exactMatchMode = false;

            /* clears variables for the convert */
            commitCount = 0;

            activeConvertType = convertType;

            updatePreeditStatus(ComposingText::LAYER2, true, true);

            focusNextCandidate();
        }
    }

    void changeL2Segment(const wstring &word)
    {
        if (word.empty())
            return;
        vector<StrSegment> ss;
        ss.push_back(composingText.getStrSegment(ComposingText::LAYER2, 0));
        if (ss[0].clause)
            ss[0].clause->candidate = word;
        ss[0].str = word;
        composingText.replaceStrSegment(ComposingText::LAYER2, ss);
        updatePreeditStatus(ComposingText::LAYER2, false, false);
    }

    void resetScreen()
    {
        preeditText.clear();
        composingText.clear();
        exactMatchMode = false;
        activeConvertType = CONVERT_TYPE_NONE;
        clearCandidates();
    }

    void updateViewStatusForPrediction(bool updateCandidates, bool updateEmptyText)
    {
        activeConvertType = CONVERT_TYPE_NONE;
        updatePreeditStatus(ComposingText::LAYER1, updateCandidates, updateEmptyText);
    }

    //Pre-edit processing
    void updatePreeditStatus(ComposingText::TextLayer layer, bool updateCandidates, bool updateEmptyText)
    {
        targetLayer = layer;

        if (updateCandidates)
            updateCandidateView();

        /* set the text for displaying as the composing text */
        preeditText.clear();
        preeditText.insert(0, composingText.toString(layer));
    }

    void updateCandidateView()
    {
        switch (targetLayer) {
        case ComposingText::LAYER0:
        case ComposingText::LAYER1: /* prediction */
            if (enablePrediction)
                /* update the candidates view */
                updatePrediction();
            break;
        case ComposingText::LAYER2: /* convert */
            if (commitCount == 0)
                converter->convert(composingText);

            if (converter->makeCandidateListOf(commitCount) != 0) {
                composingText.setCursor(ComposingText::LAYER2, 1);
                updatedCandidates();
            } else {
                composingText.setCursor(ComposingText::LAYER1,
                                         composingText.toString(ComposingText::LAYER1).length());
                clearCandidates();
            }
            break;
        default:
            break;
        }
    }

    void updatePrediction()
    {
        int candidates = 0;
        int cursor = composingText.getCursor(ComposingText::LAYER1);
        if (isEnableL2Converter()) {
            if (exactMatchMode)
                /* exact matching */
                candidates = converter->predict(composingText, 0, cursor);
            else
                /* normal prediction */
                candidates = converter->predict(composingText, 0, -1);
        }

        /* update the candidates view */
        if (candidates > 0)
            updatedCandidates();
        else
            clearCandidates();
    }

    void updatedCandidates()
    {
        clearCandidates(true);
        shared_ptr<WnnWord> result;
        while ((result = converter->getNextCandidate()))
            candidateList.push_back(result->candidate);
    }


    void clearCandidates(bool deferUpdate = false)
    {
        activeWordIndex = -1;
        if (!candidateList.empty()) {
            candidateList.clear();
        }
    }

    wstring focusNextCandidate()
    {
        if (candidateList.empty())
            return 0;
        activeWordIndex++;
        if (activeWordIndex >= candidateList.size())
            activeWordIndex = 0;
        return candidateList.at(activeWordIndex);
    }

    void commitAll()
    {
        if (activeConvertType != CONVERT_TYPE_NONE) {
            commitConvertingText();
        } else {
            composingText.setCursor(ComposingText::LAYER1,
                                     composingText.size(ComposingText::LAYER1));
            commitText(true);
        }
    }


    void commitConvertingText()
    {
        if (activeConvertType != CONVERT_TYPE_NONE) {

            wstring text = composingText.toString(ComposingText::LAYER2);
            disableUpdate = true;
            commitedText = text;
            disableUpdate = false;
            resetScreen();
        }
    }

    bool commitText(bool learn = false)
    {
        ComposingText::TextLayer layer = targetLayer;
        int cursor = composingText.getCursor(layer);
        if (cursor == 0) {
            return false;
        }
        wstring tmp = composingText.toString(layer, 0, cursor - 1);

        if (converter != NULL) {
            breakSequence();
        }
        return commitText(tmp);
    }

    bool commitText(const wstring &str)
    {
        ComposingText::TextLayer layer = targetLayer;

        disableUpdate = true;
        commitedText = str;
        disableUpdate = false;

        int cursor = composingText.getCursor(layer);
        if (cursor > 0) {
            composingText.deleteStrSegment(layer, 0, composingText.getCursor(layer) - 1);
            composingText.setCursor(layer, composingText.size(layer));
        }
        exactMatchMode = false;
        commitCount++;

        if ((layer == ComposingText::LAYER2) && (composingText.size(layer) == 0))
            layer = ComposingText::LAYER1; /* for connected prediction */

        if (layer == ComposingText::LAYER2) {
            activeConvertType = CONVERT_TYPE_RENBUN;
            updatePreeditStatus(layer, true, false);
            focusNextCandidate();
        } else {
            updateViewStatusForPrediction(true, false);
        }

        return isComposing();
    }

    bool isComposing()
    {
        return composingText.size(ComposingText::LAYER0) > 0;
    }

    bool isAlphabetLast(const wstring &str)
    {
        if (str.empty())
            return false;
        char ch = str.at(str.length() - 1);
        return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
    }

    void commitTextWithoutLastAlphabet()
    {
        wstring last = composingText.getStrSegment(targetLayer, -1).str;

        if (isAlphabetLast(last)) {
            composingText.moveCursor(ComposingText::LAYER1, -1);
            commitText(false);
            composingText.moveCursor(ComposingText::LAYER1, 1);
        } else {
            commitText(false);
        }
    }

    bool processLeftKeyEvent()
    {
        if (composingText.size(ComposingText::LAYER1) == 0)
            return false;

        if (activeConvertType != CONVERT_TYPE_NONE) {
            if (composingText.getCursor(ComposingText::LAYER1) > 1) {
                composingText.moveCursor(ComposingText::LAYER1, -1);
            }
        } else if (exactMatchMode) {
            composingText.moveCursor(ComposingText::LAYER1, -1);
        } else {
            exactMatchMode = true;
        }

        commitCount = 0; /* retry consecutive clause conversion if necessary. */
        updatePreeditStatus(targetLayer, true, true);

        if (activeConvertType != CONVERT_TYPE_NONE)
            focusNextCandidate();

        return true;
    }


    bool processRightKeyEvent()
    {
        if (composingText.size(ComposingText::LAYER1) == 0)
            return false;

        ComposingText::TextLayer layer = targetLayer;
        if (exactMatchMode || activeConvertType != CONVERT_TYPE_NONE) {
            int textSize = composingText.size(ComposingText::LAYER1);
            if (composingText.getCursor(ComposingText::LAYER1) == textSize) {
                exactMatchMode = false;
                layer = ComposingText::LAYER1; /* convert -> prediction */
                activeConvertType = CONVERT_TYPE_NONE;
            } else {
                composingText.moveCursor(ComposingText::LAYER1, 1);
            }
        } else {
            if (composingText.getCursor(ComposingText::LAYER1) < composingText.size(ComposingText::LAYER1)) {
                composingText.moveCursor(ComposingText::LAYER1, 1);
            }
        }

        commitCount = 0; /* retry consecutive clause conversion if necessary. */

        updatePreeditStatus(layer, true, true);

        if (activeConvertType != CONVERT_TYPE_NONE)
            focusNextCandidate();

        return true;
    }

    OpenWnnInputMethod::InputMode inputMode;
    bool exactMatchMode;
    wstring preeditText;
    wstring commitedText;
    OpenWnnEngineJAJP *converter;
    OpenWnnEngineJAJP converterJAJP;
    ConvertType activeConvertType;
    ComposingText composingText;
    shared_ptr<LetterConverter> preConverter;
    bool enablePrediction;
    bool enableConverter;
    bool disableUpdate;
    int commitCount;
    ComposingText::TextLayer targetLayer;
    vector<wstring> candidateList;
    int activeWordIndex;
};

OpenWnnInputMethod::OpenWnnInputMethod()
{
    p.reset(new OpenWnnInputMethodPrivate());
}

OpenWnnInputMethod::~OpenWnnInputMethod()
{
}

bool OpenWnnInputMethod::setInputMode(InputMode inputMode)
{
    update();
    switch (inputMode) {
    case Hiragana:
        p->changeEngineMode(OpenWnnInputMethodPrivate::ENGINE_MODE_DEFAULT);
        break;

    case Katakana:
        p->changeEngineMode(OpenWnnInputMethodPrivate::ENGINE_MODE_FULL_KATAKANA);
        break;

    case FullwidthLatin:
        p->changeEngineMode(OpenWnnInputMethodPrivate::ENGINE_MODE_FULL_ROMAN);
        break;

    default:
        p->changeEngineMode(OpenWnnInputMethodPrivate::ENGINE_MODE_DIRECT);
        break;
    }
    p->inputMode = inputMode;
    return true;
}

bool OpenWnnInputMethod::keyEvent(int key, const wstring &text)
{
    p->commitedText.clear();

    if (p->inputMode != Latin && p->preConverter == NULL && !p->isEnableL2Converter())
        return false;

    switch (key) {
    case KEY_LEFT:
        if (p->isEnableL2Converter() && p->composingText.size(ComposingText::LAYER1) > 0)
            return p->processLeftKeyEvent();
        else
            return p->commitText(false);
        break;

    case KEY_RIGHT:
        if (p->isEnableL2Converter() && p->composingText.size(ComposingText::LAYER1) > 0)
            return p->processRightKeyEvent();
        else
            return p->commitText(false);
        break;

    case KEY_BACKSPACE:
        if (p->composingText.size(ComposingText::LAYER1) > 0) {
            if (p->activeConvertType == OpenWnnInputMethodPrivate::CONVERT_TYPE_RENBUN) {
                p->composingText.setCursor(ComposingText::LAYER1,
                                           p->composingText.toString(ComposingText::LAYER1).length());
                p->exactMatchMode = false;
            } else {
                if ((p->composingText.size(ComposingText::LAYER1) == 1) &&
                     p->composingText.getCursor(ComposingText::LAYER1) != 0) {
                    p->resetScreen();
                    return true;
                } else {
                    p->composingText.deleteAt(ComposingText::LAYER1, false);
                }
            }
            p->updateViewStatusForPrediction(true, true);
            return true;
        }
        break;

    case KEY_SPACE:
        if (p->composingText.size(ComposingText::LAYER0) == 0) {
            p->clearCandidates();
            p->breakSequence();
        } else {
            if (p->targetLayer == ComposingText::LAYER2)
                p->changeL2Segment(p->focusNextCandidate());
            else if (p->isEnableL2Converter())
                p->startConvert(OpenWnnInputMethodPrivate::CONVERT_TYPE_RENBUN);
            else
                return p->commitText(false);
            return true;
        }
        break;

    case KEY_RETURN:
    case KEY_ENTER:
        if (p->composingText.size(ComposingText::LAYER0) > 0) {
            p->commitText(true);
            return true;
        }
        break;
    default:
        if (!text.empty()) {
            if (p->composingText.size(ComposingText::LAYER1) + text.size() > OpenWnnInputMethodPrivate::MAX_COMPOSING_TEXT)
                return true;

            const int last = text.size() - 1;

            if(text.compare(KEY_DAKUON) == 0 || text.compare(KEY_HANDAKUON) == 0) {
                int cursor = p->composingText.getCursor(ComposingText::LAYER1);
                if ((cursor > 0) &&
                        p->activeConvertType == OpenWnnInputMethodPrivate::CONVERT_TYPE_NONE) {
                    wstring search = p->composingText.getStrSegment(ComposingText::LAYER1, cursor - 1).str;
                    wstring replace = text.compare(KEY_DAKUON) == 0 ?
                            p->converter->getDakuon(search) : p->converter->getHandakuon(search);
                    if (!replace.empty()) {
                        p->composingText.deleteAt(ComposingText::LAYER1,false);
                        p->composingText.insertStrSegment(ComposingText::LAYER0,ComposingText::LAYER1,replace);
                        p->updateViewStatusForPrediction(true, true);
                        return true;
                    }
                }
            }

            for (int i = 0; i <= last; ++i) {
                if (p->isEnableL2Converter()) {
                    p->commitConvertingText();
                    p->composingText.insertStrSegment(ComposingText::LAYER0, ComposingText::LAYER1, text.substr(i, 1));
                    if (p->preConverter != NULL)
                        p->preConverter->convert(p->composingText);
                    if (i == last)
                        p->updateViewStatusForPrediction(true, true);
                } else {
                    p->composingText.insertStrSegment(ComposingText::LAYER0, ComposingText::LAYER1, text.substr(i, 1));
                    wstring layer1 = p->composingText.toString(ComposingText::LAYER1);
                    if (!p->isAlphabetLast(layer1)) {
                        p->commitText(false);
                    } else {
                        bool completed = p->preConverter->convert(p->composingText);
                        if (completed) {
                            p->commitTextWithoutLastAlphabet();
                        } else {
                            if (i == last)
                                p->updateViewStatusForPrediction(true, true);
                        }
                    }
                }
            }
            return true;
        }
        break;
    }

    return false;
}

bool OpenWnnInputMethod::isComposing()
{
    return p->isComposing();
}

int OpenWnnInputMethod::candidateListItemCount()
{
    return p->candidateList.size();
}

void OpenWnnInputMethod::candidateListItemSelected(int index)
{
   p->commitText(p->candidateList.at(index));
}

vector<wstring>* OpenWnnInputMethod::getCandidateList() {
    return &p->candidateList;
}

void OpenWnnInputMethod::reset()
{
    p->resetScreen();
}

void OpenWnnInputMethod::update()
{
    if (!p->disableUpdate)
        reset();
}

wstring OpenWnnInputMethod::getPreedit()
{
    return p->preeditText;
}

wstring OpenWnnInputMethod::getCommit()
{
    return p->commitedText;
}

