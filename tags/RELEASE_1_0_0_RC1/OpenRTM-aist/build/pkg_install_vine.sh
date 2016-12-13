#!/bin/sh
#
# @file pkg_install_vine.sh
# @brief OpenRTM-aist dependent packages installation script for Vine Linux
# @author Noriaki Ando <n-ando@aist.go.jp>
#         Shinji Kurihara
#         Tetsuo Ando
#
# ���Υ����륹����ץȤϡ�ace�����omniORB�Υѥå������򥤥󥹥ȡ��뤷��
# Vine Linux�γ�ȯ�Ķ����ۤ��ޤ���
#
#---------------------------------------
# Vine�С����������
#---------------------------------------
vinever=`cat /etc/vine-release | sed 's/.*\([0-9].[0-9]\).*/\1/'`

#---------------------------------------
# ��ݥ��ȥꥵ����
#---------------------------------------
openrtm_repo="rpm     http://www.openrtm.org/pub/Linux/Vine/apt $vinever/\$(ARCH) main"

#---------------------------------------
# �ѥå������ꥹ��
#---------------------------------------
omni="omniORB omniORB-devel omniORB-servers omniORB-doc"
ace="ace ace-devel"
openrtm="OpenRTM-aist OpenRTM-aist-devel OpenRTM-aist-doc OpenRTM-aist-example PyYAML"
libuuid="e2fsprogs e2fsprogs-devel"
packages="$libuuid $omni $ace $openrtm"




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
# �ѥå������򥤥󥹥ȡ��뤹��
#----------------------------------------
install_packages () {
    for p in $*; do
	ins=`rpm -qa $p`
	if test "x$ins" = "x"; then
	    echo "Now installing: " $p
	    apt-get install $p
	    echo "done."
	    echo ""
	else
	    echo $p "is already installed."
	    echo ""
	fi
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
	rpm -e $p
	echo "done."
	echo ""
    done
}



#---------------------------------------
# �ᥤ��
#---------------------------------------
check_root
if test "x$1" = "x-u" ; then
    uninstall_packages `reverse $packages`
else
    update_source_list
    apt-get update
    install_packages $packages
fi
