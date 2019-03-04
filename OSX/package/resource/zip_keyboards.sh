#!/bin/sh
cd `dirname $0`
mkdir -p "others/"
rm "others/kbLayouts.zip"
cd ./../../../LayoutScripts/
zip ./../OSX/package/others/kbLayouts "Zawgyi L.km2" "Zawgyi L - Unicode.km2" "mm3std.km2" "PaOh.km2" "Yunghkio SHAN.km2"