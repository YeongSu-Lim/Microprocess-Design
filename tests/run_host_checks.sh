#!/bin/sh
set -eu
# Existing behavior and known-defect reproduction, NOT functional acceptance.
printf '%s\n' 'SUITE: existing behavior / known-defect reproduction (not acceptance)'
cd "$(dirname "$0")/.."
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT HUP INT TERM
printf '#include "common.h"\nint main(void) { return 0; }\n' > "$work/header.c"
gcc -std=c11 -Wall -Wextra -Werror -I. -fsyntax-only "$work/header.c"
gcc -std=c11 -Wall -Wextra -Werror -I. state.c tests/state_characterization.c -o "$work/state"
"$work/state"
