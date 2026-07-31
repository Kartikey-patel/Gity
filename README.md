# Mini Version Control System (VCS)

A simplified Git-like Version Control System written in **C++17** to understand how modern version control systems work internally.

Instead of using Git as a black box, this project recreates some of Git's core concepts from scratch, including object storage, staging, commits, commit history traversal, and repository status.

---

# Features

- Repository initialization (`init`)
- File staging (`add`)
- Commit creation (`commit`)
- Commit history (`log`)
- Repository status (`status`)
  - Staged files
  - Modified files
  - Untracked files
- SHA-1 based object storage
- Persistent staging index
- Git-inspired content-addressable object database

---

# Project Structure

```
.vcs/
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
| objects/ | Stores blob, tree and commit objects |
| refs/main | Stores the latest commit hash |
| HEAD | Points to the current branch |
| index | Stores the staging area |

---

# Commands

## Initialize Repository

```bash
vcs init
```

Creates the `.vcs` directory and initializes the repository.

---

## Stage a File

```bash
vcs add <file>
```

Reads the file, computes its SHA-1 hash, stores it as a blob object, and records it in the staging index.

---

## Create a Commit

```bash
vcs commit "Commit message"
```

Creates a tree object from all staged files, creates a commit object, updates the current branch, and clears the staging area.

---

## View Commit History

```bash
vcs log
```

Traverses commits starting from `HEAD` by following parent commit hashes.

---

## Repository Status

```bash
vcs status
```

Displays the current repository state:

- Staged files
- Modified files
- Untracked files

---

# Example Workflow

```bash
vcs init

vcs add hello.txt

vcs commit "Initial commit"

vcs log

vcs status
```

---

# Internal Architecture

```
                 CLI
                  │
                  ▼
        +-------------------+
        |  Command Classes  |
        +-------------------+
        │
        ├── AddCommand
        ├── CommitCommand
        ├── LogCommand
        └── StatusCommand
                  │
                  ▼
          Core Components
        ├── Repository
        ├── Index
        ├── ObjectStore
        ├── Tree
        ├── Commit
        ├── Hasher
        └── FileUtils
```

---

# Object Storage

Objects are stored using a Git-inspired content-addressable storage model.

Example SHA-1:

```
aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d
```

Stored as:

```
objects/
└── aa/
    └── f4c61ddcc5e8a2dabede0f3b482cd9aea9434d
```

Using the first two characters as the directory name prevents thousands of files from accumulating inside a single directory.

---

# Commit Format

Each commit is serialized as:

```
tree <treeHash>
parent <parentHash>
time <timestamp>
message <commit message>
```

The initial commit omits the `parent` field.

---

# Technologies Used

- C++17
- `std::filesystem`
- Crypto++
- CMake

---

# Future Improvements (Version 2)

- [ ] Repository discovery
- [ ] Multiple file staging
- [ ] `add .`
- [ ] `.vcsignore`
- [ ] Checkout
- [ ] Branching
- [ ] Merge
- [ ] Restore
- [ ] Tags
- [ ] Delta compression

---

# Learning Objectives

This project was built to understand the internal implementation of Git, including:

- Content-addressable storage
- SHA-1 hashing
- Blob, Tree and Commit objects
- Staging area
- Commit history traversal
- Repository status detection
- File serialization
- Filesystem operations in C++

---

# Author

**Kartikey Patel**

GitHub: https://github.com/Kartikey-patel