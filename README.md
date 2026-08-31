# Gity

A Git-inspired Version Control System written in **C++17**, built from scratch to understand how modern version control systems work internally.

Instead of using Git as a black box, Gity recreates core version control concepts including content-addressable object storage, staging, commits, commit history traversal, repository status, and ignore rules.

---

## Features

- Repository initialization (`init`)
- File staging (`add`)
- Directory and recursive staging
- Stage all files using `add .`
- Commit creation (`commit`)
- Commit history (`log`)
- Repository status (`status`)
  - Staged files
  - Modified files
  - Deleted files
  - Untracked files
- `.gityignore` support
  - Exact file patterns
  - Directory patterns
  - Wildcard suffix patterns such as `*.log`
  - Comments and blank lines
- SHA-1 based object storage
- Persistent staging index
- Git-inspired content-addressable object database
- Automatic protection of `.gity/` and `.git/`
- Installable command-line executable
- CMake-based build system

---

## Requirements

- C++17 compatible compiler
- CMake 3.16 or later
- Crypto++

---

## Installation

### Clone the repository

```bash
git clone https://github.com/Kartikey-patel/Gity.git
cd Gity
```

### Build Gity

```bash
cmake -S . -B build -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build
```

### Install Gity

```bash
cmake --install build
```

Make sure `$HOME/.local/bin` is included in your `PATH`.

Verify the installation:

```bash
which gity
```

Then run:

```bash
gity
```

Gity can now be used from any directory.

---

## Repository Structure

When a repository is initialized, Gity creates a `.gity` directory:

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

### Directory Description

| File / Directory | Purpose |
|------------------|---------|
| `objects/` | Stores blob, tree and commit objects |
| `refs/main` | Stores the latest commit hash |
| `HEAD` | Points to the current branch |
| `index` | Stores the staging area |

---

# Commands

## Initialize a Repository

```bash
gity init
```

Creates the `.gity` directory and initializes the repository.

---

## Stage a File

```bash
gity add <file>
```

Reads the file, computes its SHA-1 hash, stores it as a blob object, and records it in the staging index.

Example:

```bash
gity add hello.txt
```

---

## Stage a Directory

```bash
gity add <directory>
```

Recursively stages files inside the specified directory.

Example:

```bash
gity add src/
```

---

## Stage All Files

```bash
gity add .
```

Recursively stages files in the repository while respecting `.gityignore`.

The `.gity/` and `.git/` directories are always ignored.

---

## Create a Commit

```bash
gity commit "Commit message"
```

Creates a tree object from the staged files, creates a commit object, updates the current branch, and clears the staging area.

Example:

```bash
gity commit "Initial commit"
```

---

## View Commit History

```bash
gity log
```

Traverses commits starting from `HEAD` by following parent commit hashes.

---

## Repository Status

```bash
gity status
```

Displays the current repository state:

- Staged files
- Modified files
- Deleted files
- Untracked files

---

# `.gityignore`

Gity provides a simplified ignore system through a `.gityignore` file.

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

Files matching `.gityignore` rules are excluded from:

- Staging
- Untracked-file detection

The `.gity` and `.git` directories are always ignored automatically.

## Supported Ignore Patterns

### Exact File

```text
secret.txt
```

### Directory

```text
build/
```

The directory and its contents are ignored.

### Wildcard Suffix

```text
*.log
```

Ignores files ending in `.log`.

### Comments

Lines beginning with `#` are treated as comments.

### Blank Lines

Blank lines are ignored.

> Gity currently implements a simplified ignore-pattern system and does not provide complete Git-compatible `.gitignore` semantics.

---

# Example Workflow

Initialize a repository:

```bash
gity init
```

Create or modify files:

```bash
echo "Hello Gity" > hello.txt
```

Stage the file:

```bash
gity add hello.txt
```

Create a commit:

```bash
gity commit "Initial commit"
```

View the history:

```bash
gity log
```

Check repository status:

```bash
gity status
```

---

## Example Using `.gityignore`

Create a `.gityignore` file:

```text
build/
*.log
secret.txt
```

Then:

```bash
gity add .
```

Ignored files will not be staged.

For example:

```text
project/
├── .gity/
├── .gityignore
├── README.md
├── src/
│   └── main.cpp
├── build/
│   └── generated.o
├── debug.log
└── secret.txt
```

Running:

```bash
gity add .
```

will stage:

```text
.gityignore
README.md
src/main.cpp
```

while ignoring:

```text
build/
debug.log
secret.txt
```

---

# Internal Architecture

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

---

# Core Components

## Repository

Responsible for repository initialization and repository metadata such as:

- `HEAD`
- `refs/main`
- `index`
- `objects/`

## ObjectStore

Implements Git-inspired content-addressable storage.

Objects are identified using SHA-1 hashes and stored inside the `.gity/objects` directory.

## Hasher

Generates SHA-1 hashes for file and object contents.

## Index

Maintains the persistent staging area.

Each index entry associates a repository-relative file path with the SHA-1 hash of its contents.

## AddCommand

Handles file and directory staging.

