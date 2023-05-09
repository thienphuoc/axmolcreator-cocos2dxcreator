/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 https://axmolengine.github.io/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __TEXTURE2D_TEST_H__
#define __TEXTURE2D_TEST_H__

#include "../BaseTest.h"

DEFINE_TEST_SUITE(Texture2DTests);

class TextureDemo : public TestCase
{
public:
    virtual ~TextureDemo();
    virtual void onEnter() override;
};

class TextureASTC : public TextureDemo
{
public:
    CREATE_FUNC(TextureASTC);
    virtual std::string title() const override;
    virtual void onEnter() override;

    ax::Node* _background;
};

class TextureETC1Alpha : public TextureDemo
{
public:
    CREATE_FUNC(TextureETC1Alpha);
    TextureETC1Alpha();
    bool init() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void addNewSpriteWithCoords(ax::Vec2 p);
    void onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event);

    ax::Node* _background;
};

class TextureETC2 : public TextureDemo
{
public:
    CREATE_FUNC(TextureETC2);
    TextureETC2();
    bool init() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void addNewSpriteWithCoords();
    void onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event);

    ax::Node* _background;
};

class TextureBMP : public TextureDemo
{
public:
    CREATE_FUNC(TextureBMP);
    TextureBMP();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void addNewSpriteWithCoords(ax::Vec2 p);
    void onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event);
};

