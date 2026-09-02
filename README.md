# Gity

A Git-inspired Version Control System written in **C++17**, built from scratch to understand how modern version control systems work internally.

Gity focuses on useful local version-control functionality rather than reproducing every Git command.

## Features

- Repository initialization (`init`)
- File staging (`add`)
- Recursive directory staging
- Stage all files with `add .`
- Commit creation (`commit`)
- Commit history (`log`)
- Repository status (`status`)
  - Staged new files
  - Staged modifications
  - Staged deletions
  - Unstaged modifications
  - Unstaged deletions
  - Untracked files
- Three-state comparison of `HEAD`, Index, and Working Tree
- `.gityignore` support
- SHA-1 based object identification
- Persistent staging index
- Content-addressable object storage
- Blob, Tree, and Commit objects
- CMake build system
- Installable `gity` command

## Requirements

- C++17 compatible compiler
- CMake 3.16+
- Crypto++

## Installation

```bash
git clone https://github.com/Kartikey-patel/Gity.git
cd Gity

cmake -S . -B build -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build
cmake --install build
```

Ensure `$HOME/.local/bin` is in your `PATH`.

```bash
which gity
gity
```

## Repository Structure

After `gity init`:

```text
.gity/
├── objects/
│   ├── aa/
│   ├── b3/
│   └── ...
├── refs/
│   └── main
├── HEAD
└── index
```

| File / Directory | Purpose |
|---|---|
| `objects/` | Stores Blob, Tree, and Commit objects |
| `refs/main` | Stores the latest commit hash |
| `HEAD` | Points to the current branch |
| `index` | Persistent staging area |

## Commands

### Initialize

```bash
gity init
```

Creates and initializes a `.gity` directory.

### Stage a File

```bash
gity add <file>
```

The file is read, hashed, stored as a Blob object, and added to the staging index.

### Stage a Directory

```bash
gity add <directory>
```

Recursively stages files while respecting `.gityignore`.

### Stage Everything

```bash
gity add .
```

Stages repository files while respecting ignore rules. Previously tracked files that have been deleted are also staged for deletion.

### Commit

```bash
gity commit "Commit message"
```

Creates a Tree from the staging index, creates a Commit object, updates `refs/main`, and clears the index.

A commit contains:

- Tree hash
- Parent commit hash
- Timestamp
- Commit message

### Log

```bash
gity log
```

Starts from `HEAD` and follows parent commit hashes backwards.

### Status

```bash
gity status
```

Gity compares:

```text
HEAD
 │
 ▼
Index
 │
 ▼
Working Tree
```

Example:

```text
On branch main

Changes to be committed:
  new file: "new.txt"
  modified: "src/main.cpp"
  deleted: "old.txt"

Changes not staged for commit:
  modified: "README.md"
  deleted: "temp.txt"

Untracked files:
  test.txt
```

A clean repository reports:

```text
On branch main

nothing to commit, working tree clean
```

### Status State Model

| HEAD | Index | Working Tree | Meaning |
|---|---|---|---|
| absent | present | present | Staged new file |
| present | changed | changed | Staged modification |
| present | absent | absent | Staged deletion |
| present | present | changed | Unstaged modification |
| present | present | absent | Unstaged deletion |
| absent | absent | present | Untracked file |

This three-state model is the basis of `gity status`.

## `.gityignore`

Gity supports a simplified ignore system.

Example:

```text
# Generated files
build/

# Log files
*.log

# Secret files
.env
secret.txt
```

Ignored files are excluded from staging and untracked-file detection.

Supported patterns include:

- Exact file names
- Directory patterns
- Suffix wildcards such as `*.log`
- Comments
- Blank lines

`.gity/` and `.git/` are automatically ignored.

> Gity intentionally implements simplified ignore semantics rather than complete Git-compatible `.gitignore` behavior.

## Example Workflow

```bash
mkdir demo
cd demo

gity init

echo "Hello Gity" > hello.txt

gity status
gity add hello.txt
gity commit "Initial commit"

gity log
gity status
```

Modify the file:

```bash
echo "Updated content" > hello.txt
gity status
```

Stage and commit:

```bash
gity add hello.txt
gity commit "Update hello.txt"
```

## Testing

Gity includes an automated CLI integration test suite covering the currently implemented behavior, including:

- Repository initialization
- File staging
- Commits and history
- Clean repository status
- Staged and unstaged modifications
- Staged and unstaged deletions
- Staged new files
- Untracked files
- Staged-new-then-deleted files
- `.gityignore`
- Multiple commits
- Repository persistence
- Invalid input handling

Run the test suite with:

