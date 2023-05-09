#!/usr/bin/python
# coding=utf-8
"""****************************************************************************
Copyright (c) 2014 cocos2d-x.org
Copyright (c) 2021-2022 Bytedance Inc.

https://axmolengine.github.io/axmol

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
****************************************************************************"""

'''
This script will install environment variables needed to by axmol. It will set these envrironment variables:
* AX_CONSOLE_ROOT: used to run axmol console tools
* ANDROID_HOME: used to generate applicatoin on Android through commands
* AX_ROOT: path where axmol is installed

On Max OS X, when start a shell, it will read these files and execute commands in sequence:

~/.bash_profile
~/.bash_login
~/.profile

And it will read only one of them. So we will add environment variable in the same sequence.
Which means that
* add environment variables into ~/.bash_profile if it exists
* otherwise it will the add environment variables into ~/.bash_login if it exists
* otherwise it will the add environment variables into ~/.profile if it exists

Will create ~/.bash_profile when none of them exist, and add environment variables into it.
'''

import os
import sys
import fileinput
import shutil
import subprocess
import ssl
try:
    ssl._create_default_https_context = ssl._create_unverified_context
    print("==> setup.py set ssl context ok")
except Exception:
    pass
from optparse import OptionParser
from time import time
from time import sleep
from os.path import dirname

AX_ROOT = 'AX_ROOT'
AX_CONSOLE_ROOT = 'AX_CONSOLE_ROOT'

ANDROID_HOME = 'ANDROID_HOME'

def _check_python_version():
    major_ver = sys.version_info[0]
    if major_ver < 2:
        print ("The python version is %d.%d. But python 2.x+ is required. (Version 2.7 is well tested)\n"
               "Download it here: https://www.python.org/" % (major_ver, sys.version_info[1]))
        return False

    return True


