#!/usr/bin/python
# ----------------------------------------------------------------------------
# MultiLanguage: Get the multi-language strings for console.
#
# Author: Bin Zhang
#
# License: MIT
# ----------------------------------------------------------------------------
'''
Get the multi-language strings for console.
'''

import axmol
import os
import sys
import json
import locale

def get_current_path():
    if getattr(sys, 'frozen', None):
        ret = os.path.realpath(os.path.dirname(sys.executable))
    else:
        ret = os.path.realpath(os.path.dirname(__file__))

    return ret

class MultiLanguage(object):
    CONFIG_FILE_NAME = 'strings.json'
    DEFAULT_LANGUAGE = 'en'
    instance = None

    @classmethod
    def get_available_langs(cls):
        info = cls.get_instance().cfg_info
        ret = []
        if info is not None:
            for key in info.keys():
                if axmol.isunicode(key):
                    ret.append(key.encode('utf-8'))

        return ret

    @classmethod
    def get_instance(cls):
        if cls.instance is None:
            cls.instance = MultiLanguage()

        return cls.instance

    @classmethod
    def get_string(cls, key, fmt_value=None):
        fmt = cls.get_instance().get_current_string(key)
        if fmt_value is None:
            ret = fmt
        else:
            if isinstance(fmt_value, tuple):
                dst_values = []
                for value in fmt_value:
                    if axmol.isunicode(value):
                        dst_values.append(value.encode(cls.get_instance().get_encoding()))
                    else:
                        dst_values.append(value)
                ret = fmt % tuple(dst_values)
            elif axmol.isunicode(fmt_value):
                ret = fmt % fmt_value.encode(cls.get_instance().get_encoding())
            else:
                ret = fmt % fmt_value

        return ret


    @classmethod
    def set_language(cls, lang):
        cls.get_instance().set_current_language(lang)

    def __init__(self):
        cfg_file_path = os.path.join(get_current_path(), MultiLanguage.CONFIG_FILE_NAME)

        # python3 always unicode, so no needs to translate locale at python scripts
        try:
            sys_lang, self.encoding = locale.getdefaultlocale()
        except:
            sys_lang = None
            self.encoding = None
            pass

        if (self.encoding is None) or sys.version_info.major >= 3:
            self.encoding = 'utf-8'
        
        if sys_lang is None:
            cur_lang_key = MultiLanguage.DEFAULT_LANGUAGE
        else:
            cur_lang_key = self.get_lang_key(sys_lang)

        # get the strings info
        if os.path.isfile(cfg_file_path):
            if(sys.version_info.major >= 3):
                f = open(cfg_file_path,'r', encoding='utf-8')
                self.cfg_info = json.load(f)
            else:
                f = open(cfg_file_path)
                self.cfg_info = json.load(f, encoding='utf-8')
            f.close()
            
            # python3 if self.cfg_info.has_key(cur_lang_key):
            if (cur_lang_key in self.cfg_info):
                self.cur_lang_strings = self.cfg_info[cur_lang_key]
            else:
                self.cur_lang_strings = None

            if MultiLanguage.DEFAULT_LANGUAGE in self.cfg_info: # if self.cfg_info.has_key(MultiLanguage.DEFAULT_LANGUAGE):
                self.default_lang_strings = self.cfg_info[MultiLanguage.DEFAULT_LANGUAGE]
            else:
                self.default_lang_strings = None
        else:
            self.cfg_info = None
            self.cur_lang_strings = None
            self.default_lang_strings = None

    def get_lang_key(self, sys_lang):
        sys_lang_info = sys_lang.split('_')
        lang = sys_lang_info[0]
        lang = lang.lower()
        region = None
        if len(sys_lang_info) > 1:
            region = sys_lang_info[1]
            region = region.lower()

        if lang == 'zh':
            if (region is None) or (region == 'cn'):
                ret = lang
            else:
                ret = 'zh_tr'
        else:
            ret = lang

        return ret

    def has_key(self, key, strings_info):
        ret = False
        if strings_info is not None and (key in strings_info): # if strings_info is not None and strings_info.has_key(key):
            ret = True

        return ret

    def set_current_language(self, lang):
        if (self.cfg_info is not None) and (self.cfg_info.has_key(lang)):
            self.cur_lang_strings = self.cfg_info[lang]
        else:
            axmol.Logging.warning(MultiLanguage.get_string('COCOS_WARNING_LANG_NOT_SUPPORT_FMT', lang))

    def get_encoding(self):
        return self.encoding

    def get_current_string(self, key):
        if self.has_key(key, self.cur_lang_strings):
            ret = self.cur_lang_strings[key]
        elif self.has_key(key, self.default_lang_strings):
            ret = self.default_lang_strings[key]
        else:
            ret= key

        ret = axmol.transcode(ret, self.encoding)

        return ret