```bash
./tests/run_tests.sh


## Internal Architecture

```text
                         CLI
                          │
                          ▼
                 +-------------------+
                 |  Command Classes  |
                 +-------------------+
                          │
             ┌────────────┼────────────┐
             │            │            │
             ▼            ▼            ▼
        AddCommand   CommitCommand  StatusCommand
             │            │            │
             └────────────┼────────────┘
                          ▼
                   Core Components
                          │
       ┌──────────────────┼──────────────────┐
       │                  │                  │
       ▼                  ▼                  ▼
   Repository        ObjectStore           Index
       │                  │                  │
       ▼                  ▼                  ▼
      HEAD          Blob/Tree/Commit      Staging
                          │
                          ▼
                       Hasher

                     FileUtils
                         │
                         ▼
                  Filesystem traversal
                         │
                         ▼
                   IgnoreManager
                         │
                         ▼
                    .gityignore
```

## Core Components

### Repository

Initializes the repository and manages:

- `.gity/`
- `HEAD`
- `refs/main`
- `index`
- `objects/`

### ObjectStore

Provides content-addressable object storage:

- Object path generation
- Object existence checks
- Object storage
- Object loading

Objects are identified by SHA-1 hashes.

### Hasher

Generates SHA-1 hashes for file contents and serialized objects.

### Index

Maintains the persistent staging area:

```text
file path → SHA-1 hash
```

### AddCommand

Reads files, hashes them, stores Blob objects, updates the index, handles staged deletions, and applies ignore rules.

### Tree

Represents a snapshot of the staged files. Tree entries are serialized deterministically and stored as objects.

### Commit

Represents a committed snapshot containing a Tree hash, optional parent hash, timestamp, and message.

### CommitCommand

Creates the commit chain:

```text
Index
  │
  ▼
Tree
  │
  ▼
Commit
  │
  ▼
HEAD / refs/main
```

### LogCommand

Traverses commit history by following parent hashes.

### StatusCommand

Builds maps for `HEAD`, Index, and Working Tree and compares file presence and SHA-1 hashes to detect staged, unstaged, deleted, and untracked states.

### FileUtils

Provides file reading and recursive filesystem traversal.

### IgnoreManager

Loads and evaluates `.gityignore` rules and is shared by staging and status operations.

## Object Storage

Gity uses Git-inspired **content-addressable storage**.

For example:

```text
aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d
```

is stored as:

```text
objects/
└── aa/
    └── f4c61ddcc5e8a2dabede0f3b482cd9aea9434d
```

The first two hash characters form the directory name.

## Object Model

```text
Commit
  │
  ▼
 Tree
  │
  ├── Blob → file1
  ├── Blob → file2
  └── Blob → file3
```

- **Blob** — stores file contents
- **Tree** — represents a snapshot of staged files
- **Commit** — represents a committed snapshot

## Commit History

Each commit stores the hash of its parent:

```text
Commit C
   │
   ▼
Commit B
   │
   ▼
Commit A
```

`gity log` starts at `HEAD` and follows this chain backwards.

## Development Philosophy

Gity is not intended to become a copy of every Git feature.

Features are added when they provide meaningful value to a practical local version-control system or demonstrate an important version-control concept.

The project prioritizes:

- Understandable architecture
- Correctness
- Practical usefulness
- Persistent data structures
- Clear separation of responsibilities
- Features that can be clearly explained in technical interviews

## Current Development Status

The current core includes:

- Repository initialization
- SHA-1 hashing
- Content-addressable object storage
- Blob objects
- Tree objects
- Commit objects
- Persistent staging index
- File and directory staging
- `add .`
- Staged deletions
- Commit history
- Three-state repository status
- Automated CLI integration testing
- Untracked-file detection
- `.gityignore`
- Filesystem traversal
- Installable executable
- CMake build system

The goal is to validate Gity by using it to track real projects before expanding the system further.

## Future Improvements

Potential future work includes:

- [ ] Repository discovery from subdirectories
- [ ] More complete ignore-pattern compatibility
- [ ] Snapshot recovery / `restore`
- [ ] Checkout
- [ ] Branching
- [ ] Merge
- [ ] Tags
- [ ] Improved object management
- [ ] Performance improvements
- [ ] Remote repository support

Features will be added based on actual usefulness rather than simply matching Git's feature set.

## GityHub

Gity is intended to become the local version-control layer for a future project called **GityHub**.

```text
                 Gity
                  │
                  │ local repository
                  ▼
          ┌─────────────────┐
          │     GityHub     │
          │                 │
          │ Remote Hosting  │
          │ Collaboration   │
          │ Authentication  │
          │ Web Interface   │
          └─────────────────┘
```

GityHub will provide remote repository hosting and collaboration capabilities on top of the concepts implemented by Gity.

GityHub will be started after Gity has been validated by managing real projects.

## Why Gity?

The goal is not to replace Git.

The goal is to understand **how a version control system actually works**.

By implementing the system from scratch in C++17, the project explores:

- Content-addressable storage
- Hash-based object identification
- Persistent staging
- Snapshot creation
- Commit history
- Filesystem traversal
- Repository state comparison
- Serialization
- Ignore rules
- Separation between working tree, index, and committed state

## Author

**Kartikey Patel**

GitHub: https://github.com/Kartikey-patel