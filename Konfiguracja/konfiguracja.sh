#!/bin/bash

echo 'Install video4linux'
sudo apt-get install -y v4l-utils

echo 'move file to rules.d'
sudo cp 99-Bionik.rules /etc/udev/rules.d

echo 'Unplug and pluge a camera'
echo 'Press enter...'
read -n 1 -s

echo 'reload names'
sudo udevadm control --reload

echo 'Adjust resolution of camera'
sudo v4l2-ctl -d /dev/videoBionik -v width=640,height=480

echo 'List parameters video99...'
v4l2-ctl -d /dev/videoBionik -V

echo 'List parameters video0...'
v4l2-ctl -d /dev/video0 -V

echo 'List parameters video1...'
v4l2-ctl -d /dev/video1 -V

