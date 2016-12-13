#!/bin/sh
#
# @file pkg_install_debianSarge.sh
# @brief OpenRTM-aist dependent packages install script for Debian-sarge
# @author Noriaki Ando <n-ando@aist.go.jp> and Shinji Kurihara
#

#---------------------------------------
# �ѥå������ꥹ��
#---------------------------------------
omni="libomniorb4 libomniorb4-dev omniidl4 omniorb4-nameserver"
ace="libace libace-dev"
openrtm="openrtm-aist openrtm-aist-doc openrtm-aist-dev openrtm-aist-example python-yaml"

devel="gcc g++ make"
packages="$devel $omni $ace $openrtm"
u_packages="$omni $ace $openrtm "

#---------------------------------------
# ��ݥ��ȥꥵ����
#---------------------------------------
create_srclist () {
    cnames="sarge edgy feisty gutsy hardy"
    for c in $cnames; do
	if test -f "/etc/apt/sources.list"; then
	    res=`grep $c /etc/apt/sources.list`
	else
	    echo "This distribution may not be debian/ubuntu."
	    exit
	fi
	if test ! "x$res" = "x" ; then
	    code_name=$c
	fi
    done
    if test ! "x$code_name" = "x"; then
	echo "The code name is : " $code_name
    else
	echo "This OS is not supported."
	exit
    fi
    dist_name="debian"
    if test -f "/etc/lsb-release" ; then
	res=`grep Ubuntu /etc/lsb-release`
	if test ! "x$res" = "x" ; then
	    dist_name="ubuntu"
	fi
    fi
    openrtm_repo="deb http://www.openrtm.org/pub/Linux/$dist_name/ $code_name main"
}

#---------------------------------------
# �������ꥹ�ȹ����ؿ������
#---------------------------------------
update_source_list () {
    rtmsite=`grep openrtm /etc/apt/sources.list`
    if test "x$rtmsite" = "x" ; then
	echo "OpenRTM-aist �Υ�ݥ��ȥ꤬��Ͽ����Ƥ��ޤ���"
	echo "Source.list �� OpenRTM-aist �Υ�ݥ��ȥ�: "
	echo "  " $openrtm_repo
	read -p "���ɲä��ޤ�����������Ǥ����� (y/n) [y] " kick_shell

	if test "x$kick_shell" = "xn" ; then
	    echo "���Ǥ��ޤ���"
	    exit 0
	else
	    echo $openrtm_repo >> /etc/apt/sources.list
	fi
    fi
}

#----------------------------------------
# root ���ɤ���������å�
#----------------------------------------
check_root () {
    if test ! `id -u` = 0 ; then
	echo ""
	echo "This script should be run by root user."
	echo "Abort."
	echo ""
	exit 1
    fi
}

#----------------------------------------
# �ѥå��������󥹥ȡ���ؿ�
#----------------------------------------
install_packages () {
    for p in $*; do
	echo "Now installing: " $p
	apt-get install $p
	echo "done."
	echo ""
    done
}

#------------------------------------------------------------
# �ꥹ�Ȥ�ս�ˤ���
#------------------------------------------------------------
reverse () {
    for i in $*; do
	echo $i
    done | sed '1!G;h;$!d'
}

#----------------------------------------
# �ѥå������򥢥󥤥󥹥ȡ��뤹��
#----------------------------------------
uninstall_packages () {
    for p in $*; do
	echo "Now uninstalling: " $p
	aptitude remove $p
	echo "done."
	echo ""
    done
}

#---------------------------------------
# �ᥤ��
#---------------------------------------
check_root
if test "x$1" = "x-u" ; then
    uninstall_packages `reverse $u_packages`
else
    create_srclist
    update_source_list
    apt-get update
    install_packages $packages
fi
