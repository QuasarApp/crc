# AGENTS.md

Working guidance for AI coding agents and developers in the `QuasarApp/crc` repository.

## Description

QuasarApp/crc is a small C/C++ library that computes CRC checksums
(crc8/crc16/crc32/crc64), including the polynomial tables and a C++ hash
wrapper (`crchash`). It is a leaf dependency: used as a submodule by
`QuasarApp/Heart` (and transitively by Games/Ecliptica) for data integrity
checks.

- **Language:** C (implementation) + C++23 (wrapper); pure computation,
  no external dependencies (no Qt, no network)
- **Build system:** CMake ≥ 3.19 (Ninja preferred); license: see `LICENSE`
- **Layout:** `crc/` — implementations + polynomial tables + `crchash`;
  `unit-test/` — unit tests; `Makefile.comm` — shared makefile rules

The public API is the `crc/*.h` headers; consumers either add
`add_subdirectory(<crc>)` and link `crc`, or include the headers directly.

## Developing process

### Build

No submodules. CMake ≥ 3.19 + any C/C++23 compiler, no Qt.

```sh
git clone https://github.com/QuasarApp/crc.git
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCRC_TESTS=ON
cmake --build build --parallel 16
```

The deliverable is the `crc` library (there is no product binary); with
`CRC_TESTS=ON` the test executable lands at `build/crc_test`.

CMake options:

- `CRC_TESTS` — build the `crc_test` unit-test executable (default `OFF`)

### Branching and pull requests (workflow for AI agents)

- One issue → one branch `task_<issue>`, branched **strictly from `main`**:
  `git fetch origin && git checkout -b task_<issue> origin/main`.
- Commit messages: short imperative subject + issue reference,
  e.g. `fix: <summary> (issue #<n>)`. Keep the build green; a red CI blocks the merge.
- Label the issue while working (`question` if input is needed); once the PR
  is open, set `waiting-for-review`.
- Post the initial checklist as a comment on the issue, then update that same
  comment in place — one line per sub-task, marked `- [x]` with a short
  evidence note: `gh issue comment <n> --edit-last --body-file /tmp/checklist.md`.
- An agent has no browser — push and open the PR with the `gh` CLI
  (token with the `repo` scope; in this organisation the agent runs as
  `QuasarAppCI`):

```sh
git push -u origin task_<issue>
gh pr create --base main --head task_<issue> \
  --title "<type>: <summary> (issue #<n>)" --body-file /tmp/pr_body.md
```

- The PR body carries what changed and the verification actually performed
  (commands + their real output); `(issue #<n>)` in the title links the PR to
  the issue. A PR exists only when `gh pr create` prints its URL — confirm
  with `gh pr view <n> --json number,url,mergeable` before reporting
  "PR created".

### Documentation rules

- The README build section is part of the contract — keep it in sync with
  `CMakeLists.txt`.
- New CRC variants ship **with** unit tests — never in a follow-up ticket.

## Testing process

Plain C/C++ unit tests in `unit-test/` (no Qt Test); with `CRC_TESTS=ON`
they are compiled into the `crc_test` executable, which links the `crc`
library:

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCRC_TESTS=ON
cmake --build build --parallel 16
./build/crc_test
```

Known issue on `main`: the inherited `unit-test/crc_test.cpp` still calls
the old API (`crc8_msb`, `crc16`, `crc32`, …) while the headers now declare
the QuasarApp API (`qa_calc_crc8_msb`, …) — `CRC_TESTS=ON` fails at the build
step until the tests are ported to the current headers (a separate task).

The smoke test (merge gate) is: configure + build the `crc` library green —
headless, seconds-fast, no external dependencies. A red build blocks the
merge, because every QuasarApp network/serialization path
(Heart → Games → Ecliptica) uses these checksums.