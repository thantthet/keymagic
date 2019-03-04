IDENTITY_APP = "Developer ID Application: H T KHIN ZAW (YQ4W4QMX9D)"
IDENTITY_INSTALLER = "Developer ID Installer: H T KHIN ZAW (YQ4W4QMX9D)"
ROOT_DIR = "/tmp/KeyMagic.dst"

all: clean copy
	pkgbuild \
	--root $(ROOT_DIR) \
	--component-plist components.plist \
	--identifier org.keymagic.pkg.app \
	--version 1.5.2 \
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
	installer.pkg

cp: copy #components plist
	pkgbuild --analyze --root $(ROOT_DIR) app.plist

dp: #distribution plist
	productbuild --synthesize \
	--package app.pkg \
	--package resources.pkg \
	distribution.plist

signapp:
	codesign --sign $(IDENTITY_APP) KeyMagic.app

clean:
	rm -rf $(ROOT_DIR)
	
copy:
	mkdir -p $(ROOT_DIR)
	cp -r ./KeyMagic.app $(ROOT_DIR)/
	/usr/bin/xattr -rc $(ROOT_DIR)