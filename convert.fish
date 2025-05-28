#!/usr/bin/env fish
for i in (find -type f -name "**.h" -o -name "**.cpp" -o -name "**.rc"); iconv -f SHIFT-JIS -t UTF-8 $i > "$i-new" && rm $i && mv $i-new $i; end
