IDENTITY_INSTALLER = "Developer ID Installer: Big IMPACT Media Company Limited (4M4NDB57J9)"
ROOT_DIR = /tmp/KeyMagic.dst
GET_VERSION = defaults read $(ROOT_DIR)/KeyMagic.app/Contents/Info.plist CFBundleVersion
VERSION = $(shell $(GET_VERSION))
# assuming keymagic repo and keymagic-keyboards repo are in the same directory
# so you need to clone keymagic-keyboards repo to run successfully
KEYBOARDS_REPO = "../../../keymagic-keyboards/"
ZIP_PATH = "resource/others/kbLayouts.zip"
BUILT_APP_PATH = "../IMKBased/build/Release/KeyMagic.app"

all: clean build pack

pack:
	pkgbuild \
	--root $(ROOT_DIR) \
	--install-location 'Library/Input Methods' \
	--component-plist components.plist \
	--identifier org.keymagic.pkg.app \
	--version $(VERSION) \
	app.pkg

	pkgbuild \
	--root ./resource/others \
	--install-location 'Library/Input Methods' \
	--identifier org.keymagic.resources \
	--scripts ./resource/scripts \
	resources.pkg

	productbuild \
	--distribution distribution_.plist \
	--resources ./ \
	--package-path . \
	--sign $(IDENTITY_INSTALLER) \
	keymagic-$(VERSION).pkg

cp: build #components plist
	pkgbuild --analyze --root $(ROOT_DIR) components.plist

dp: #distribution plist
	productbuild --synthesize \
	--package app.pkg \
	--package resources.pkg \
	distribution.plist

clean:
	rm -rf $(ROOT_DIR)
	rm -f app.pkg resources.pkg
	
build:
	xcodebuild -configuration Release -project ../IMKBased/KeyMagic.xcodeproj
	mkdir -p $(ROOT_DIR)
	cp -r $(BUILT_APP_PATH) $(ROOT_DIR)/
	/usr/bin/xattr -rc $(ROOT_DIR)
	sh -c "sed -e 's/__VERSION__/`$(GET_VERSION)`/' distribution.plist > distribution_.plist"

notaryPkg:
	xcrun altool --notarize-app \
               --primary-bundle-id "org.keymagic.KeyMagic.installer" \
               --password "@keychain:thant@bim.com.mm" \
               --asc-provider "4M4NDB57J9" \
               --file keymagic-$(VERSION).pkg

notaryApp:
	/usr/bin/ditto -c -k --keepParent $(BUILT_APP_PATH) /tmp/KeyMagic.zip
	xcrun altool --notarize-app \
               --primary-bundle-id "org.keymagic.KeyMagic.zip" \
               --password "@keychain:thant@bim.com.mm" \
               --asc-provider "4M4NDB57J9" \
               --file "/tmp/KeyMagic.zip"

getNotaryStatus:
	xcrun altool --notarization-history 0 -p "@keychain:thant@bim.com.mm"

checkSigs:
	spctl -a -vv $(BUILT_APP_PATH)
	spctl -a -vv -t install keymagic-$(VERSION).pkg

prepare:
	mkdir -p resource/others
	rm $(ZIP_PATH) || true
	zip --junk-paths $(ZIP_PATH) \
	"$(KEYBOARDS_REPO)/Zawgyi/Zawgyi.km2" \
	"$(KEYBOARDS_REPO)/ZawCode/ZawCode.km2" \
	"$(KEYBOARDS_REPO)/Pyidaungsu/Pyidaungsu MM.km2" \
	"$(KEYBOARDS_REPO)/PaOh/PaOh.km2" \
	"$(KEYBOARDS_REPO)/Yunghkio/Yunghkio SHAN.km2"
