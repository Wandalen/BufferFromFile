#! /bin/sh
set -o errexit

the_pwd=$PWD

cd "$(dirname "$BASH_SOURCE")"
cd repo

#cls && node-gyp rebuild && cls && node sample/experiment.js

{
  /usr/bin/osascript -e 'tell application "System Events" to tell process "Terminal" to keystroke "k" using command down' &&
  node-gyp rebuild &&
  /usr/bin/osascript -e 'tell application "System Events" to tell process "Terminal" to keystroke "k" using command down' &&
  node sample/experiment.js
} ||
{
  cd $the_pwd
}

cd $the_pwd
