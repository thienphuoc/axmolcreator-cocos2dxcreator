/****************************************************************************
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

#include "FileUtilsTest.h"

USING_NS_AX;

FileUtilsTests::FileUtilsTests()
{
    ADD_TEST_CASE(TestSearchPath);
    ADD_TEST_CASE(TestIsFileExist);
    ADD_TEST_CASE(TestIsDirectoryExist);
    ADD_TEST_CASE(TestFileFuncs);
    ADD_TEST_CASE(TestDirectoryFuncs);
    ADD_TEST_CASE(TestWriteString);
    ADD_TEST_CASE(TestGetContents);
    ADD_TEST_CASE(TestWriteData);
    ADD_TEST_CASE(TestWriteValueMap);
    ADD_TEST_CASE(TestWriteValueVector);
    ADD_TEST_CASE(TestUnicodePath);
    ADD_TEST_CASE(TestIsFileExistAsync);
    ADD_TEST_CASE(TestIsDirectoryExistAsync);
    ADD_TEST_CASE(TestFileFuncsAsync);
    ADD_TEST_CASE(TestWriteStringAsync);
    ADD_TEST_CASE(TestWriteDataAsync);
    ADD_TEST_CASE(TestListFiles);
    ADD_TEST_CASE(TestIsFileExistRejectFolder);
}

// TestSearchPath

void TestSearchPath::onEnter()
{
    FileUtilsDemo::onEnter();
    auto sharedFileUtils = FileUtils::getInstance();

    std::string ret;

    sharedFileUtils->purgeCachedEntries();
    _defaultSearchPathArray              = sharedFileUtils->getOriginalSearchPaths();
    std::vector<std::string> searchPaths = _defaultSearchPathArray;
    std::string writablePath             = sharedFileUtils->getWritablePath();
    std::string fileName                 = writablePath + "external.txt";
    char szBuf[100]                      = "Hello Cocos2d-x!";
    FILE* fp                             = fopen(fileName.c_str(), "wb");
    if (fp)
    {
        size_t ret = fwrite(szBuf, 1, strlen(szBuf), fp);
        AXASSERT(ret != 0, "fwrite function returned zero value");
        fclose(fp);
        if (ret != 0)
            log("Writing file to writable path succeed.");
    }

    searchPaths.insert(searchPaths.begin(), writablePath);
    searchPaths.insert(searchPaths.begin() + 1, "Misc/searchpath1");
    searchPaths.insert(searchPaths.begin() + 2, "Misc/searchpath2");
    sharedFileUtils->setSearchPaths(searchPaths);

    for (int i = 1; i < 3; i++)
    {
        auto filename = StringUtils::format("file%d.txt", i);
        ret           = sharedFileUtils->fullPathForFilename(filename);
        log("%s -> %s", filename.c_str(), ret.c_str());
    }

    // Gets external.txt from writable path
    std::string fullPath = sharedFileUtils->fullPathForFilename("external.txt");
    log("external file path = %s", fullPath.c_str());
    if (fullPath.length() > 0)
    {
        fp = fopen(fullPath.c_str(), "rb");
        if (fp)
        {
            char szReadBuf[100] = {0};
            size_t read         = fread(szReadBuf, 1, strlen(szReadBuf), fp);
            if (read > 0)
                log("The content of file from writable path: %s", szReadBuf);
            fclose(fp);
        }
    }

    // FIXME: should fix the issue on Android
#if (AX_TARGET_PLATFORM != AX_PLATFORM_ANDROID)

    // Save old resource root path
    std::string oldDefaultRootPath = sharedFileUtils->getDefaultResourceRootPath();
    sharedFileUtils->setDefaultResourceRootPath(oldDefaultRootPath + "extensions");
    auto sp1 = Sprite::create("orange_edit.png");
    sp1->setPosition(VisibleRect::center());
    addChild(sp1);

    // Recover resource root path
    sharedFileUtils->setDefaultResourceRootPath(oldDefaultRootPath);

    auto oldSearchPaths = sharedFileUtils->getOriginalSearchPaths();
    sharedFileUtils->addSearchPath("Images");
    auto sp2 = Sprite::create("btn-about-normal.png");
    sp2->setPosition(VisibleRect::center() + Vec2(0, -50));
    addChild(sp2);

    // Recover old search paths
    sharedFileUtils->setSearchPaths(oldSearchPaths);
#endif
}

void TestSearchPath::onExit()
{
    FileUtils* sharedFileUtils = FileUtils::getInstance();

    // reset search path
    sharedFileUtils->setSearchPaths(_defaultSearchPathArray);
    FileUtilsDemo::onExit();
}

std::string TestSearchPath::title() const
{
    return "FileUtils: search path";
}

std::string TestSearchPath::subtitle() const
{
    return "See the console, can see a orange box and a 'about' picture, except Android";
}

// TestIsFileExist

void TestIsFileExist::onEnter()
{
    FileUtilsDemo::onEnter();
    auto s               = Director::getInstance()->getWinSize();
    auto sharedFileUtils = FileUtils::getInstance();

    Label* label = nullptr;
    bool isExist = false;

    isExist = sharedFileUtils->isFileExist("Images/grossini.png");

    label = Label::createWithSystemFont(isExist ? "Images/grossini.png exists" : "Images/grossini.png doesn't exist",
                                        "", 20);
    label->setPosition(s.width / 2, s.height / 3);
    this->addChild(label);

    isExist = sharedFileUtils->isFileExist("Images/grossini.xcf");
    label   = Label::createWithSystemFont(isExist ? "Images/grossini.xcf exists" : "Images/grossini.xcf doesn't exist",
                                        "", 20);
    label->setPosition(s.width / 2, s.height / 3 * 2);
    this->addChild(label);
}

void TestIsFileExist::onExit()
{

    FileUtils* sharedFileUtils = FileUtils::getInstance();

    FileUtilsDemo::onExit();
}

std::string TestIsFileExist::title() const
{
    return "FileUtils: check whether the file exists";
}

std::string TestIsFileExist::subtitle() const
{
    return "";
}

// TestIsDirectoryExist

void TestIsDirectoryExist::onEnter()
{
    FileUtilsDemo::onEnter();
    auto s    = Director::getInstance()->getWinSize();
    auto util = FileUtils::getInstance();
    int x = s.width / 2, y = s.height / 3;

    Label* label = nullptr;
    std::string dir;
    auto getMsg = [&dir](bool b) -> std::string {
        char msg[512];
        snprintf((char*)msg, 512, "%s for dir: \"%s\"", b ? "success" : "failed", dir.c_str());
        return std::string(msg);
    };

    dir   = "Images";
    label = Label::createWithSystemFont(getMsg(util->isDirectoryExist(dir)), "", 20);
    label->setPosition(x, y * 2);
    this->addChild(label);

    dir   = util->getWritablePath();
    label = Label::createWithSystemFont(getMsg(util->isDirectoryExist(dir)), "", 20);
    label->setPosition(x, y * 1);
    this->addChild(label);

    dir   = util->getWritablePath();
    label = Label::createWithSystemFont(getMsg(util->isDirectoryExist(dir)), "", 20);
    label->setPosition(x, y * 1);
    this->addChild(label);
}

void TestIsDirectoryExist::onExit()
{

    FileUtils* sharedFileUtils = FileUtils::getInstance();

    // reset filename lookup
    sharedFileUtils->purgeCachedEntries();

    FileUtilsDemo::onExit();
}

std::string TestIsDirectoryExist::title() const
{
    return "FileUtils: check whether the directory exists";
}

std::string TestIsDirectoryExist::subtitle() const
{
    return "";
}

// TestFileFuncs

void TestFileFuncs::onEnter()
{
    FileUtilsDemo::onEnter();
    auto s               = Director::getInstance()->getWinSize();
    auto sharedFileUtils = FileUtils::getInstance();

    int x = s.width / 2, y = s.height / 5;
    Label* label = nullptr;

    std::string filename  = "__test.test";
    std::string filename2 = "__newtest.test";
    std::string filepath  = sharedFileUtils->getWritablePath() + filename;
    std::string content   = "Test string content to put into created file";
    std::string msg;

    FILE* out = fopen(filepath.c_str(), "w");
    fputs(content.c_str(), out);
    fclose(out);

    // Check whether file can be created
    if (sharedFileUtils->isFileExist(filepath))
    {
        label = Label::createWithSystemFont("Test file '__test.test' created", "", 20);
        label->setPosition(x, y * 4);
        this->addChild(label);

        // getFileSize Test
        int32_t size = sharedFileUtils->getFileSize(filepath);
        msg          = StringUtils::format("getFileSize: Test file size equals %d", size);
        label        = Label::createWithSystemFont(msg, "", 20);
        label->setPosition(x, y * 3);
        this->addChild(label);

        // renameFile Test
        if (sharedFileUtils->renameFile(sharedFileUtils->getWritablePath(), filename, filename2))
        {
            label = Label::createWithSystemFont("renameFile: Test file renamed to  '__newtest.test'", "", 20);
            label->setPosition(x, y * 2);
            this->addChild(label);

            // removeFile Test
            filepath = sharedFileUtils->getWritablePath() + filename2;
            if (sharedFileUtils->removeFile(filepath))
            {
                label = Label::createWithSystemFont("removeFile: Test file removed", "", 20);
                label->setPosition(x, y * 1);
                this->addChild(label);
            }
            else
            {
                label = Label::createWithSystemFont("removeFile: Failed to remove test file", "", 20);
                label->setPosition(x, y * 1);
                this->addChild(label);
            }
        }
        else
        {
            label = Label::createWithSystemFont(
                "renameFile: Failed to rename test file to  '__newtest.test', further test skipped", "", 20);
            label->setPosition(x, y * 2);
            this->addChild(label);
        }
    }
    else
    {
        label = Label::createWithSystemFont("Test file can not be created, test skipped", "", 20);
        label->setPosition(x, y * 4);
        this->addChild(label);
    }
}

std::string TestFileFuncs::title() const
{
    return "FileUtils: file control functions";
}

std::string TestFileFuncs::subtitle() const
{
    return "";
}

// TestDirectoryFuncs

void TestDirectoryFuncs::onEnter()
{
    FileUtilsDemo::onEnter();
    auto s               = Director::getInstance()->getWinSize();
    auto sharedFileUtils = FileUtils::getInstance();

    int x = s.width / 2, y = s.height / 4;
    Label* label = nullptr;

    std::string dir        = sharedFileUtils->getWritablePath() + "__test";
    std::string subDir     = "dir1/dir2";
    std::string fullSubDir = dir + "/" + subDir;
    std::string msg;
    bool ok;

    // Check whether dir can be created
    ok = sharedFileUtils->createDirectory(dir);
    if (ok && sharedFileUtils->isDirectoryExist(dir))
    {
        msg   = StringUtils::format("createDirectory: Directory '__test' created");
        label = Label::createWithSystemFont(msg, "", 20);
        label->setPosition(x, y * 3);
        this->addChild(label);

        // Create sub directories recursively
        ok = sharedFileUtils->createDirectory(fullSubDir);
        if (ok && sharedFileUtils->isDirectoryExist(fullSubDir))
        {
            msg   = StringUtils::format("createDirectory: Sub directories '%s' created", subDir.c_str());
            label = Label::createWithSystemFont(msg, "", 20);
            label->setPosition(x, y * 2);
            this->addChild(label);
        }
        else
        {
            msg   = StringUtils::format("createDirectory: Failed to create sub directories '%s'", subDir.c_str());
            label = Label::createWithSystemFont(msg, "", 20);
            label->setPosition(x, y * 2);
            this->addChild(label);
        }

        // Remove directory
        ok = sharedFileUtils->removeDirectory(dir);
        if (ok && !sharedFileUtils->isDirectoryExist(dir))
        {
            msg   = StringUtils::format("removeDirectory: Directory '__test' removed");
            label = Label::createWithSystemFont(msg, "", 20);
            label->setPosition(x, y);
            this->addChild(label);
        }
        else
        {
            msg   = StringUtils::format("removeDirectory: Failed to remove directory '__test'");
            label = Label::createWithSystemFont(msg, "", 20);
            label->setPosition(x, y);
            this->addChild(label);
        }
    }
    else
    {
        msg   = StringUtils::format("createDirectory: Directory '__test' can not be created");
        label = Label::createWithSystemFont(msg, "", 20);
        label->setPosition(x, y * 2);
        this->addChild(label);
    }
}

std::string TestDirectoryFuncs::title() const
{
    return "FileUtils: directory control functions";
}

std::string TestDirectoryFuncs::subtitle() const
{
    return "";
}

void TestWriteString::onEnter()
{
    FileUtilsDemo::onEnter();

    auto winSize = Director::getInstance()->getWinSize();

    auto writeResult = Label::createWithTTF("show writeResult", "fonts/Thonburi.ttf", 18);
    this->addChild(writeResult);
    writeResult->setPosition(winSize.width / 2, winSize.height * 3 / 4);

    auto readResult = Label::createWithTTF("show readResult", "fonts/Thonburi.ttf", 18);
    this->addChild(readResult);
    readResult->setPosition(winSize.width / 2, winSize.height / 3);

    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fileName     = "writeStringTest.txt";

    // writeTest
    std::string writeDataStr = "the string data will be write into a file";
    std::string fullPath     = writablePath + fileName;
    if (FileUtils::getInstance()->writeStringToFile(writeDataStr, fullPath.c_str()))
    {
        log("see the plist file at %s", fullPath.c_str());
        writeResult->setString("write success:" + writeDataStr);
    }
    else
    {
        log("write plist file failed");
        writeResult->setString("write fail");
    }

    // readTest
    std::string readDataStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    readResult->setString("read success:" + readDataStr);
}

void TestWriteString::onExit()
{
    FileUtilsDemo::onExit();
}

std::string TestWriteString::title() const
{
    return "FileUtils: TestWriteString to files";
}

std::string TestWriteString::subtitle() const
{
    return "";
}

static void saveAsBinaryText(std::string_view filename, const std::vector<char>& binary)
{
    auto fs = FileUtils::getInstance();
    std::string text(binary.begin(), binary.end());
    fs->writeStringToFile(text, filename);
}

static const std::string FileErrors[] = {
    "OK", "NotExists", "OpenFailed", "ReadFailed", "NotInitialized", "TooLarge", "ObtainSizeFailed",
};

void TestGetContents::onEnter()
{
    FileUtilsDemo::onEnter();
    auto fs = FileUtils::getInstance();

    auto winSize = Director::getInstance()->getWinSize();

    auto readResult = Label::createWithTTF("show readResult", "fonts/Thonburi.ttf", 16);
    this->addChild(readResult);
    readResult->setPosition(winSize.width / 2, winSize.height / 2);

    std::vector<char> binary = {'\r', '\n', '\r', '\n', '\0', '\0', '\r', '\n'};
    _generatedFile           = fs->getWritablePath() + "file-with-zeros-and-crlf";
    saveAsBinaryText(_generatedFile, binary);

    auto runTests = [&]() {
        // Test read string in binary mode
        std::string bs;
        fs->getContents(_generatedFile, &bs);
        if (bs.size() != binary.size() || !std::equal(bs.begin(), bs.end(), binary.begin()))
            return std::string("failed: read as binary string");

        // Text read string in text mode
        std::string ts = fs->getStringFromFile(_generatedFile);
        if (strcmp(ts.c_str(), "\r\n\r\n") != 0)
            return std::string("failed: read as zero terminated string");

        std::string files[] = {_generatedFile, "background.wav", "fileLookup.plist"};
        for (auto&& file : files)
        {
            std::string sbuf;

            auto serr = fs->getContents(file, &sbuf);
            if (serr != FileUtils::Status::OK)
                return std::string("failed: error: " + FileErrors[(int)serr]);

            std::vector<int> vbuf;
            auto verr = fs->getContents(file, &vbuf);
            if (verr != FileUtils::Status::OK)
                return std::string("failed: error: " + FileErrors[(int)verr]);

            Data dbuf;
            auto derr = fs->getContents(file, &dbuf);
            if (derr != FileUtils::Status::OK)
                return std::string("failed: error: " + FileErrors[(int)derr]);

            if (memcmp(&sbuf.front(), &vbuf.front(), sbuf.size()) != 0)
                return std::string("failed: error: sbuf != vbuf");

            if (dbuf.getSize() != sbuf.size())
                return std::string("failed: error: sbuf.size() != dbuf.getSize()");

            if (memcmp(&sbuf.front(), dbuf.getBytes(), sbuf.size()) != 0)
                return std::string("failed: error: sbuf != dbuf");
        }
        return std::string("read success");
    };
    readResult->setString("FileUtils::getContents() " + runTests());
}

void TestGetContents::onExit()
{
    if (!_generatedFile.empty())
        FileUtils::getInstance()->removeFile(_generatedFile);

    FileUtilsDemo::onExit();
}

std::string TestGetContents::title() const
{
    return "FileUtils: TestGetContents";
}

std::string TestGetContents::subtitle() const
{
    return "";
}

void TestWriteData::onEnter()
{
    FileUtilsDemo::onEnter();

    auto winSize = Director::getInstance()->getWinSize();

    auto writeResult = Label::createWithTTF("show writeResult", "fonts/Thonburi.ttf", 18);
    this->addChild(writeResult);
    writeResult->setPosition(winSize.width / 2, winSize.height * 3 / 4);

    auto readResult = Label::createWithTTF("show readResult", "fonts/Thonburi.ttf", 18);
    this->addChild(readResult);
    readResult->setPosition(winSize.width / 2, winSize.height / 3);

    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fileName     = "writeDataTest.txt";

    // writeTest
    std::string writeDataStr = "the binary data will be write into a file";
    Data writeData;
    writeData.copy((unsigned char*)writeDataStr.c_str(), writeDataStr.size());
    std::string fullPath = writablePath + fileName;
    if (FileUtils::getInstance()->writeDataToFile(writeData, fullPath.c_str()))
    {
        log("see the plist file at %s", fullPath.c_str());
        writeResult->setString("write success:" + writeDataStr);
    }
    else
    {
        log("write plist file failed");
        writeResult->setString("write fail");
    }

    // readTest
    unsigned char* buffer = nullptr;
    Data readData         = FileUtils::getInstance()->getDataFromFile(fullPath);
    buffer                = (unsigned char*)malloc(sizeof(unsigned char) * (readData.getSize() + 1));
    memcpy(buffer, readData.getBytes(), readData.getSize());
    buffer[readData.getSize()] = '\0';
    std::string readDataStr((const char*)buffer);
    free(buffer);

    readResult->setString("read success:" + readDataStr);
}

void TestWriteData::onExit()
{
    FileUtilsDemo::onExit();
}

std::string TestWriteData::title() const
{
    return "FileUtils: TestWriteData to files";
}

std::string TestWriteData::subtitle() const
{
    return "";
}

void TestWriteValueMap::onEnter()
{
    FileUtilsDemo::onEnter();

    auto winSize = Director::getInstance()->getWinSize();

    auto writeResult = Label::createWithTTF("show writeResult", "fonts/Thonburi.ttf", 18);
    this->addChild(writeResult);
    writeResult->setPosition(winSize.width / 2, winSize.height * 3 / 4);

    auto readResult = Label::createWithTTF("show readResult", "fonts/Thonburi.ttf", 18);
    this->addChild(readResult);
    readResult->setPosition(winSize.width / 2, winSize.height / 3);

    ValueMap valueMap;

    ValueMap mapInValueMap;
    mapInValueMap["string1"] = "string in dictInMap key 0";
    mapInValueMap["string2"] = "string in dictInMap key 1";
    mapInValueMap["none"].getType();

    valueMap["data0"] = Value(mapInValueMap);

    valueMap["data1"] = Value("string in array");

    ValueVector arrayInMap;
    arrayInMap.emplace_back(Value("string 0 in arrayInMap"));
    arrayInMap.emplace_back(Value("string 1 in arrayInMap"));
    valueMap["data2"] = arrayInMap;

    // add boolean to the plist
    auto booleanObject = Value(true);
    valueMap["data3"]  = booleanObject;

    // add integer to the plist
    auto intObject    = Value(1024);
    valueMap["data4"] = intObject;

    // add float to the plist
    auto floatObject  = Value(1024.1024f);
    valueMap["data5"] = floatObject;

    // add double to the plist
    auto doubleObject = Value(1024.123);
    valueMap["data6"] = doubleObject;

    // end with /
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fullPath     = writablePath + "testWriteValueMap.plist";
    if (FileUtils::getInstance()->writeValueMapToFile(valueMap, fullPath.c_str()))
    {
        log("see the plist file at %s", fullPath.c_str());
        writeResult->setString("write success");
    }
    else
    {
        log("write plist file failed");
        writeResult->setString("write failed");
    }

    ValueMap readValueMap   = FileUtils::getInstance()->getValueMapFromFile(fullPath.c_str());
    std::string readDataStr = "read data:\n";
    // read value map data
    ValueMap readMapInMap = readValueMap["data0"].asValueMap();
    readDataStr += "  mapValue:[\"string1\"][" + readMapInMap["string1"].asString() + "]\n";
    readDataStr += "  mapValue:[\"string2\"][" + readMapInMap["string2"].asString() + "]\n";

    // read string data
    readDataStr += "  stringValue:" + readValueMap["data1"].asString() + "\n";

    // read value vector data
    ValueVector readVectorInMap = readValueMap["data2"].asValueVector();
    readDataStr += "  vectorValue:[1]" + readVectorInMap.at(0).asString() + "\n";
    readDataStr += "  vectorValue:[2]" + readVectorInMap.at(1).asString() + "\n";

    // read bool data
    readDataStr += "  boolValue:" + StringUtils::format("%d", readValueMap["data3"].asBool()) + "\n";

    // read int data
    readDataStr += "  intValue:" + StringUtils::format("%d", readValueMap["data4"].asInt()) + "\n";

    // read float data
    readDataStr += "  floatValue:" + StringUtils::format("%f", readValueMap["data5"].asFloat()) + "\n";

    // read double data
    readDataStr += "  doubleValue:" + StringUtils::format("%f", readValueMap["data6"].asDouble()) + "\n";

    readResult->setString(readDataStr);
}
void TestWriteValueMap::onExit()
{
    FileUtilsDemo::onExit();
}

std::string TestWriteValueMap::title() const
{
    return "FileUtils: TestWriteValueMap to files";
}

std::string TestWriteValueMap::subtitle() const
{
    return "";
}

void TestWriteValueVector::onEnter()
{
    FileUtilsDemo::onEnter();

    auto winSize = Director::getInstance()->getWinSize();

    auto writeResult = Label::createWithTTF("show writeResult", "fonts/Thonburi.ttf", 18);
    this->addChild(writeResult);
    writeResult->setPosition(winSize.width / 2, winSize.height * 3 / 4);

    auto readResult = Label::createWithTTF("show readResult", "fonts/Thonburi.ttf", 18);
    this->addChild(readResult);
    readResult->setPosition(winSize.width / 2, winSize.height / 3);

    ValueVector array;

    ValueMap mapInArray;
    mapInArray["string1"] = "string in dictInArray key 0";
    mapInArray["string2"] = "string in dictInArray key 1";
    array.emplace_back(Value(mapInArray));

    array.emplace_back(Value("string in array"));

    ValueVector arrayInArray;
    arrayInArray.emplace_back(Value("string 0 in arrayInArray"));
    arrayInArray.emplace_back(Value("string 1 in arrayInArray"));
    array.emplace_back(Value(arrayInArray));

    // add boolean to the plist
    auto booleanObject = Value(true);
    array.emplace_back(booleanObject);

    // add integer to the plist
    auto intObject = Value(1024);
    array.emplace_back(intObject);

    // add float to the plist
    auto floatObject = Value(1024.1024f);
    array.emplace_back(floatObject);

    // add double to the plist
    auto doubleObject = Value(1024.123);
    array.emplace_back(doubleObject);

    // end with /
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fullPath     = writablePath + "testWriteValueVector.plist";
    if (FileUtils::getInstance()->writeValueVectorToFile(array, fullPath.c_str()))
    {
        log("see the plist file at %s", fullPath.c_str());
        writeResult->setString("write success");
    }
    else
    {
        log("write plist file failed");
        writeResult->setString("write failed");
    }

    ValueVector readArray   = FileUtils::getInstance()->getValueVectorFromFile(fullPath.c_str());
    std::string readDataStr = "read data:\n";
    // read value map data
    ValueMap readMapInArray = readArray.at(0).asValueMap();
    readDataStr += "  mapValue:[\"string1\"][" + readMapInArray["string1"].asString() + "]\n";
    readDataStr += "  mapValue:[\"string2\"][" + readMapInArray["string2"].asString() + "]\n";

    // read string data
    readDataStr += "  stringValue:" + readArray.at(1).asString() + "\n";

    // read value vector data
    ValueVector readVectorInArray = readArray.at(2).asValueVector();
    readDataStr += "  vectorValue:[1]" + readVectorInArray.at(0).asString() + "\n";
    readDataStr += "  vectorValue:[2]" + readVectorInArray.at(1).asString() + "\n";

    // read bool data
    readDataStr += "  boolValue:" + StringUtils::format("%d", readArray.at(3).asBool()) + "\n";

    // read int data
    readDataStr += "  intValue:" + StringUtils::format("%d", readArray.at(4).asInt()) + "\n";

    // read float data
    readDataStr += "  floatValue:" + StringUtils::format("%f", readArray.at(5).asFloat()) + "\n";

    // read double data
    readDataStr += "  doubleValue:" + StringUtils::format("%f", readArray.at(6).asDouble()) + "\n";

    readResult->setString(readDataStr);
}

void TestWriteValueVector::onExit()
{
    FileUtilsDemo::onExit();
}

std::string TestWriteValueVector::title() const
{
    return "FileUtils: TestWriteValueVector to files";
}

std::string TestWriteValueVector::subtitle() const
{
    return "";
}

// TestUnicodePath

void TestUnicodePath::onEnter()
{
    FileUtilsDemo::onEnter();
    auto s    = Director::getInstance()->getWinSize();
    auto util = FileUtils::getInstance();

    int x = s.width / 2, y = s.height / 5;
    Label* label = nullptr;

    std::string dir      = "中文路径/";
    std::string filename = "测试文件.test";

    std::string act;
    auto getMsg = [&act](bool b, std::string_view path) -> std::string {
        char msg[512];
        snprintf((char*)msg, 512, "%s for %s path: \"%s\"", b ? "success" : "failed", act.c_str(), path.data());
        return std::string(msg);
    };

    // Check whether unicode dir should be create or not
    std::string dirPath = util->getWritablePath() + dir;
    if (!util->isDirectoryExist(dirPath))
    {
        util->createDirectory(dirPath);
    }

    act          = "create";
    bool isExist = util->isDirectoryExist(dirPath);
    label        = Label::createWithSystemFont(getMsg(isExist, dirPath), "", 12, Size(s.width, 0));
    label->setPosition(x, y * 4);
    this->addChild(label);

    if (isExist)
    {
        // Check whether unicode file should be create or not
        std::string filePath = dirPath + filename;
        if (!util->isFileExist(filePath))
        {
            std::string writeDataStr = " 测试字符串.";
            Data writeData;
            writeData.copy((unsigned char*)writeDataStr.c_str(), writeDataStr.size());
            util->writeDataToFile(writeData, filePath);
        }

        isExist = util->isFileExist(filePath);
        label   = Label::createWithSystemFont(getMsg(isExist, filePath), "", 12, Size(s.width, 0));
        label->setPosition(x, y * 3);
        this->addChild(label);

        act = "remove";
        if (isExist)
        {
            // read file content and log it
            unsigned char* buffer = nullptr;
            Data readData         = util->getDataFromFile(filePath);
            buffer                = (unsigned char*)malloc(sizeof(unsigned char) * (readData.getSize() + 1));
            memcpy(buffer, readData.getBytes(), readData.getSize());
            buffer[readData.getSize()] = '\0';
            // vc can't treat unicode string correctly, don't use unicode string in code
            log("The content of file from writable path: %s", buffer);
            free(buffer);

            // remove test file
            label = Label::createWithSystemFont(getMsg(util->removeFile(filePath), filePath), "", 12, Size(s.width, 0));
            label->setPosition(x, y * 2);
            this->addChild(label);
        }

        // remove test dir
        label = Label::createWithSystemFont(getMsg(util->removeDirectory(dirPath), dirPath), "", 12, Size(s.width, 0));
        label->setPosition(x, y * 1);
        this->addChild(label);
    }
}

void TestUnicodePath::onExit()
{

    FileUtils* sharedFileUtils = FileUtils::getInstance();
    sharedFileUtils->purgeCachedEntries();
    FileUtilsDemo::onExit();
}

std::string TestUnicodePath::title() const
{
    return "FileUtils: check unicode path";
}

std::string TestUnicodePath::subtitle() const
{
    return "";
}

// TestIsFileExist

void TestIsFileExistAsync::onEnter()
{
    FileUtilsDemo::onEnter();
    auto s               = Director::getInstance()->getWinSize();
    auto sharedFileUtils = FileUtils::getInstance();

    sharedFileUtils->isFileExist("Images/grossini.png", [=](bool isExist) {
        AXASSERT(std::this_thread::get_id() == Director::getInstance()->getAxmolThreadId(),
                 "Callback should be on cocos thread");
        auto label = Label::createWithSystemFont(
            isExist ? "Images/grossini.png exists" : "Images/grossini.png doesn't exist", "", 20);
        label->setPosition(s.width / 2, s.height / 3);
        this->addChild(label);

        isExist = sharedFileUtils->isFileExist("Images/grossini.xcf");
        label   = Label::createWithSystemFont(
              isExist ? "Images/grossini.xcf exists" : "Images/grossini.xcf doesn't exist", "", 20);
        label->setPosition(s.width / 2, s.height / 3 * 2);
        this->addChild(label);
    });
}

void TestIsFileExistAsync::onExit()
{
    FileUtilsDemo::onExit();
}

std::string TestIsFileExistAsync::title() const
{
    return "FileUtilsAsync: check whether the file exists";
}

std::string TestIsFileExistAsync::subtitle() const
{
    return "";
}

void TestIsDirectoryExistAsync::onEnter()
{
    FileUtilsDemo::onEnter();
    auto s    = Director::getInstance()->getWinSize();
    auto util = FileUtils::getInstance();
    int x = s.width / 2, y = s.height / 3;

    std::string dir;
    auto getMsg = [](bool b, std::string_view dir) -> std::string {
        char msg[512];
        snprintf((char*)msg, 512, "%s for dir: \"%s\"", b ? "success" : "failed", dir.data());
        return std::string(msg);
    };

    dir = util->getWritablePath();
    util->isDirectoryExist(dir, [=](bool exists) {
        CCAssert(exists, "Writable path should exist");
        auto label = Label::createWithSystemFont(getMsg(exists, dir), "", 20);
        label->setPosition(x, y * 2);
        this->addChild(label);
    });
}

void TestIsDirectoryExistAsync::onExit()
{

    FileUtils* sharedFileUtils = FileUtils::getInstance();

    // reset filename lookup
    sharedFileUtils->purgeCachedEntries();

    FileUtilsDemo::onExit();
}

std::string TestIsDirectoryExistAsync::title() const
{
    return "FileUtilsAsync: check whether the directory exists";
}

std::string TestIsDirectoryExistAsync::subtitle() const
{
    return "";
}

void TestFileFuncsAsync::onEnter()
{
    FileUtilsDemo::onEnter();
    auto s               = Director::getInstance()->getWinSize();
    auto sharedFileUtils = FileUtils::getInstance();

    int x = s.width / 2, y = s.height / 5;

    std::string filename  = "__test.test";
    std::string filename2 = "__newtest.test";
    std::string filepath  = sharedFileUtils->getWritablePath() + filename;
    std::string content   = "Test string content to put into created file";
    std::string msg;

    FILE* out = fopen(filepath.c_str(), "w");
    fputs(content.c_str(), out);
    fclose(out);

    sharedFileUtils->isFileExist(filepath, [=](bool exists) {
        AXASSERT(exists, "File could not be found");
        auto label = Label::createWithSystemFont("Test file '__test.test' created", "", 20);
        label->setPosition(x, y * 4);
        this->addChild(label);

        sharedFileUtils->getFileSize(filepath, [=](int32_t size) {
            auto msg   = StringUtils::format("getFileSize: Test file size equals %d", size);
            auto label = Label::createWithSystemFont(msg, "", 20);
            label->setPosition(x, y * 3);
            this->addChild(label);

            sharedFileUtils->renameFile(sharedFileUtils->getWritablePath(), filename, filename2, [=](bool success) {
                AXASSERT(success, "Was not able to properly rename file");
                auto label = Label::createWithSystemFont("renameFile: Test file renamed to  '__newtest.test'", "", 20);
                label->setPosition(x, y * 2);
                this->addChild(label);

                sharedFileUtils->removeFile(sharedFileUtils->getWritablePath() + filename2, [=](bool success) {
                    AXASSERT(success, "Was not able to remove file");
                    auto label = Label::createWithSystemFont("removeFile: Test file removed", "", 20);
                    label->setPosition(x, y * 1);
                    this->addChild(label);
                });
            });
        });
    });
}

std::string TestFileFuncsAsync::title() const
{
    return "FileUtilsAsync: file control functions";
}

std::string TestFileFuncsAsync::subtitle() const
{
    return "";
}

void TestWriteStringAsync::onEnter()
{
    FileUtilsDemo::onEnter();

    auto winSize = Director::getInstance()->getWinSize();

    auto writeResult = Label::createWithTTF("show writeResult", "fonts/Thonburi.ttf", 18);
    this->addChild(writeResult);
    writeResult->setPosition(winSize.width / 2, winSize.height * 3 / 4);

    auto readResult = Label::createWithTTF("show readResult", "fonts/Thonburi.ttf", 18);
    this->addChild(readResult);
    readResult->setPosition(winSize.width / 2, winSize.height / 3);

    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fileName     = "writeStringTest.txt";

    // writeTest
    std::string writeDataStr = "the string data will be write into a file";
    std::string fullPath     = writablePath + fileName;

    FileUtils::getInstance()->writeStringToFile(writeDataStr, fullPath, [=](bool success) {
        AXASSERT(success, "Write String to data failed");
        writeResult->setString("write success:" + writeDataStr);

        FileUtils::getInstance()->getStringFromFile(fullPath, [=](std::string_view value) {
            AXASSERT(!value.empty(), "String should be readable");

            std::string strVal = "read success: ";
            readResult->setString(strVal.append(value));
        });
    });
}

void TestWriteStringAsync::onExit()
{
    FileUtilsDemo::onExit();
}

std::string TestWriteStringAsync::title() const
{
    return "FileUtilsAsync: TestWriteString to files";
}

std::string TestWriteStringAsync::subtitle() const
{
    return "";
}

void TestWriteDataAsync::onEnter()
{
    FileUtilsDemo::onEnter();

    auto winSize = Director::getInstance()->getWinSize();

    auto writeResult = Label::createWithTTF("show writeResult", "fonts/Thonburi.ttf", 18);
    this->addChild(writeResult);
    writeResult->setPosition(winSize.width / 2, winSize.height * 3 / 4);

    auto readResult = Label::createWithTTF("show readResult", "fonts/Thonburi.ttf", 18);
    this->addChild(readResult);
    readResult->setPosition(winSize.width / 2, winSize.height / 3);

    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fileName     = "writeDataTest.txt";

    // writeTest
    std::string writeDataStr = "the binary data will be write into a file";
    Data writeData;
    writeData.copy((unsigned char*)writeDataStr.c_str(), writeDataStr.size());
    std::string fullPath = writablePath + fileName;

    FileUtils::getInstance()->writeDataToFile(writeData, fullPath, [=](bool success) {
        if (success)
        {
            writeResult->setString("Write result success");
        }
        else
        {
            writeResult->setString("Write result failure");
        }

        FileUtils::getInstance()->getDataFromFile(fullPath, [=](const Data& readData) {
            auto buffer = (unsigned char*)malloc(sizeof(unsigned char) * (readData.getSize() + 1));
            memcpy(buffer, readData.getBytes(), readData.getSize());
            buffer[readData.getSize()] = '\0';
            std::string readDataStr((const char*)buffer);
            free(buffer);

            readResult->setString("read success:" + readDataStr);
        });
    });
}

void TestWriteDataAsync::onExit()
{
    FileUtilsDemo::onExit();
}

std::string TestWriteDataAsync::title() const
{
    return "FileUtilsAsync: TestWriteData to files";
}

std::string TestWriteDataAsync::subtitle() const
{
    return "";
}

void TestListFiles::onEnter()
{
    FileUtilsDemo::onEnter();

    auto winSize = Director::getInstance()->getWinSize();

    auto infoLabel = Label::createWithTTF("show file count, should not be 0", "fonts/Thonburi.ttf", 18);
    this->addChild(infoLabel);
    infoLabel->setPosition(winSize.width / 2, winSize.height * 3 / 4);

    auto cntLabel = Label::createWithTTF("show readResult", "fonts/Thonburi.ttf", 18);
    this->addChild(cntLabel);
    cntLabel->setPosition(winSize.width / 2, winSize.height / 3);
    // writeTest
    std::vector<std::string> listFonts = FileUtils::getInstance()->listFiles("fonts");
    auto defaultPath                   = FileUtils::getInstance()->getDefaultResourceRootPath();
    std::vector<std::string> list      = FileUtils::getInstance()->listFiles(defaultPath);

    char cntBuffer[200] = {0};
    snprintf(cntBuffer, 200, "'fonts/' %zu, $defaultResourceRootPath %zu", listFonts.size(), list.size());

    for (int i = 0; i < listFonts.size(); i++)
    {
        AXLOG("fonts/ %d: \t %s", i, listFonts[i].c_str());
    }

    for (int i = 0; i < list.size(); i++)
    {
        AXLOG("defResRootPath %d: \t %s", i, list[i].c_str());
    }

    cntLabel->setString(cntBuffer);
}

void TestListFiles::onExit()
{
    FileUtilsDemo::onExit();
}

std::string TestListFiles::title() const
{
    return "FileUtils: list files of directory";
}

std::string TestListFiles::subtitle() const
{
    return "";
}

void TestIsFileExistRejectFolder::onEnter()
{
    FileUtilsDemo::onEnter();

    auto winSize = Director::getInstance()->getWinSize();

    auto infoLabel = Label::createWithTTF("tests folder 'NavMesh/maps', expect to be false", "fonts/Thonburi.ttf", 18);
    this->addChild(infoLabel);
    infoLabel->setPosition(winSize.width / 2, winSize.height * 3 / 4);

    auto cntLabel = Label::createWithTTF("waiting...", "fonts/Thonburi.ttf", 18);
    this->addChild(cntLabel);
    cntLabel->setPosition(winSize.width / 2, winSize.height / 3);

    auto exists      = FileUtils::getInstance()->isFileExist("NavMesh/maps");
    auto isDirectory = FileUtils::getInstance()->isDirectoryExist("NavMesh/maps");

    char cntBuffer[200] = {0};
    snprintf(cntBuffer, 200, "isDir: %s, isFile: %s", isDirectory ? "true" : "false", exists ? "true" : "false");
    cntLabel->setString(cntBuffer);
}

void TestIsFileExistRejectFolder::onExit()
{
    FileUtilsDemo::onExit();
}

std::string TestIsFileExistRejectFolder::title() const
{
    return "FileUtils: isFileExist(direname)";
}

std::string TestIsFileExistRejectFolder::subtitle() const
{
    return "";
}
