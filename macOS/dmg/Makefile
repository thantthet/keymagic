ARCHIVE:="./build/KeyMagic.xcarchive"
EXPORT:="./export"
DEV_USERNAME:=${DEV_USERNAME} # set this in env
DEV_PASSWORD:=${DEV_PASSWORD} # set this in env
BUNDLE:=KeyMagic.app
DMG:=./KeyMagic.dmg
APP:=./assets/$(BUNDLE)

all: buildapp dmg 

clean:
	rm -rf $(APP)
	rm -rf ./assets/KeyMagic.icns

buildapp:
	xcodebuild -configuration Release -project ../IMKBased/KeyMagic.xcodeproj -scheme "KeyMagic (IMK-based)" -archivePath $(ARCHIVE) archive
	xcodebuild  -exportArchive -archivePath $(ARCHIVE) -exportPath $(EXPORT) -exportOptionsPlist "export-option.plist"
	cp -r $(EXPORT)/KeyMagic.app $(APP)
	cp ../IMKBased/KeyMagic.icns ./assets/
	rm -rf ./build
	rm -rf ./export

dmg:
	rm KeyMagic.dmg || true
	yarn run appdmg dmg.json $(DMG)
	codesign --sign "Developer ID" $(DMG)

notarize:
	./notarize.sh $(DMG) "org.keymagic.inputmethod.KeyMagic" $(DEV_USERNAME) $(DEV_PASSWORD)

validate:
	spctl -a -t exec -vv $(APP)
	spctl -a -t open --context context:primary-signature -v $(DMG)
