#!/bin/sh
prog=(/Users/thantthetkz/KeyMagic/build/Release/KeyMagic.app/Contents/MacOS/KeyMagic)

killall ${prog[0]##*/}

exec "${prog[@]}" startup

# defaults write com.apple.loginwindow LoginHook /Users/thantthetkz/KeyMagic/startup.sh