class SetEnvVar(object):

    RESULT_UPDATE_FAILED = -2
    RESULT_ADD_FAILED = -1
    RESULT_DO_NOTHING = 0
    RESULT_UPDATED = 1
    RESULT_ADDED = 2

    MAC_CHECK_FILES = ['.bash_profile', '.bash_login', '.profile']
    LINUX_CHECK_FILES = ['.bashrc']
    ZSH_CHECK_FILES = ['.zshrc']
    RE_FORMAT = r'^export[ \t]+%s=(.+)'

    def __init__(self):
        self.need_backup = True
        self.backup_file = None
        self.current_absolute_path = os.path.dirname(
            os.path.realpath(__file__))
        self.file_used_for_setup = ''

    def _isWindows(self):
        return sys.platform == 'win32'

    def _isLinux(self):
        return sys.platform.startswith('linux')

    def _is_mac(self):
        return sys.platform == 'darwin'

    def _is_zsh(self):
        shellItem = os.environ.get('SHELL')
        if shellItem is not None:
            if len(shellItem) >= 3:
                return shellItem[-3:] == "zsh"
        return False

    def _get_unix_file_list(self):
        file_list = None

        if self._is_zsh():
            file_list = SetEnvVar.ZSH_CHECK_FILES
        elif self._isLinux():
            file_list = SetEnvVar.LINUX_CHECK_FILES
        elif self._is_mac():
            file_list = SetEnvVar.MAC_CHECK_FILES

        return file_list

    def _get_filepath_for_setup(self):
        file_list = self._get_unix_file_list()

        file_to_write = None
        if file_list is None:
            return ''

        home = os.path.expanduser('~')
        for file_name in file_list:
            file_path = os.path.join(home, file_name)
            if os.path.exists(file_path):
                file_to_write = file_path
                break

        if file_to_write is None:
            self.need_backup = False
            file_to_write = os.path.join(home, file_list[0])
            file_obj = open(file_to_write, 'w')
            file_obj.close()

        return file_to_write

    # modify registry table to add an environment variable on windows
    def _set_environment_variable_win32(self, key, value):
        ret = False
        try:
            env = None
            env = _winreg.OpenKeyEx(_winreg.HKEY_CURRENT_USER,
                                    'Environment',
                                    0,
                                    _winreg.KEY_SET_VALUE | _winreg.KEY_READ)
            _winreg.SetValueEx(env, key, 0, _winreg.REG_SZ, value)
            _winreg.FlushKey(env)
            _winreg.CloseKey(env)
            ret = True
        except Exception:
            if env:
                _winreg.CloseKey(env)
            ret = False

        return ret

    def _gen_backup_file(self):
        file_name = os.path.basename(self.file_used_for_setup)
        file_path = os.path.dirname(self.file_used_for_setup)
        backup_file_name = file_name + ".backup"
        path = os.path.join(file_path, backup_file_name)
        i = 1
        while os.path.exists(path):
            backup_file_name = file_name + ".backup%d" % i
            path = os.path.join(file_path, backup_file_name)
            i += 1

        return path

    def _set_environment_variable_unix(self, key, value):

        if self.need_backup:
            # backup the environment file
            self.backup_file = self._gen_backup_file()
            shutil.copy(self.file_used_for_setup, self.backup_file)
            self.need_backup = False

        file = open(self.file_used_for_setup, 'a')
        file.write('\n# Add environment variable %s for axmol\n' % key)
        file.write('export %s="%s"\n' % (key, value))
        file.write('export PATH=$%s:$PATH\n' % key)
        if key == ANDROID_HOME:
            file.write(
                'export PATH=$%s/tools:$%s/platform-tools:$PATH\n' % (key, key))
        file.close()
        return True

    def _set_environment_variable(self, key, value):

        print("  -> Add %s environment variable..." % key)
        ret = False
        if self._isWindows():
            ret = self._set_environment_variable_win32(key, value)
        else:
            ret = self._set_environment_variable_unix(key, value)

        if ret:
            print("    ->Added %s=%s\n" % (key, value))
        else:
            print("    ->Add failed\n")

        return ret

    def _search_unix_variable(self, var_name, file_name):
        if not os.path.isfile(file_name):
            return None

        import re
        str_re = SetEnvVar.RE_FORMAT % var_name
        patten = re.compile(str_re)
        ret = None
        for line in open(file_name):
            str1 = line.lstrip(' \t')
            match = patten.match(str1)
            if match is not None:
                ret = match.group(1)

        return ret

    def _find_environment_variable(self, var):
        print("  ->Search for environment variable %s..." % var)
        ret = None
        try:
            ret = os.environ[var]
        except Exception:
            if not self._isWindows():
                file_list = self._get_unix_file_list()

                if file_list is not None:
                    home = os.path.expanduser('~')
                    for name in file_list:
                        path = os.path.join(home, name)
                        ret = self._search_unix_variable(var, path)
                        if ret is not None:
                            break
            else:
                try:
                    env = None
                    env = _winreg.OpenKeyEx(_winreg.HKEY_CURRENT_USER,
                                            'Environment',
                                            0,
                                            _winreg.KEY_READ)

                    ret = _winreg.QueryValueEx(env, var)[0]
                    _winreg.CloseKey(env)
                except Exception:
                    if env:
                        _winreg.CloseKey(env)
                    ret = None

        if ret is None:
            print("    ->%s not found\n" % var)
        else:
            print("    ->%s is found : %s\n" % (var, ret))

        return ret

    def _get_input_value(self, var_name):
        if sys.version_info.major >= 3:
            ret = input(
                '  ->Please enter the path of %s (or press Enter to skip):' % var_name)
        else:
            ret = raw_input(
                '  ->Please enter the path of %s (or press Enter to skip):' % var_name)
        ret.rstrip(" \t")
        return ret


    def _check_valid(self, var_name, value):
        ret = False
        if var_name == ANDROID_HOME:
            ret = self._is_android_sdk_root_valid(value)
        else:
            ret = False

        if not ret:
            print(
                '    ->Error: "%s" is not a valid path of %s. Ignoring it.' % (value, var_name))

        return ret

    def _is_android_sdk_root_valid(self, android_sdk_root):
        if not android_sdk_root:
            return False

        if self._isWindows():
            android_path = os.path.join(
                android_sdk_root, 'tools', 'android.bat')
        else:
            android_path = os.path.join(android_sdk_root, 'tools', 'android')
        if os.path.isfile(android_path):
            return True
        else:
            return False

    def remove_dir_from_win_path(self, remove_dir):
        try:
            env = None
            path = None
            env = _winreg.OpenKeyEx(_winreg.HKEY_CURRENT_USER,
                                    'Environment',
                                    0,
                                    _winreg.KEY_SET_VALUE | _winreg.KEY_READ)
            path = _winreg.QueryValueEx(env, 'Path')[0]

            path_lower = path.lower()
            remove_dir = remove_dir.replace('/', '\\')
            remove_dir_lower = remove_dir.lower()
            start_pos = path_lower.find(remove_dir_lower)
            if (start_pos >= 0):
                length = len(remove_dir_lower)
                need_remove = path[start_pos:(start_pos + length)]
                path = path.replace(need_remove, '')
                path = path.replace(';;', ';')
                _winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
                _winreg.FlushKey(env)
            _winreg.CloseKey(env)

            print('  ->Remove directory \"%s\" from PATH!\n' % remove_dir)
        except Exception:
            print('  ->Remove directory \"%s\" from PATH failed!\n' %
                  remove_dir)

    def set_windows_path(self, add_dir):
        ret = False
        try:
            env = None
            path = None
            env = _winreg.OpenKeyEx(_winreg.HKEY_CURRENT_USER,
                                    'Environment',
                                    0,
                                    _winreg.KEY_SET_VALUE | _winreg.KEY_READ)
            path = _winreg.QueryValueEx(env, 'Path')[0]

            # add variable if can't find it in PATH
            path_lower = path.lower()
            add_dir_lower = add_dir.lower()
            if (path_lower.find(add_dir_lower) == -1):
                path = add_dir + ';' + path
                _winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
                _winreg.FlushKey(env)

            _winreg.CloseKey(env)
            ret = True
        except Exception:
            if not path:
                path = add_dir
                _winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
                _winreg.FlushKey(env)
                ret = True
            else:
                _winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
                _winreg.FlushKey(env)
                ret = False

            if env:
                _winreg.CloseKey(env)

        if ret:
            print("  ->Add directory \"%s\" into PATH succeed!\n" % add_dir)
        else:
            print("  ->Add directory \"%s\" into PATH failed!\n" % add_dir)

    def set_console_root(self):
        print("->Check environment variable %s" % AX_CONSOLE_ROOT)
        axmol_console_root = os.path.join(
            self.current_absolute_path, 'tools', 'console', 'bin')
        old_dir = self._find_environment_variable(AX_CONSOLE_ROOT)
        if old_dir is None:
            # add environment variable
            if self._isWindows():
                self.set_windows_path(axmol_console_root)

            self._set_environment_variable(
                AX_CONSOLE_ROOT, axmol_console_root)
        else:
            if old_dir == axmol_console_root:
                # is same with before, nothing to do
                return

            # update the environment variable
            if self._isWindows():
                self.remove_dir_from_win_path(old_dir)
                self.set_windows_path(axmol_console_root)

            self._force_update_env(AX_CONSOLE_ROOT, axmol_console_root)

    def set_axmol_root(self):
        print("->Check environment variable %s" % AX_ROOT)
        axmol_root = self.current_absolute_path
        old_dir = self._find_environment_variable(AX_ROOT)
        if old_dir is None:
            # add environment variable
            self._set_environment_variable(AX_ROOT, axmol_root)
        else:
            if old_dir == axmol_root:
                # is same with before, nothing to do
                return
            self._force_update_env(AX_ROOT, axmol_root)

    def _force_update_unix_env(self, var_name, value):
        import re
        home = os.path.expanduser('~')
        str_re = SetEnvVar.RE_FORMAT % var_name
        patten = re.compile(str_re)
        replace_str = 'export %s=%s\n' % (var_name, value)

        file_list = SetEnvVar.MAC_CHECK_FILES
        if self._isLinux():
            file_list = SetEnvVar.LINUX_CHECK_FILES

        print("  ->Update variable %s in files %s" %
              (var_name, str(file_list)))
        variable_updated = False
        for file_name in file_list:
            path = os.path.join(home, file_name)
            if os.path.isfile(path):
                lines = []
                # read files
                need_over_write = False
                file_obj = open(path, 'r')
                for line in file_obj:
                    str_temp = line.lstrip(' \t')
                    match = patten.match(str_temp)
                    if match is not None:
                        variable_updated = True
                        need_over_write = True
                        lines.append(replace_str)
                    else:
                        lines.append(line)
                file_obj.close()

                # rewrite file
                if need_over_write:
                    file_obj = open(path, 'w')
                    file_obj.writelines(lines)
                    file_obj.close()
                    print("    ->File %s updated!" % path)

        # nothing updated, should add variable
        if not variable_updated:
            print("\n  ->No files updated, add variable %s instead!" %
                  var_name)
            ret = self._set_environment_variable(var_name, value)
        else:
            ret = True

        return ret

    def _force_update_env(self, var_name, value):
        ret = False
        if self._isWindows():
            print("  ->Force update environment variable %s" % var_name)
            ret = self._set_environment_variable_win32(var_name, value)
            if not ret:
                print("    ->Failed!")
            else:
                print("    ->Succeed : %s=%s" % (var_name, value))
        else:
            ret = self._force_update_unix_env(var_name, value)
        return ret

    def _get_androidsdk_path(self):
        return self._get_sdkpath_for_cmd("android")

    def _get_sdkpath_for_cmd(self, cmd, has_bin_folder=True):
        ret = None
        print("  ->Search for command " + cmd + " in system...")
        if not self._isWindows():
            if sys.version_info.major >= 3:
                import subprocess
            else:
                import commands as subprocess
            state, result = subprocess.getstatusoutput("which " + cmd)
            if state == 0:
                ret = os.path.realpath(result)
                ret = os.path.dirname(ret)
                # Use parent folder if has_bin_folder was set
                if has_bin_folder:
                    ret = os.path.dirname(ret)

        if ret is not None:
            print("    ->Path " + ret + " was found\n")
        else:
            print("    ->Command " + cmd + " not found\n")
        return ret

    def _find_value_from_sys(self, var_name):
        if var_name == ANDROID_HOME:
            return self._get_androidsdk_path()
        else:
            return None

    def set_variable(self, var_name, value):
        print("->Check environment variable %s" % var_name)
        find_value = self._find_environment_variable(var_name)
        var_found = (find_value is not None)
        action_none = 0
        action_add = 1
        action_update = 2

        need_action = action_none
        if var_found:
            if value and self._check_valid(var_name, value):
                # should update
                need_action = action_update
            else:
                # do nothing
                need_action = action_none
        else:
            if not value:
                # find the command path in system
                value = self._find_value_from_sys(var_name)

            if not value:
                value = self._get_input_value(var_name)

            if value and self._check_valid(var_name, value):
                # should add variable
                need_action = action_add
            else:
                # do nothing
                need_action = action_none

        if need_action == action_none:
            # do nothing
            return SetEnvVar.RESULT_DO_NOTHING
        elif need_action == action_add:
            # add variable
            if self._set_environment_variable(var_name, value):
                return SetEnvVar.RESULT_ADDED
            else:
                return SetEnvVar.RESULT_ADD_FAILED
        elif need_action == action_update:
            # update variable
            if self._force_update_env(var_name, value):
                # update succeed
                return SetEnvVar.RESULT_UPDATED
            else:
                # update failed
                return SetEnvVar.RESULT_UPDATE_FAILED
        else:
            return SetEnvVar.RESULT_DO_NOTHING

    def set_environment_variables(self, android_sdk_root, quiet):

        print('\nSetting up axmol...')

        self.file_used_for_setup = self._get_filepath_for_setup()

        self.set_axmol_root()
        self.set_console_root()
        
        if self._isWindows():
            print(
                '->Configuration for Android platform only, you can also skip and manually edit your environment variables\n')
        else:
            print('->Configuration for Android platform only, you can also skip and manually edit "%s"\n' %
                  self.file_used_for_setup)
        if(quiet) :
            sdk_ret = self.set_variable(ANDROID_HOME, android_sdk_root)

        # tip the backup file
        if (self.backup_file is not None) and (os.path.exists(self.backup_file)):
            print('\nA backup file \"%s\" is created for \"%s\".' %
                  (self.backup_file, self.file_used_for_setup))

        if self._isWindows():
            print(
                '\nPlease restart the terminal or restart computer to make added system variables take effect\n')
        else:
            print('\nPlease execute command: "source %s" to make added system variables take effect\n' %
                  self.file_used_for_setup)

