#!/bin/sh
cd `dirname $0`
mkdir -p "others/"
rm "others/kbLayouts.zip"
# assuming keymagic repo and keymagic-keyboards repo are in the same directory
# so you need to clone keymagic-keyboards repo to run successfully
cd ./../../../../keymagic-keyboards/
zip --junk-paths ./../keymagic/OSX/package/resource/others/kbLayouts "Zawgyi/Zawgyi.km2" "ZawCode/ZawCode.km2" "Pyidaungsu/Pyidaungsu MM.km2" "PaOh/PaOh.km2" "Yunghkio/Yunghkio SHAN.km2"