#!/bin/sh
cd `dirname $0`
mkdir -p "others/"
rm "others/kbLayouts.zip"
cd ./../../../LayoutScripts/
zip ./../OSX/package/resource/others/kbLayouts "Zawgyi L.km2" "Zawgyi L - Unicode.km2" "Pyidaungsu MM.km2" "PaOh.km2" "Yunghkio SHAN.km2"