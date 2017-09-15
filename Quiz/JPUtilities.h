#ifndef __JP_UTILITIES_H__
#define __JP_UTILITIES_H__

#include <vector>
#include <algorithm>
#include <QString>

inline const std::vector<QString>& hiragana()
{
    const static std::vector<QString> hiragana{ "あ", "い", "う", "え", "お", "か", "き", "く", "け", "こ", "さ", "し", "す", "せ", "そ", "た", "ち", "つ", "て", "と", "な", "に", "ぬ", "ね", "の", "は", "ひ", "ふ", "へ", "ほ", "ま", "み", "む", "め", "も", "や", "ゆ", "よ", "ら", "り", "る", "れ", "ろ", "わ", "を", "ん", "が", "ぎ", "ぐ", "げ", "ご", "ざ", "じ", "ず", "ぜ", "ぞ", "だ", "ぢ", "づ", "で", "ど", "ば", "び", "ぶ", "べ", "ぼ", "ぱ", "ぴ", "ぷ", "ぺ", "ぽ", "っ", "ゃ", "ゅ", "ょ" };
    return hiragana;
}

inline const std::vector<QString>& katakana()
{
    const static std::vector<QString> katakana{ "ア", "イ", "ウ", "エ", "オ", "カ", "キ", "ク", "ケ", "コ", "サ", "シ", "ス", "セ", "ソ", "タ", "チ", "ツ", "テ", "ト", "ナ", "ニ", "ヌ", "ネ", "ノ", "ハ", "ヒ", "フ", "ヘ", "ホ", "マ", "ミ", "ム", "メ", "モ", "ヤ", "ユ", "ヨ", "ラ", "リ", "ル", "レ", "ロ", "ワ", "ヲ", "ン", "ガ", "ギ", "グ", "ゲ", "ゴ", "ザ", "ジ", "ズ", "ゼ", "ゾ", "ダ", "ヂ", "ヅ", "デ", "ド", "バ", "ビ", "ブ", "ベ", "ボ", "パ", "ピ", "プ", "ペ", "ポ", "ッ", "ャ", "ュ", "ョ" };
    return katakana;
}

inline bool is_hiragana(const QString& str)
{
    for (const auto& c : str)
    {
        auto found = std::find(hiragana().begin(), hiragana().end(), c);
        if (found == hiragana().end())
            return false;
    }
    return true;
}

inline bool is_hiragana(const std::vector<QString>& strs)
{
    for (const auto& str : strs)
    {
        if (!is_hiragana(str))
            return false;
    }
    return true;
}

inline bool is_katakana(const QString& str)
{
    for (const auto& c : str)
    {
        auto found = std::find(katakana().begin(), katakana().end(), c);
        if (found == katakana().end())
            return false;
    }
    return true;
}

template<class TContainer>
bool is_katakana(const TContainer& strs)
{
    for (const auto& str : strs)
    {
        if (!is_katakana(str))
            return false;
    }
    return true;
}

inline QString hiragana_to_katakana(QString base)
{
    for (auto& c : base)
    {
        auto found = std::find(hiragana().begin(), hiragana().end(), c);
        if (found != hiragana().end())
            base = base.replace(c, katakana()[std::distance(hiragana().begin(), found)]);
    }

    return base;
}

inline QString katakana_to_hiragana(QString base)
{
    for (auto& c : base)
    {
        auto found = std::find(katakana().begin(), katakana().end(), c);
        if (found != katakana().end())
            base = base.replace(c, hiragana()[std::distance(katakana().begin(), found)]);
    }

    return base;
}

#endif