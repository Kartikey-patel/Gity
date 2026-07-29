# Mini Version Control System (VCS)

A simplified Git-like Version Control System written in **C++17** to understand how modern version control systems work internally.

Instead of using Git as a black box, this project recreates some of its core concepts from scratch, including object storage, staging, commits, and commit history traversal.

---

## Features

- Repository initialization (`init`)
- File staging (`add`)
- Commit creation (`commit`)
- Commit history (`log`)
- SHA-1 based object storage
- Persistent staging index
- Git-inspired object database

---

## Project Structure

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

| File/Directory | Purpose |
|---------------|---------|
| objects/ | Stores blobs, trees and commits |
| refs/main | Stores the latest commit hash |
| HEAD | Points to the current branch |
| index | Stores staged files |

---

## Commands

### Initialize Repository

```bash
vcs init
```

Creates the `.vcs` directory and initializes the repository.

---

### Stage a File

```bash
vcs add <file>
```

Reads the file, generates its SHA-1 hash, stores it as a blob object and records it in the staging index.

---

### Create a Commit

```bash
vcs commit "Commit message"
```

Creates a tree from all staged files, creates a commit object and updates the HEAD reference.

---
/**
 * @class Repository
 * @brief Initializes the version control repository.
 *
 * Responsible for creating the directory structure required
 * for the VCS, including object storage, references, HEAD,
 * and the staging index.
 */
### View Commit History

```bash
vcs log
```

Traverses commits starting from HEAD by following parent hashes.

---

## Internal Architecture

```
Working Directory
        │
        ▼
AddCommand
        │
        ▼
Hasher
        │
        ▼
ObjectStore
        │
        ▼
Index
        │
        ▼
CommitCommand
        │
        ▼
Tree
        │
        ▼
Commit
```

---

## Object Storage

Objects are stored using their SHA-1 hash.

Example:

Hash

```
aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d
```

Stored as

```
objects/
└── aa/
    └── f4c61ddcc5e8a2dabede0f3b482cd9aea9434d
```

Splitting the hash prevents thousands of files from being stored inside a single directory.

---

## Commit Format

Each commit is serialized as

```
tree <treeHash>
parent <parentHash>
time <timestamp>
message <commit message>
```

The initial commit does not contain a parent entry.

---

## Technologies Used

- C++17
- std::filesystem
- Crypto++ (SHA-1)
- CMake

---

## Future Improvements

- [ ] status
- [ ] checkout
- [ ] branch
- [ ] merge
- [ ] restore
- [ ] tags
- [ ] delta compression

---

## Learning Objectives

This project was built to understand the internal working of Git, including:

- Content-addressable storage
- SHA-1 hashing
- Blob, Tree and Commit objects
- Staging Area
- Commit history traversal
- File serialization
- Filesystem operations in C++

---

## Author

**Kartikey Patel**

GitHub: https://github.com/Kartikey-patel