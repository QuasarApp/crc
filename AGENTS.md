# AGENTS.md

Working guidance for AI coding agents and developers in the `QuasarApp/crc` repository.

## Description

QuasarApp/crc is a small C/C++ library that computes CRC checksums
(crc8/crc16/crc32/crc64), including the polynomial tables and a C++ hash
wrapper (`crchash`). It is a leaf dependency: used as a submodule by
`QuasarApp/Heart` (and transitively by Games/Ecliptica) for data integrity
checks.

- **Language:** C (implementation) + C++23 (wrapper), header-friendly
- **Build system:** CMake ≥ 3.19
- **License:** see `LICENSE`

### Repository layout

| Path | Contents |
|---|---|
| `crc/` | `crc8`/`crc16`/`crc32`/`crc64` implementations + polynomial tables + `crchash` C++ wrapper |
| `unit-test/` | unit tests (built into the `crc_test` executable with `CRC_TESTS=ON`) |
| `Makefile.comm` | shared makefile rules (standalone builds) |

### Architecture

- Pure computation, no external dependencies (no Qt, no network).
- The public API is the `crc/*.h` headers; consumers either add
  `add_subdirectory(<crc>)` and link `crc`, or include the headers directly.

## Developing process

### Toolchain

| Tool | Version / notes |
|---|---|
| CMake | ≥ 3.19 |
| Compiler | C++23 required (`CMAKE_CXX_STANDARD 23`) |
| Ninja | preferred generator |

### Build

Step by step.

**1. One-time toolchain setup.** CMake ≥ 3.19 + any C/C++23 compiler. No Qt.

**2. Get the sources.** No submodules.

```sh
git clone https://github.com/QuasarApp/crc.git
```

**3. Configure** (enable the tests).

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCRC_TESTS=ON
```

**4. Build.**

```sh
cmake --build build --parallel 16
```

**5. Find the result.** The test executable lands at `build/crc_test`
(the project is top-level, so per-target layout equals the build root);
there is no product binary — the deliverable is the `crc` library.

CMake options:

- `CRC_TESTS` — build the `crc_test` unit-test executable (default `OFF`)

### Branching, labels and pull requests (workflow for AI agents)

- Work is issue-driven: one GitHub issue → one branch named
  `task_<issue-number>`, branched **strictly from `main`** — never from
  another task branch.
- Commit messages: short imperative subject + issue reference, e.g.
  `fix: <summary> (issue #<issue>)`, `docs: <summary> (issue #<issue>)`.
- Keep the build and lint green; a red CI blocks the merge.

**The exact commands.**

```sh
# 1. every task starts from a freshly fetched main
git fetch origin
git checkout -b task_<issue> origin/main

# 2. label the issue while working. Workflow labels in this repo:
#    in-progress, high/medium/low priority, question
gh issue edit <issue> --add-label in-progress

# 3. post the initial checklist as a comment; afterwards update that same
#    comment in place, never re-post it
gh issue comment <issue> --body-file /tmp/checklist.md
gh issue comment <issue> --edit-last --body-file /tmp/checklist.md

# 4. push and open the PR — see "Creating the pull request" below
git push -u origin task_<issue>
gh pr create --base main --head task_<issue> \
  --title "<type>: <short imperative summary> (issue #<issue>)" \
  --body-file /tmp/pr_body.md

# 5. the issue goes to waiting-for-review; watch CI from the CLI
gh issue edit <issue> --remove-label in-progress --add-label waiting-for-review
gh pr checks <number>
```

This repo does not define a `waiting-for-review` label yet — create it
before first use: `gh label create waiting-for-review --color 'fbca04'`.

**Status messages — what to post and when.** An agent must keep a
human-readable trail on the issue (and on the PR once it exists). Every
message is a GitHub comment with a `##` heading; the checklist comment is
updated in place (`--edit-last`), never duplicated.

| Stage | Label(s) | Comment template |
|---|---|---|
| Start | `in-progress` | `## Task issue_XXX: <title> — initial checklist (branch task_XXX from main)` — one `- [ ]` line per sub-task |
| Each step done | — | edit the checklist comment: mark the line `- [x]` + one short evidence note (command run, result) |
| Blocked / needs input | `blocked` / `question` | `## Blocked — task_XXX` — what is blocking and exactly what is needed to continue |
| PR opened | `waiting-for-review` | `## Progress — task_XXX` — all lines `- [x]` + `**PR: <url>**` + one-line verification summary |
| Review changes requested | `waiting-for-review` | `## Review-fix cycle — PR #NNN — update` — one `- [x]` line per review point, each with the evidence |
| CI red on the PR | `in-progress` | `## CI failure — PR #NNN` — failing check, cause, fix plan; push the fix, then update the comment |

**Creating the pull request.** An agent has no browser and cannot create a
pull request from the GitHub web UI — it must use the `gh` CLI (a token with
the `repo` scope; in this organisation the agent runs as `QuasarAppCI`):

```sh
gh pr create \
  --base main \
  --head task_<issue> \
  --title "<type>: <short imperative summary> (issue #<issue>)" \
  --body-file /tmp/pr_body.md
```

- The body carries what changed and the verification actually performed
  (commands + their real output); `(issue #<n>)` in the title links the PR
  to the issue. Use `--draft` while still iterating.
- A pull request exists only when `gh pr create` prints its URL. Confirm
  with `gh pr view <number> --json number,url,mergeable` before reporting
  "PR created" — never post that status on faith.
- If work is blocked, keep the PR open, set the issue label to `blocked` and
  post the status message above.

### Documentation rules

- The README build section is part of the contract — keep it in sync with
  `CMakeLists.txt`.
- New CRC variants ship **with** unit tests — never in a follow-up ticket.

## Testing process

### Test infrastructure

Plain C/C++ unit tests in `unit-test/` (no Qt Test). With `CRC_TESTS=ON`
they are compiled into the `crc_test` executable, which links the `crc`
library.

### Running the tests from CMake

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCRC_TESTS=ON
cmake --build build --parallel 16
./build/crc_test
```

Known issue on `main`: the unit tests do not compile. `unit-test/crc_test.cpp`
was inherited from the original `gityf/crc` and still calls the old API
(`crc8_msb`, `crc8_lsb`, `crc16`, `crc32`), while the library headers now
declare the renamed QuasarApp API (`qa_calc_crc8_msb`, `qa_calc_crc8_lsb`,
…). `CRC_TESTS=ON` therefore fails at the build step until the tests are
ported to the current headers (a separate task).

### Smoke tests — the most important testing step

The smoke test is: configure (with `CRC_TESTS=ON`) and build the `crc`
library green — headless, seconds-fast, no external dependencies. The unit
suite (`crc_test`) must be run as well once it compiles again (see the known
issue above). A red library build blocks the merge; because every QuasarApp
network/serialization path (Heart → Games → Ecliptica) uses these checksums,
a regression here is a regression everywhere downstream.