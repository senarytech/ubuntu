#!/bin/bash

LD_LIBRARY_PATH=/opt/syna/lib /opt/syna/bin/test_disp hdmipwr off
sleep 1

ps -elf | grep ${USER} | grep -v grep | awk '{print $4}' | xargs kill -9

rm -fr /home/${USER}/.config/xfce4/xfconf/xfce-perchannel-xml/xfwm4.xml

LD_LIBRARY_PATH=/opt/syna/lib /opt/syna/bin/test_disp hdmipwr on

