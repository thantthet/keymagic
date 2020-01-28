all: buildapp dmg 

clean:
	rm -rf ./assets/KeyMagic.app
buildapp:
	xcodebuild -configuration Release -project ../IMKBased/KeyMagic.xcodeproj
	cp -r ../IMKBased/build/Release/KeyMagic.app ./assets/
	cp ../IMKBased/KeyMagic.icns ./assets/
dmg:
	rm KeyMagic.dmg || true
	yarn run appdmg dmg.json KeyMagic.dmg