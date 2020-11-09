#!/usr/bin/python3

import argparse
import os

basedir = os.path.abspath(os.path.dirname(__file__))

init_cmake_info = '''
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

# if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
# endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)'''


class QmakeParser(object):
    """Parse a qmake file and store a map
    """

    cmakeparsed = {}

    def __init__(self, qmake_file_lines):
        self.qmake_file_lines = qmake_file_lines
        self.tokens = {
            "TEMPLATE": self.template,
            "SOURCES": self.sources,
            "FORMS": self.forms,
            "RESOURCES": self.resources,
            "TRANSLATIONS": self.translations,
            "TARGET": self.target,
            "INCLUDEPATH": self.includepaths
        }

    def parse(self):
        parse_dict = dict()
        snippet = []
        for line in self.qmake_file_lines:
            while ' ' in line:
                line = line.replace(' ', '')
            if line.strip().endswith('\\'):
                result = line.strip().replace('\\', '')
            else:
                result = line.strip().replace('\\', '/')

            if len(result) and '#' not in line:
                snippet.append(result)
            else:
                continue
            # print(snippet)
            if '\\' in line:
                for token in self.tokens.keys():
                    if snippet[0].lstrip().startswith(token):
                        parse_dict[token] = self.tokens[token](snippet)
            else:
                for token in self.tokens.keys():
                    if snippet[0].lstrip().startswith(token):
                        parse_dict[token] = self.tokens[token](snippet)
                snippet.clear()
        return parse_dict

    def template(self, codesnippet):
        return ' '.join(codesnippet).replace('=', ' ').split(' ')[1:]

    def sources(self, codesnippet):
        return ' '.join(codesnippet).replace('+=', ' ').split(' ')[1:]

    def includepaths(self, codesnippet):
        return ' '.join(codesnippet).replace('+=', ' ').split(' ')[1:]

    def forms(self, codesnippet):
        return ' '.join(codesnippet).replace('+=', ' ').split(' ')[1:]

    def resources(self, codesnippet):
        return ' '.join(codesnippet).replace('+=', ' ').split(' ')[1:]

    def translations(self, codesnippet):
        return ' '.join(codesnippet).replace('+=', ' ').split(' ')[1:]

    def target(self, codesnippet):
        return ' '.join(codesnippet).replace('=', ' ').split(' ')[1:]

    def write_include_path(self, cmake_file, include_paths):
        for include_path in include_paths:
            cmake_file.write("include_directories(")
            cmake_file.write("${CMAKE_CURRENT_SOURCE_DIR}/%s" % include_path)
            cmake_file.write(")")
            cmake_file.write("\n\n")
            cmake_file.flush()

    def write_executable(self, cmake_file, sources, project, parse_dict):
        cmake_file.write("add_executable(" + project)
        cmake_file.write('\n')
        cmake_file.flush()
        for source in sources:
            for source_file in parse_dict.get(source, []):
                if source_file == '':
                    continue
                cmake_file.write('  ' + source_file)
                cmake_file.write('\n')
                cmake_file.flush()
        cmake_file.write(")")
        cmake_file.write('\n\n')
        cmake_file.flush()

    def pasrse2cmake(self):
        parse_dict = qmake_parser.parse()
        sources = [
            "SOURCES",
            "FORMS",
            "RESOURCES",
            "TRANSLATIONS"
        ]
        include_paths = parse_dict.get("INCLUDEPATH", [])
        print(parse_dict)
        librarys = parse_dict.get("LIBS", [])
        project = parse_dict["TARGET"][0]
        template = parse_dict["TEMPLATE"][0]
        cmake_file = open('CMakeLists.txt', 'w')

        # version
        cmake_file.write("cmake_minimum_required(VERSION 3.5)")
        cmake_file.write('\n\n')
        cmake_file.write("project(%s LANGUAGES CXX)" % project)
        cmake_file.write('\n')
        cmake_file.flush()

        # qt project init
        cmake_file.write(init_cmake_info)
        cmake_file.write('\n\n')
        cmake_file.flush()

        # include_path
        self.write_include_path(cmake_file, include_paths)

        # add_executable
        self.write_executable(cmake_file, sources, project, parse_dict)

        # target_link_libraries
        cmake_file.write(
            "target_link_libraries(%s PRIVATE Qt5::Widgets)" % project)
        cmake_file.write('\n')
        cmake_file.flush()

        cmake_file.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        'qmake_path')

    args = parser.parse_args()

    try:
        with open(args.qmake_path, 'r') as qmake_file:
            qmake_parser = QmakeParser(qmake_file.readlines())
    except IOError as e:
        print(str(e))

    qmake_parser.pasrse2cmake()
