/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
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

#pragma once

#include "axmol.h"
#include "../BaseTest.h"

DEFINE_TEST_SUITE(SpriteFrameCacheTests);

class SpriteFrameCachePixelFormatTest : public TestCase
{
public:
    CREATE_FUNC(SpriteFrameCachePixelFormatTest);

    virtual std::string title() const override { return "Pixel formats test"; }
    virtual std::string subtitle() const override { return "It shouldn't crash"; }

    SpriteFrameCachePixelFormatTest();

private:
    void loadSpriteFrames(std::string_view file, ax::backend::PixelFormat expectedFormat);

private:
    ax::Label* infoLabel;
};

class SpriteFrameCacheLoadMultipleTimes : public TestCase
{
public:
    CREATE_FUNC(SpriteFrameCacheLoadMultipleTimes);

    virtual std::string title() const override { return "Load same plist multiple times"; }
    virtual std::string subtitle() const override { return "It shouldn't crash"; }

    SpriteFrameCacheLoadMultipleTimes();

private:
    void loadSpriteFrames(std::string_view file, ax::backend::PixelFormat expectedFormat);
};

class SpriteFrameCacheFullCheck : public TestCase
{
public:
    CREATE_FUNC(SpriteFrameCacheFullCheck);

    virtual std::string title() const override { return "Test isSpriteFramesWithFileLoaded"; }
    virtual std::string subtitle() const override { return "It shouldn't crash"; }

    SpriteFrameCacheFullCheck();

private:
    void loadSpriteFrames(std::string_view file, ax::backend::PixelFormat expectedFormat);
};

class SpriteFrameCacheJsonAtlasTest : public TestCase
{
public:
    CREATE_FUNC(SpriteFrameCacheJsonAtlasTest);

    virtual std::string title() const override { return "Test Generic JSON Atlas"; }
    virtual std::string subtitle() const override { return "It shouldn't crash"; }

    SpriteFrameCacheJsonAtlasTest();
    ~SpriteFrameCacheJsonAtlasTest() override;

private:
    void loadSpriteFrames(std::string_view file, ax::backend::PixelFormat expectedFormat);

    ax::Label* infoLabel;
};
