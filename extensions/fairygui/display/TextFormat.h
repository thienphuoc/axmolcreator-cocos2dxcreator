#ifndef __TEXTFORMAT_H__
#define __TEXTFORMAT_H__

#include "cocos2d.h"
#include "FairyGUIMacros.h"

NS_FGUI_BEGIN

class TextFormat
{
public:
    TextFormat();
    TextFormat(const TextFormat & other);
    TextFormat &operator =(const TextFormat & other);

	void setFormat(const TextFormat& format);
    void enableEffect(int effectFlag) { effect |= effectFlag; }
    void disableEffect(int effectFlag) { effect &= ~effectFlag; }
    bool hasEffect(int effectFlag) const { return (effect & effectFlag) != 0; }

    static const int OUTLINE = 1;
    static const int SHADOW = 2;
    static const int GLOW = 4;

    std::string face;
    float fontSize;
    ax::Color3B color;
    bool bold;
    bool italics;
    bool underline;
    int lineSpacing;
    int letterSpacing;
    ax::TextHAlignment align;
    ax::TextVAlignment verticalAlign;

    int effect;
    ax::Color3B outlineColor;
    int outlineSize;
    ax::Color3B shadowColor;
    ax::Size shadowOffset;
    int shadowBlurRadius;
    ax::Color3B glowColor;

    //internal use
    bool _hasColor;
};

NS_FGUI_END

#endif
