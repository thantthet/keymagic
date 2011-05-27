#!/bin/sh
cd "/tmp"
mkdir "$HOME/.keymagic"
mv "kbLayouts.zip" "$HOME/.keymagic/"
cd "$HOME/.keymagic/"
unzip "kbLayouts.zip"
rm "kbLayouts.zip"