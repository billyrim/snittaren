#!/bin/zsh
set -eu
cd "${0:A:h}/.."
source_bundle="$PWD/build/Snittaren_artefacts/Release/AU/SNITTAREN.component"
destination="$HOME/Library/Audio/Plug-Ins/Components"
[[ -d "$source_bundle" ]] || { print -u2 'Build the AU first.'; exit 1; }
mkdir -p "$destination"
if [[ -d "$destination/SNITTAREN.component" ]]; then
  mv "$destination/SNITTAREN.component" "$destination/SNITTAREN.component.backup-$(date +%Y%m%d-%H%M%S)"
fi
ditto "$source_bundle" "$destination/SNITTAREN.component"
codesign --force --deep --sign - "$destination/SNITTAREN.component"
touch "$destination/SNITTAREN.component"
print 'Installed. Quit and reopen Logic, then run: auval -v aufx Snit BlRm'
