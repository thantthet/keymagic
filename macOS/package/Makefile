IDENTITY_APP = "Developer ID Application: H T KHIN ZAW (YQ4W4QMX9D)"
IDENTITY_INSTALLER = "Developer ID Installer: H T KHIN ZAW (YQ4W4QMX9D)"
ROOT_DIR = "/tmp/KeyMagic.dst"
VERSION = "1.5.4"
# assuming keymagic repo and keymagic-keyboards repo are in the same directory
# so you need to clone keymagic-keyboards repo to run successfully
KEYBOARDS_REPO = "../../../keymagic-keyboards/"
ZIP_PATH="resource/others/kbLayouts.zip"

all: clean build
	pkgbuild \
	--root $(ROOT_DIR) \
	--install-location "/Library/Input Methods" \
	--component-plist components.plist \
	--identifier org.keymagic.pkg.app \
	--version $(VERSION) \
	app.pkg

	pkgbuild --root ./resource/others \
	--identifier org.keymagic.resources \
	--scripts ./resource/scripts \
	resources.pkg

	productbuild \
	--distribution distribution.plist \
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

signapp:
	codesign --sign $(IDENTITY_APP) KeyMagic.app

clean:
	rm -rf $(ROOT_DIR)
	rm -f app.pkg resources.pkg
	
build:
	xcodebuild -configuration Release -project ../IMKBased/KeyMagic.xcodeproj
	mkdir -p $(ROOT_DIR)
	cp -r ../IMKBased/build/Release/KeyMagic.app $(ROOT_DIR)/
	/usr/bin/xattr -rc $(ROOT_DIR)

prepare:
	mkdir -p resource/others
	rm $(ZIP_PATH) || true
	zip --junk-paths $(ZIP_PATH) \
	"$(KEYBOARDS_REPO)/Zawgyi/Zawgyi.km2" \
	"$(KEYBOARDS_REPO)/ZawCode/ZawCode.km2" \
	"$(KEYBOARDS_REPO)/Pyidaungsu/Pyidaungsu MM.km2" \
	"$(KEYBOARDS_REPO)/PaOh/PaOh.km2" \
	"$(KEYBOARDS_REPO)/Yunghkio/Yunghkio SHAN.km2"
