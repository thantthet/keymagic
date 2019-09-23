#!/bin/bash
if [[ $# -ne 1 ]]; then
	echo "$0 <version>"
	exit 1
fi

filename=$(mktemp)

echo "$1" > ${filename}
aws s3 cp ${filename} s3://keymagic/releases/macos/latest/version.txt --acl public-read --profile thantthet	
rm ${filename}