It:

1. Reads files
2. Calculates SHA-1 hashes
3. Stores blob objects
4. Updates the staging index
5. Applies `.gityignore` rules

## Commit

Represents a commit object containing:

- Tree hash
- Parent commit hash
- Timestamp
- Commit message

## Tree

Represents the directory/file structure of a committed snapshot.

## CommitCommand

Creates tree and commit objects from the staged index and updates the current branch reference.

## LogCommand

Traverses the commit history by following parent commit hashes.

## StatusCommand

Compares the staging index with the current working tree to detect:

- Staged files
- Modified files
- Deleted files
- Untracked files

It also respects `.gityignore`.

## FileUtils

Provides reusable filesystem functionality including:

- File reading
- Recursive filesystem traversal
- Ignore-aware file discovery

## IgnoreManager

Loads and evaluates `.gityignore` rules.

It is shared by staging and status operations so that ignored files are treated consistently throughout the system.

---

# Object Storage

Gity uses a Git-inspired **content-addressable storage** model.

Each object is identified by the SHA-1 hash of its contents.

Example:

```text
aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d
```

is stored as:

```text
objects/
└── aa/
    └── f4c61ddcc5e8a2dabede0f3b482cd9aea9434d
```

The first two characters of the SHA-1 hash are used as the directory name.

This prevents thousands of objects from accumulating inside a single directory.

---

# Object Types

Gity currently uses three major object types:

```text
Blob
 │
 └── Stores file contents

Tree
 │
 └── Represents the repository file structure

Commit
 │
 └── Represents a committed snapshot
```

The relationship can be visualized as:

```text
Commit
  │
  ▼
 Tree
  │
  ├── Blob → file1
  ├── Blob → file2
  └── Tree
       └── Blob → file3
```

---

# Commit Format

Each commit is serialized using a simple text-based format:

```text
tree <treeHash>
parent <parentHash>
time <timestamp>
message <commit message>
```

The initial commit does not contain a `parent` field.

Subsequent commits reference the previous commit through the `parent` field.

This creates a linked history:

```text
Commit C
   │
   ▼
Commit B
   │
   ▼
Commit A
```

---

# Staging Model

Gity uses a persistent staging index.

The basic workflow is:

```text
Working Tree
     │
     │ gity add
     ▼
   Index
     │
     │ gity commit
     ▼
  Commit
```

When a file is staged:

```text
file contents
      │
      ▼
    SHA-1
      │
      ▼
    Blob
      │
      ▼
    Index
```

This separates the current working state from the next committed snapshot.

---

# Branch References

Gity currently initializes a default branch called `main`.

`HEAD` points to the current branch:

```text
HEAD
 │
 ▼
refs/main
 │
 ▼
commit hash
```

The `refs/main` file stores the hash of the latest commit on the branch.

---

# Technologies Used

- **C++17**
- `std::filesystem`
- **Crypto++**
- **CMake**

---

# Current Development Status

Gity currently implements the core functionality of a small Git-inspired version control system:

- Repository initialization
- SHA-1 hashing
- Content-addressable object storage
- Blob objects
- Tree objects
- Commit objects
- Persistent staging index
- File staging
- Directory staging
- `add .`
- Commit history
- Repository status
- Deleted-file detection
- Ignore rules through `.gityignore`
- Shared filesystem traversal
- Installable command-line executable

The project is still under active development and is intentionally focused on understanding the internal mechanisms behind version control systems.

---

# Future Improvements

The following features are planned for future versions:

- [ ] Repository discovery from subdirectories
- [ ] Complete `HEAD → Index → Working Tree` status comparison
- [ ] Checkout
- [ ] Branching
- [ ] Merge
- [ ] Restore
- [ ] Tags
- [ ] Delta compression
- [ ] More complete ignore-pattern compatibility
- [ ] Automated unit and integration testing
- [ ] Performance improvements
- [ ] Remote repository support
- [ ] GityHub integration

---

# Roadmap

```text
Version 1
    │
    ├── Repository initialization
    ├── Object storage
    ├── Staging
    ├── Commits
    ├── Log
    └── Status
         │
         ▼
Version 1.1
    │
    ├── Improved status handling
    ├── .gityignore
    ├── Filesystem refactoring
    ├── .gity metadata directory
    └── Installable CLI
         │
         ▼
Version 2
    │
    ├── Repository discovery
    ├── Checkout
    ├── Branching
    ├── Merge
    ├── Restore
    ├── Tags
    └── Improved object management
         │
         ▼
GityHub
    │
    ├── Remote repositories
    ├── Authentication
    ├── Repository hosting
    ├── Push / Pull
    ├── Collaboration
    └── Web interface
```

---

# Why Gity?

Git is extremely powerful, but its internal implementation can be difficult to understand when using it only through its command-line interface.

Gity is an attempt to recreate the fundamental ideas behind a version control system from scratch.

The goal is not to replace Git.

The goal is to understand **how a version control system actually works**.

---

# Author

**Kartikey Patel**

GitHub: https://github.com/Kartikey-patel