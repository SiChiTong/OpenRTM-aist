## -*- Makefile -*-
##---------------------------------------------------------------------------
## Makefile for cmake: creating OpenRTM-aist's FindOpenRTM.cmake for cmake
##
## $Id$
##---------------------------------------------------------------------------
#
# CMake's OpenRTMFind.cmake installation diretories
#
# * for the Windows systems
#  <prefix>/                                               (W)
#  <prefix>/(cmake|CMake)/                                 (W)
#  <prefix>/<name>*/                                       (W)
#  <prefix>/<name>*/(cmake|CMake)/                         (W)
#
# * for the UNIX systems
#  <prefix>/(share|lib)/cmake/<name>*/                     (U)
#  <prefix>/(share|lib)/<name>*/                           (U)
#  <prefix>/(share|lib)/<name>*/(cmake|CMake)/             (U)
#
# In Mac OS X with MacPorts, <prefix> might be /opt/local. (not confirmed)
# And on systems supporting OS X Frameworks and Application Bundles
# the following directories are searched for frameworks or bundles
# containing a configuration file:
#
# * for Mac OS X with frameworks and Application Bundles
#  <prefix>/<name>.framework/Resources/                    (A)
#  <prefix>/<name>.framework/Resources/CMake/              (A)
#  <prefix>/<name>.framework/Versions/*/Resources/         (A)
#  <prefix>/<name>.framework/Versions/*/Resources/CMake/   (A)
#  <prefix>/<name>.app/Contents/Resources/                 (A)
#  <prefix>/<name>.app/Contents/Resources/CMake/           (A)
#
# for the OpenRTM-aist
#
# Windows:
#   <prefix>/<name>*/cmake: C:/Progam Files/OpenRTM-aist/1.1/cmake
# UNIX:
#   <prefix>/share/<name>*/cmake: /usr/share/openrtm-1.1/cmake
# Mac OS X
#   <prefix>/share/<name>*/cmake: /opt/local/share/openrtm-1.1/cmake
#
# Users' CMakeList.txt just only use the following find_package() command
#   find_package(OpenRTM-aist ....)
#                ^^^^^^^^^^^^ cmake handle <name> case-insensitively
#
#
#
# testing find_package ability
# 
# Windows)
# configfile: C:\Program Files\OpenRTM-aist\1.1\cmake\
# directives:
#   find_package(OpenRTM CONFIGS OpenRTMConfig.cmake): OK
#   find_package(openrtm CONFIGS OpenRTMConfig.cmake): OK
#   find_package(OpenRTM-aist CONFIGS OpenRTMConfig.cmake): OK
#   find_package(openrtm-aist CONFIGS OpenRTMConfig.cmake): OK
#
# UNIX)
# configfile: /usr/share/openrtm-1.1/cmake/OpenRTMConfig.cmake
# directives:
#   find_package(openrtm CONFIGS OpenRTMConfig.cmake): OK
#   find_package(OpenRTM CONFIGS OpenRTMConfig.cmake): OK
#   find_package(OpenRTM-aist CONFIGS OpenRTMConfig.cmake): NG
#   find_package(openrtm-aist CONFIGS OpenRTMConfig.cmake): NG
#   find_package(OpenRTM-aist CONFIGS OpenRTMConfig.cmake NAMES openrtm): OK
#   find_package(openrtm-aist CONFIGS OpenRTMConfig.cmake NAMES openrtm): OK
#

clean-local:
	rm -f OpenRTMConfig.cmake 
	rm -rf *~

dist-hook: clean-local

EXTRA_DIST =              \
	cmake.yaml