class TextureTGA : public TextureDemo
{
public:
    CREATE_FUNC(TextureTGA);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePNG : public TextureDemo
{
public:
    CREATE_FUNC(TexturePNG);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureJPEG : public TextureDemo
{
public:
    CREATE_FUNC(TextureJPEG);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TextureWEBP : public TextureDemo
{
public:
    CREATE_FUNC(TextureWEBP);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TextureWEBPNoAlpha : public TextureDemo
{
public:
    CREATE_FUNC(TextureWEBPNoAlpha);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TextureMipMap : public TextureDemo
{
public:
    CREATE_FUNC(TextureMipMap);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRTest : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRTest);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVR2BPP : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVR2BPP);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVR4BPP : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVR4BPP);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA8888 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGBA8888);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRBGRA8888 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRBGRA8888);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA4444 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGBA4444);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA4444GZ : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGBA4444GZ);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA4444CCZ : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGBA4444CCZ);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA5551 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGBA5551);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRRGB565 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGB565);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRRGB888 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGB888);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRA8 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRA8);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRI8 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRI8);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRAI88 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRAI88);
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVR2BPPv3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVR2BPPv3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRII2BPPv3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRII2BPPv3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVR4BPPv3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVR4BPPv3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRII4BPPv3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRII4BPPv3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA8888v3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGBA8888v3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRBGRA8888v3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRBGRA8888v3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA4444v3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGBA4444v3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA5551v3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGBA5551v3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGB565v3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGB565v3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGB888v3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRRGB888v3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRA8v3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRA8v3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRI8v3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRI8v3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRAI88v3 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRAI88v3);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRBadEncoding : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRBadEncoding);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRMipMap : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRMipMap);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRMipMap2 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRMipMap2);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRNonSquare : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRNonSquare);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRNPOT4444 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRNPOT4444);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRNPOT8888 : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRNPOT8888);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureAlias : public TextureDemo
{
public:
    CREATE_FUNC(TextureAlias);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePixelFormat : public TextureDemo
{
public:
    CREATE_FUNC(TexturePixelFormat);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureBlend : public TextureDemo
{
public:
    CREATE_FUNC(TextureBlend);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureAsync : public TextureDemo
{
public:
    CREATE_FUNC(TextureAsync);
    virtual ~TextureAsync();

    virtual float getDuration() const override { return 5.0f; }
    void loadImages(float dt);
    void imageLoaded(ax::Texture2D* texture);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;

private:
    int _imageOffset;
};

class TextureGlRepeat : public TextureDemo
{
public:
    CREATE_FUNC(TextureGlRepeat);
    virtual ~TextureGlRepeat();
    virtual std::string title() const override;

    virtual void onEnter() override;
};

class TextureGlClamp : public TextureDemo
{
public:
    CREATE_FUNC(TextureGlClamp);
    virtual ~TextureGlClamp();
    virtual std::string title() const override;

    virtual void onEnter() override;
};

class TextureSizeTest : public TextureDemo
{
public:
    CREATE_FUNC(TextureSizeTest);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureCache1 : public TextureDemo
{
public:
    CREATE_FUNC(TextureCache1);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureDrawAtPoint : public TextureDemo
{
public:
    CREATE_FUNC(TextureDrawAtPoint);
    ~TextureDrawAtPoint();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    virtual void draw(ax::Renderer* renderer, const ax::Mat4& transform, uint32_t flags) override;

protected:
    void onDraw(const ax::Mat4& transform, uint32_t flags);

    ax::Texture2D *_tex1, *_Tex2F;
};

class TextureDrawInRect : public TextureDemo
{
public:
    CREATE_FUNC(TextureDrawInRect);
    ~TextureDrawInRect();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    virtual void draw(ax::Renderer* renderer, const ax::Mat4& transform, uint32_t flags) override;

protected:
    void onDraw(const ax::Mat4& transform, uint32_t flags);

    ax::Texture2D *_tex1, *_Tex2F;
};

class TextureMemoryAlloc : public TextureDemo
{
public:
    CREATE_FUNC(TextureMemoryAlloc);
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void updateImage(ax::Ref* sender);
    void changeBackgroundVisible(ax::Ref* sender);

private:
    ax::Sprite* _background;
};

class TexturePVRv3Premult : public TextureDemo
{
public:
    CREATE_FUNC(TexturePVRv3Premult);
    TexturePVRv3Premult();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void transformSprite(ax::Sprite* sprite);
};

// S3TC Dxt1 texture format test
class TextureS3TCDxt1 : public TextureDemo
{
public:
    CREATE_FUNC(TextureS3TCDxt1);
    TextureS3TCDxt1();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// S3TC Dxt3 texture format test
class TextureS3TCDxt3 : public TextureDemo
{
public:
    CREATE_FUNC(TextureS3TCDxt3);
    TextureS3TCDxt3();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// S3TC Dxt5 texture format test
class TextureS3TCDxt5 : public TextureDemo
{
public:
    CREATE_FUNC(TextureS3TCDxt5);
    TextureS3TCDxt5();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// S3TC texture with no mipmaps test
class TextureS3TCWithNoMipmaps : public TextureDemo
{
public:
    CREATE_FUNC(TextureS3TCWithNoMipmaps);
    TextureS3TCWithNoMipmaps();

    virtual std::string title() const override;
};

// ATITC RGB texture format test
class TextureATITCRGB : public TextureDemo
{
public:
    CREATE_FUNC(TextureATITCRGB);
    TextureATITCRGB();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// ATITC RGBA Explicit texture format test
class TextureATITCExplicit : public TextureDemo
{
public:
    CREATE_FUNC(TextureATITCExplicit);
    TextureATITCExplicit();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// ATITC RGBA Interpolated texture format test
class TextureATITCInterpolated : public TextureDemo
{
public:
    CREATE_FUNC(TextureATITCInterpolated);
    TextureATITCInterpolated();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// RGB888 texture convert test
class TextureConvertRGB888 : public TextureDemo
{
public:
    CREATE_FUNC(TextureConvertRGB888);
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// RGBA8888 texture convert test
class TextureConvertRGBA8888 : public TextureDemo
{
public:
    CREATE_FUNC(TextureConvertRGBA8888);
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// I8 texture convert test
class TextureConvertL8 : public TextureDemo
{
public:
    CREATE_FUNC(TextureConvertL8);
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// AI88 texture convert test
class TextureConvertLA8 : public TextureDemo
{
public:
    CREATE_FUNC(TextureConvertLA8);
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

#endif  // __TEXTURE2D_TEST_H__
