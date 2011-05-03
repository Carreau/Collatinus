#! /bin/bash
require_clean_work_tree () {
	git rev-parse --verify HEAD >/dev/null || exit 1
	git update-index -q --ignore-submodules --refresh
	err=0

	if ! git diff-files --quiet --ignore-submodules
	then
		echo >&2 "Cannot $1: You have unstaged changes."
		err=1
	fi

	if ! git diff-index --cached --quiet --ignore-submodules HEAD --
	then
		if [ $err = 0 ]
		then
		    echo >&2 "Cannot $1: Your index contains uncommitted changes."
		else
		    echo >&2 "Additionally, your index contains uncommitted changes."
		fi
		err=1
	fi

	if [ $err = 1 ]
	then
		test -n "$2" && echo >&2 "$2"
		exit 1
	fi
}


require_clean_work_tree


DMG_NAME="Collatinus-$(git describe --tags HEAD).dmg"
test -f $DMG_NAME && rm $DMG_NAME

create-dmg   \
	--icon-size 142  \
	--linktoappfolder \
	--window-pos 313 255  \
	--window-size 883 609  \
	--icon Applications 621 226   \
	--volname "Collatinus v9.4"   \
	--icon "Collatinus.app" 240 227  \
	--background ~/Collatinus/MacOS/backgroundImage.tiff  \
	$DMG_NAME ~/Collatinus/Collatinus.app
