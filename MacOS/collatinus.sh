#! /bin/bash
test -f Collatinus.dmg && rm Collatinus.dmg
mkdir xx
mv Collatinus.app xx/
cd xx 
ln -s /Applications .
cd ..

../yoursway-create-dmg/create-dmg   \
	--icon-size 142  \
	--window-pos 313 255  \
	--window-size 883 609  \
	--icon Applications 621 226   \
	--volname "Collatinus v9.4"   \
	--icon "Collatinus.app" 240 227  \
	--background ~/Collatinus/MacOS/backgroundImage.tiff  \
	Collatinus.dmg ~/Collatinus/xx/