class FileDownloader(object):
    def __init__(self):
       self.current_absolute_path = os.path.dirname(os.path.realpath(__file__))
        
    def download_file(self, url, filename):
        # remove file for retry
        try:
            os.remove(filename)
        except OSError:
            pass
        print("==> Ready to download '%s' from '%s'" % (filename, url))
        if(sys.version_info.major >= 3):
            import urllib.request as urllib2
        else:
            import urllib2
        try:
            u = urllib2.urlopen(url)
        except urllib2.HTTPError as e:
            if e.code == 404:
                print("==> Error: Could not find the file from url: '%s'" % (url))
            print("==> Http request failed, error code: " + str(e.code) + ", reason: " + e.read())
            sys.exit(1)

        f = open(filename, 'wb')
        content_len = 0
        if(sys.version_info.major >= 3):
            content_len = u.getheader("Content-Length")
        else:
            content_len = u.info().getheaders("Content-Length")
            if content_len and len(content_len) > 0:
                content_len = content_len[0]
        file_size = 0
        if content_len:
            file_size = int(content_len)
        else:
            print("==> WARNING: Couldn't grab the file size from remote")
            return
            
        print("==> Start to download, please wait ...")

        file_size_dl = 0
        block_sz = 8192
        block_size_per_second = 0
        old_time = time()

        status = ""
        while True:
            buffer = u.read(block_sz)
            if not buffer:
                print("%s%s" % (" " * len(status), "\r")),
                break

            file_size_dl += len(buffer)
            block_size_per_second += len(buffer)
            f.write(buffer)
            new_time = time()
            if (new_time - old_time) > 1:
                speed = block_size_per_second / (new_time - old_time) / 1000.0
                if file_size != 0:
                    percent = file_size_dl * 100. / file_size
                    status = r"Downloaded: %6dK / Total: %dK, Percent: %3.2f%%, Speed: %6.2f KB/S " % (file_size_dl / 1000, file_size / 1000, percent, speed)
                else:
                    status = r"Downloaded: %6dK, Speed: %6.2f KB/S " % (file_size_dl / 1000, speed)
                print(status),
                sys.stdout.flush()
                print("\r"),
                block_size_per_second = 0
                old_time = new_time

        print("==> Downloading finished!")
        f.close()

    def ensure_directory(self, target):
        if not os.path.exists(target):
            os.mkdir(target)

    def download_file_with_retry(self, url, filename, times, delay):
        if(sys.version_info.major >= 3):
            import urllib.request as urllib2
        else:
            import urllib2
        
        output_path = dirname(filename)
        downloader.ensure_directory(output_path)
        
        times_count = 0
        while(times_count < times):
            times_count += 1
            try:
                if(times_count > 1):
                    print("==> Download file retry " + str(times_count))
                self.download_file(url, filename)
                return
            except Exception as err:
                if(times_count >= times):
                    raise err
                sleep(delay)
                
