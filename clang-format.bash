#!/usr/bin/env bash
find ./include  ! -path '*detail/fmt/*' -type f \( -iname \*.h -o -iname \*.hpp \) | xargs clang-format -style=file -i
