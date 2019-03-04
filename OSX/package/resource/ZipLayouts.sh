#!/bin/sh
cd `dirname $0`
mkdir -p "others/tmp/"
rm "others/tmp/kbLayouts.zip"
cd ./../../LayoutScripts/
zip ./../OSX/package/others/tmp/kbLayouts "Zawgyi L.km2" "Zawgyi L - Unicode.km2" "mm3std.km2" "PaOh.km2" "Yunghkio SHAN.km2"