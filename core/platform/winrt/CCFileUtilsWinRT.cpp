/****************************************************************************
Copyright (c) 2010 cocos2d-x.org
Copyright (c) Microsoft Open Technologies, Inc.
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
#include "platform/winrt/CCFileUtilsWinRT.h"
#include <regex>
#include "platform/winrt/CCWinRTUtils.h"
#include "platform/CCCommon.h"
#include "ntcvt/ntcvt.hpp"

NS_AX_BEGIN

#define AX_MAX_PATH 512

static std::string s_pszResourcePath;

// D:\aaa\bbb\ccc\ddd\abc.txt --> D:/aaa/bbb/ccc/ddd/abc.txt
// D:\aaa\bbb\ccc\ddd\abc.txt --> D:/aaa/bbb/ccc/ddd/abc.txt
static std::string convertPathFormatToUnixStyle(const std::string_view& path)
{
    std::string ret{path};
    std::replace(ret.begin(), ret.end(), '\\', '/');
    return ret;
}

static std::string convertPathFormatToWinStyle(const std::string_view& path)
{
    std::string ret{path};
    std::replace(ret.begin(), ret.end(), '/', '\\');
    return ret;
}

static void _checkPath()
{
    if (s_pszResourcePath.empty())
    {
        // TODO: needs to be tested
        s_pszResourcePath = FileUtilsWinRT::getAppPath() + '\\' + "Content\\";
        std::replace(s_pszResourcePath.begin(), s_pszResourcePath.end(), '\\', '/');
    }
}

FileUtils* FileUtils::getInstance()
{
    if (s_sharedFileUtils == nullptr)
    {
        s_sharedFileUtils = new FileUtilsWinRT();
        if(!s_sharedFileUtils->init())
        {
          delete s_sharedFileUtils;
          s_sharedFileUtils = nullptr;
          AXLOG("ERROR: Could not init FileUtilsWinRT");
        }
    }
    return s_sharedFileUtils;
}

std::string FileUtilsWinRT::getNativeWritableAbsolutePath() const {
    if (!_writablePath.empty())
    {
        return _writablePath;
    }

    auto localFolderPath = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
    std::string ret      = PlatformStringToString(localFolderPath);
    ret += '\\';
    std::replace(ret.begin(), ret.end(), '\\', '/');
    return ret;
}

FileUtilsWinRT::FileUtilsWinRT()
{
}

bool FileUtilsWinRT::init()
{
    _checkPath();
    _defaultResRootPath = s_pszResourcePath;
    return FileUtils::init();
}

std::string FileUtilsWinRT::getPathForFilename(std::string_view filename, std::string_view resolutionDirectory) const
{
    std::string unixFileName = convertPathFormatToUnixStyle(filename);
    std::string unixResolutionDirectory = convertPathFormatToUnixStyle(resolutionDirectory);
    // std::string unixSearchPath = convertPathFormatToUnixStyle(searchPath);

    return FileUtils::getPathForFilename(unixFileName, unixResolutionDirectory);
}

std::string FileUtilsWinRT::getFullPathForFilenameWithinDirectory(std::string_view strDirectory, std::string_view strFilename) const
{
    std::string unixDirectory = convertPathFormatToUnixStyle(strDirectory);
    std::string unixFilename = convertPathFormatToUnixStyle(strFilename);
    return FileUtils::getFullPathForFilenameWithinDirectory(unixDirectory, unixFilename);
}

int64_t FileUtilsWinRT::getFileSize(std::string_view filepath) const
{
    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (!GetFileAttributesEx(ntcvt::from_chars(filepath).c_str(), GetFileExInfoStandard, &fad))
    {
        return 0; // error condition, could call GetLastError to find out more
    }
    LARGE_INTEGER size;
    size.HighPart = fad.nFileSizeHigh;
    size.LowPart = fad.nFileSizeLow;
    return (long)size.QuadPart;
}

FileUtils::Status FileUtilsWinRT::getContents(std::string_view filename, ResizableBuffer* buffer) const
{
    if (filename.empty())
        return FileUtils::Status::NotExists;

    // read the file from hardware
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);

    HANDLE fileHandle = ::CreateFile2(ntcvt::from_chars(fullPath).c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING, nullptr);
    if (fileHandle == INVALID_HANDLE_VALUE)
        return FileUtils::Status::OpenFailed;

    FILE_STANDARD_INFO info = {0};
    if (::GetFileInformationByHandleEx(fileHandle, FileStandardInfo, &info, sizeof(info)) == 0)
    {
        ::CloseHandle(fileHandle);
        return FileUtils::Status::OpenFailed;
    }

    if (info.EndOfFile.HighPart > 0)
    {
        ::CloseHandle(fileHandle);
        return FileUtils::Status::TooLarge;
    }

    buffer->resize(info.EndOfFile.LowPart);
    DWORD sizeRead = 0;
    BOOL successed = ::ReadFile(fileHandle, buffer->buffer(), info.EndOfFile.LowPart, &sizeRead, nullptr);
    ::CloseHandle(fileHandle);

    if (!successed)
    {
        buffer->resize(sizeRead);
        AXLOG("Get data from file(%s) failed, error code is %s", filename.data(), std::to_string(::GetLastError()).data());
        return FileUtils::Status::ReadFailed;
    }
    return FileUtils::Status::OK;
}

bool FileUtilsWinRT::isFileExistInternal(std::string_view strFilePath) const
{
    std::string strPath{strFilePath};
    if (!isAbsolutePath(strPath))
    {  // Not absolute path, add the default root path at the beginning.
        strPath.insert(0, _defaultResRootPath);
    }

    DWORD attr = GetFileAttributesW(ntcvt::from_chars(strPath).c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}

bool FileUtilsWinRT::isDirectoryExistInternal(std::string_view dirPath) const
{
    uint32_t fAttrib = GetFileAttributesW(ntcvt::from_chars(dirPath).c_str());
    return (fAttrib != INVALID_FILE_ATTRIBUTES && (fAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool FileUtilsWinRT::createDirectory(std::string_view path) const
{
    AXASSERT(!path.empty(), "Invalid path");

    if (isDirectoryExist(path))
        return true;

    // Split the path
    size_t start = 0;
    size_t found = path.find_first_of("/\\", start);
    std::string subpath;
    std::vector<std::string> dirs;

    if (found != std::string::npos)
    {
        while (true)
        {
            subpath = path.substr(start, found - start + 1);
            if (!subpath.empty())
                dirs.push_back(subpath);
            start = found + 1;
            found = path.find_first_of("/\\", start);
            if (found == std::string::npos)
            {
                if (start < path.length())
                {
                    dirs.emplace_back(path.substr(start));
                }
                break;
            }
        }
    }

    WIN32_FILE_ATTRIBUTE_DATA wfad;

    if (!(GetFileAttributesEx(ntcvt::from_chars(path).c_str(), GetFileExInfoStandard, &wfad)))
    {
        subpath = "";
        for (unsigned int i = 0, size = dirs.size(); i < size; ++i)
        {
            subpath += dirs[i];
            if (i > 0 && !isDirectoryExist(subpath))
            {
                BOOL ret = CreateDirectory(ntcvt::from_chars(subpath).c_str(), NULL);
                if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool FileUtilsWinRT::removeDirectory(std::string_view path) const
{
    std::wstring wpath = ntcvt::from_chars(path);
    std::wstring files = wpath + L"*.*";
    WIN32_FIND_DATA wfd;
    HANDLE  search = FindFirstFileEx(files.c_str(), FindExInfoStandard, &wfd, FindExSearchNameMatch, NULL, 0);
    bool ret = true;
    if (search != INVALID_HANDLE_VALUE)
    {
        BOOL find = true;
        while (find)
        {
            //. ..
            if (wfd.cFileName[0] != '.')
            {
                std::wstring temp = wpath + wfd.cFileName;
                if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    temp += '/';
                    ret = ret && this->removeDirectory(ntcvt::from_chars(temp));
                }
                else
                {
                    SetFileAttributes(temp.c_str(), FILE_ATTRIBUTE_NORMAL);
                    ret = ret && DeleteFile(temp.c_str());
                }
            }
            find = FindNextFile(search, &wfd);
        }
        FindClose(search);
    }
    if (ret && RemoveDirectory(wpath.c_str()))
    {
        return true;
    }
    return false;
}

bool FileUtilsWinRT::isAbsolutePath(std::string_view strPath) const
{
    if (   strPath.length() > 2
        && ( (strPath[0] >= 'a' && strPath[0] <= 'z') || (strPath[0] >= 'A' && strPath[0] <= 'Z') )
        && strPath[1] == ':')
    {
        return true;
    }
    return false;
}

bool FileUtilsWinRT::removeFile(std::string_view path) const
{
    std::wstring wpath = ntcvt::from_chars(path);
    if (DeleteFile(wpath.c_str()))
    {
        return true;
    }
    else
    {
        AXLOG("Remove file failed with error: %d", GetLastError());
        return false;
    }
}

bool FileUtilsWinRT::renameFile(std::string_view oldfullpath, std::string_view newfullpath) const
{
    AXASSERT(!oldfullpath.empty(), "Invalid path");
    AXASSERT(!newfullpath.empty(), "Invalid path");

    std::regex pat("\\/");
    std::string _oldfullpath{oldfullpath};
    std::string _newfullpath{newfullpath};
    convertPathFormatToWinStyle(_oldfullpath);
    convertPathFormatToWinStyle(_newfullpath);

    std::wstring _wNewfullpath = ntcvt::from_chars(_newfullpath);

    if (FileUtils::getInstance()->isFileExist(_newfullpath))
    {
        if (!DeleteFile(_wNewfullpath.c_str()))
        {
            AXLOGERROR("Fail to delete file %s !Error code is 0x%x", newfullpath.data(), GetLastError());
        }
    }

    if (MoveFileEx(ntcvt::from_chars(_oldfullpath).c_str(), _wNewfullpath.c_str(),
        MOVEFILE_REPLACE_EXISTING & MOVEFILE_WRITE_THROUGH))
    {
        return true;
    }
    else
    {
        AXLOGERROR("Fail to rename file %s to %s !Error code is 0x%x", oldfullpath.data(), newfullpath.data(), GetLastError());
        return false;
    }
}

bool FileUtilsWinRT::renameFile(std::string_view path, std::string_view oldname, std::string_view name) const
{
    AXASSERT(!path.empty(), "Invalid path");
    std::string oldPath{path};
    oldPath += oldname;
    std::string newPath{path};
    newPath += name;

    return renameFile(oldPath, newPath);
}

std::string FileUtilsWinRT::getWritablePath() const
{
    return getNativeWritableAbsolutePath();
}

std::string FileUtilsWinRT::getAppPath()
{
    Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
    return convertPathFormatToUnixStyle(std::string(PlatformStringToString(package->InstalledLocation->Path)));
}

NS_AX_END
