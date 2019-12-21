#!/usr/bin/python3

import os
import shutil
import hashlib

SRC_DIRECTORY = 'src'
BUILD_DIRECTORY = 'build'
OBJ_DIRECTORY = os.path.join(BUILD_DIRECTORY, 'obj')
BIN_DIRECTORY = os.path.join(BUILD_DIRECTORY, 'bin')

def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.digest()

def file_exists(src_file, dest_file):
    if not os.path.isfile(dest_file):
        return False

    return md5(src_file) == md5(dest_file)


def copy_tree(src_directory, destination_directory, dry=False):
    """
    Copies the given src directory to the destination directory.
    Files that already exists with the same version at the destination directory are ignored.
    """
    for root, directories, files in os.walk(src_directory):
        build_root = os.path.join(destination_directory, root[len(src_directory)+1:])
        if dry:
            print('makedir {}'.format(build_root))
        else:
            os.makedirs(build_root, exist_ok=True)
        for d in directories:
            current_src_dir = os.path.join(root, d)
            current_dest_dir = os.path.join(build_root, d)

            if dry:
                if not os.path.isdir(current_dest_dir):
                    print('makedir {}'.format(current_dest_dir))
            else:
                os.makedirs(current_dest_dir, exist_ok=True)

        for f in files:
            current_src_file = os.path.join(root, f)
            current_dest_file = os.path.join(build_root, f)

            if not file_exists(current_src_file, current_dest_file):
                if dry:
                    print('copyfile {} {}'.format(current_src_file, current_dest_file))
                else:
                    shutil.copy2(current_src_file, current_dest_file)


copy_tree(SRC_DIRECTORY, OBJ_DIRECTORY, dry=False)


def get_server_source_files(env):
    dirs = []
    for root, _, _ in os.walk(OBJ_DIRECTORY):
        dirs.append(root)

    source_files = []
    for d in dirs:
        source_files.extend(env.Glob(os.path.join(d, '*.cpp'), exclude=['main.cpp']))

    return source_files


env = Environment(parse_flags='-lboost_thread.a -std=c++17')
env['ENV']['TERM'] = os.environ['TERM']
env.Append(LIBS=['pthread', 'GLU', 'glfw3', 'X11', 'Xxf86vm', 'Xrandr', 'pthread', 'Xi', 'dl', 'Xinerama', 'Xcursor']),
# env.Append(CCFLAGS='-O3')

server_source_files = get_server_source_files(env)

env.Program(os.path.join(BIN_DIRECTORY, 'server'), server_source_files)