if __name__ == '__main__':
    if not _check_python_version():
        exit()

    parser = OptionParser()
    parser.add_option('-a', '--androidsdkroot',
                      dest='android_sdk_root', help='directory of android sdk root')
    parser.add_option(
        '-q', '--quiet', dest='quiet',action="store_false", default = True, help='setup without setting SDK')
    opts, args = parser.parse_args()

    # set environment variables
    env = SetEnvVar()
    if env._isWindows():
        if(sys.version_info.major >= 3):
            import winreg as _winreg
        else:
            import _winreg

    env.set_environment_variables(opts.android_sdk_root, opts.quiet)

    if env._isWindows():
        import ctypes
        HWND_BROADCAST = 0xFFFF
        WM_SETTINGCHANGE = 0x1A
        SMTO_ABORTIFHUNG = 0x0002
        result = ctypes.c_long()
        SendMessageTimeoutW = ctypes.windll.user32.SendMessageTimeoutW
        SendMessageTimeoutW(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
                            u'Environment', SMTO_ABORTIFHUNG, 5000, ctypes.byref(result))

    current_absolute_path = os.path.dirname(os.path.realpath(__file__))
    external_tools_path = os.path.join(current_absolute_path, 'tools', 'external')
    if not os.path.exists(external_tools_path):
        os.mkdir(external_tools_path)
            
    downloader = FileDownloader()
    if env._isWindows():
        file_path = os.path.join(external_tools_path, 'nuget', 'nuget.exe')
        
        if not os.path.isfile(file_path):
            downloader.download_file_with_retry('https://dist.nuget.org/win-x86-commandline/latest/nuget.exe', file_path, 5, 3)
