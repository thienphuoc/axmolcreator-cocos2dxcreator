
import platform
import sys
import os
import shutil
import time
import subprocess

copy_files = []
update_files = 0
create_files = 0

def copy_if_newer(src, dst):
    global update_files
    global create_files
    if not os.path.exists(dst):
        shutil.copy(src, dst)
        copy_files.append("Create " + dst)
        create_files = create_files + 1 
    else:
        stat_src = os.stat(src)
        stat_dst = os.stat(dst)
        if stat_src.st_mtime > stat_dst.st_mtime:
            shutil.copy(src, dst)
            copy_files.append("Update  " + dst)
            update_files = update_files  + 1

def compile_lua(src, dst, luajit_command):
    command = "%s -b \"%s\" \"%s\"" % (luajit_command, src, dst)
    print("  Compiling %s " % src)
    ret = subprocess.call(command, shell=True, cwd = os.path.dirname(luajit_command))
    if ret != 0 :
        print("[error]: %s : failed to compile lua script %s" % (__file__, src))

def compile_if_newer(src, dst, cmd):
    global update_files
    global create_files
    if src.split(".")[-1] != "lua" :
        return
    dst = dst.strip() + "c"
    if not os.path.exists(dst):
        compile_lua(src, dst, cmd)
        copy_files.append("Create " + dst)
        create_files = create_files + 1 
    else:
        stat_src = os.stat(src)
        stat_dst = os.stat(dst)
        if stat_src.st_mtime > stat_dst.st_mtime:
            compile_lua(src, dst, cmd)
            copy_files.append("Update  " + dst)
            update_files = update_files  + 1
    
## copy folder if different
def sync_folder(src_dir, dst_dir, opts, compile):
    parent_dir = os.path.dirname(dst_dir)
    if not os.path.isdir(parent_dir):
        os.makedirs(parent_dir)
    if not opts.luajit and not compile and opts.issymlink:
        if (not os.path.isdir(dst_dir)):
            print("[message] create symlink %s ===> %s" % (src_dir, dst_dir))
            if opts.iswinnt:
                # Fix error symbolic link privilege not held < win11
                subprocess.run(['mklink', '/J', dst_dir, src_dir], shell=True)
            else:
                os.symlink(src_dir, dst_dir, True)
        return
    if os.path.isfile(src_dir):
        if opts.luajit and need_compile:
            compile_if_newer(src_dir, dst_dir, opts.luajit)
        else:
            copy_if_newer(src_dir, dst_dir)

    elif os.path.isdir(src_dir):
        names = os.listdir(src_dir)
        for name in names:
            src = os.path.join(src_dir, name)
            dst = os.path.join(dst_dir, name)
            sync_folder(src, dst, opts, need_compile)
    else:
        print("[warning] %s: src file %s is bad" % (__file__, src_dir))

if __name__ == "__main__":
    from argparse import ArgumentParser
    parser = ArgumentParser()
    parser.add_argument("-s", dest="src_dir")
    parser.add_argument("-d", dest="dst_dir")
    parser.add_argument("-j", dest="luajit", default=None)
    parser.add_argument("-m", dest="mode", default=None)
    parser.add_argument("-l", dest="symlink", default='false')
    (opts, unkonw) = parser.parse_known_args(sys.argv)
    
    need_compile = False
    # if args.luajit:
    #     print("  luajit mode '%s'" % (args.mode))

    # if args.mode == "Debug" and args.luajit:
    #     need_compile = False
    #     print(" -Skip luacompile in debug mode!")
        

    create_files = 0
    update_files = 0
    start_at = time.time()
    opts.iswinnt = platform.system().lower() == 'windows'
    opts.issymlink = opts.symlink.lower() == 'true'
    sync_folder(os.path.abspath(opts.src_dir), os.path.abspath(opts.dst_dir), opts, need_compile)
    end_at = time.time()
    
    if len(copy_files) > 0:
        # reduce logs 
        last_files = copy_files[-3:]
        for x in last_files:
            print("  %s" % x)
        if len(copy_files) > len(last_files) :
            print("  ...")
        print("  %d items updated, %d items created " % (update_files, create_files))
        print("  takes %s seconds"% (end_at - start_at))

