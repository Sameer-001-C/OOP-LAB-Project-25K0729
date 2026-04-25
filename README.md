# 🗂️ Mini Git — Console-Based Version Control System

> **OOP Lab Semester Project | FAST-NUCES Karachi**  
> Developed in C++17 using Object-Oriented Programming principles

---

## 📋 Table of Contents

1. [Project Overview](#project-overview)
2. [Group Members](#group-members)
3. [Use Cases](#use-cases)
4. [OOP Concepts Demonstrated](#oop-concepts-demonstrated)
5. [Project File Structure](#project-file-structure)
6. [How to Compile](#how-to-compile)
7. [How to Run](#how-to-run)
8. [How to Use the System](#how-to-use-the-system)
9. [Test Cases](#test-cases)
10. [Assumptions & Limitations](#assumptions--limitations)

---

## 📌 Project Overview

**Mini Git** is a console-based Mini Version Control System built entirely in **C++**,
modelled after the core workflow of Git.

The system allows a user to:
- Create a named repository that persists across sessions
- Add, update, and delete files in a **working directory**
- Take **snapshots (commits)** of the working directory at any point in time
- Browse the full **commit history log**
- **Inspect** the exact contents of any past commit
- **Revert** the working directory back to any previous commit
- **Search** through commits by keyword (message or filename)

Every commit is assigned a unique hex ID (generated via djb2 hashing),
a sequential number, a precise timestamp, and a user-defined message —
making the history fully traceable.

All data is automatically **saved to disk** on exit and **reloaded on startup**,
so no work is ever lost between sessions.

---

## 👥 Group Members

| # | Name | Student ID |
|---|------|-----------|
| 1 | Muhammad [Last Name] | [Your ID] |
| 2 | [Member 2 Full Name] | [Member 2 ID] |


---

## 🎯 Use Cases

### Use Case 1 — Initialize a Repository
**Actor:** User  
**Description:** The user launches the program and enters a repository name.  
**Flow:**
- If a data file exists for that name → the system loads prior commits automatically.
- If no data file exists → a fresh empty repository is created.  

**Expected Outcome:** A named repository is ready for use, with prior history restored (if any).

---

### Use Case 2 — Add a New File
**Actor:** User  
**Description:** The user adds a new file (name + content) to the working directory.  
**Flow:**
- User selects **Add / Update File** from the menu.
- User enters a filename and its content.
- System stores the file in the working directory.  

**Expected Outcome:** The file appears in `Status` view and is included in the next commit.

---

### Use Case 3 — Update an Existing File
**Actor:** User  
**Description:** The user modifies the content of a file already in the working directory.  
**Flow:**
- User selects **Add / Update File** and provides the same filename.
- System detects the filename already exists and overwrites its content.  

**Expected Outcome:** The file's content is updated in-place without creating a duplicate.

---

### Use Case 4 — Delete a File
**Actor:** User  
**Description:** The user removes a tracked file from the working directory.  
**Flow:**
- User selects **Delete File**.
- System lists current working directory files.
- User types the filename to remove.  

**Expected Outcome:** The file is removed and excluded from future commits.

---

### Use Case 5 — Commit a Snapshot
**Actor:** User  
**Description:** The user saves the current state of all working directory files as an immutable commit.  
**Flow:**
- User selects **Commit**.
- User enters a descriptive commit message.
- System creates a `Commit` object on the heap with a unique ID and timestamp.  

**Expected Outcome:** A new commit appears in the log with correct number, ID, timestamp, message, and file list.

---

### Use Case 6 — View Commit History (Log)
**Actor:** User  
**Description:** The user wants to see all commits ever made.  
**Flow:**
- User selects **View Log**.
- System prints every commit's number, short hash, timestamp, message, and files.  

**Expected Outcome:** A complete chronological list of all commits is displayed.

---

### Use Case 7 — Inspect a Commit in Detail
**Actor:** User  
**Description:** The user wants to see the exact file contents stored inside a specific commit.  
**Flow:**
- User selects **Commit Detail** and the log is shown.
- User enters a commit number.
- System prints the full content of every file captured in that commit.  

**Expected Outcome:** Each file's name and complete content from that snapshot is displayed.

---

### Use Case 8 — Revert Working Directory
**Actor:** User  
**Description:** The user wants to undo recent changes and restore a previous state.  
**Flow:**
- User selects **Revert** and the log is shown.
- User enters a commit number.
- System replaces the current working directory with the file snapshot from that commit.  

**Expected Outcome:** Working directory matches the selected commit exactly; commit history is unaffected.

---

### Use Case 9 — Search Commits
**Actor:** User  
**Description:** The user wants to find commits related to a specific keyword.  
**Flow:**
- User selects **Search Commits** and enters a keyword.
- System performs a case-insensitive search across all commit messages AND all filenames inside each commit.
- Matching commits are displayed with the reason for the match.  

**Expected Outcome:** Only commits matching the keyword are shown.

---

## 🧠 OOP Concepts Demonstrated

### 1. Classes & Objects
Every real-world entity is modelled as a class:

| Class | Represents |
|-------|-----------|
| `FileEntry` | A single tracked file (name + content) |
| `CommitMetadata` | Abstract base holding commit identity data |
| `Commit` | A snapshot of the working directory at a point in time |
| `VCOperation` | Abstract base for all user-executable operations |
| `StatusOperation` | "Show working directory" command |
| `AddFileOperation` | "Add/update file" command |
| `DeleteFileOperation` | "Delete file" command |
| `CommitOperation` | "Commit snapshot" command |
| `LogOperation` | "View history" command |
| `ShowCommitDetailOperation` | "Inspect commit" command |
| `RevertOperation` | "Revert to commit" command |
| `SearchOperation` | "Search commits" command |
| `Repository` | Core engine managing state and persistence |
| `CLI` | Menu-driven interface coordinating user interaction |

---

### 2. Encapsulation
All class data members are declared `private`. No external code can directly
read or modify internal state — all access goes through clearly defined
`public` getter and setter methods.

**Example — `FileEntry`:**
```cpp
class FileEntry {
private:
    std::string filename;   // private — not directly accessible
    std::string content;

public:
    std::string getFilename() const;               // controlled read
    void        setContent(const std::string& c);  // controlled write
};
```

---

### 3. Inheritance
The project uses **two separate inheritance hierarchies**:

**Hierarchy A — Commit data:**
```
CommitMetadata   (abstract base)
    └── Commit   (derived — adds file snapshot vector)
```

**Hierarchy B — Operations:**
```
VCOperation              (abstract base)
    ├── StatusOperation
    ├── AddFileOperation
    ├── DeleteFileOperation
    ├── CommitOperation
    ├── LogOperation
    ├── ShowCommitDetailOperation
    ├── RevertOperation
    └── SearchOperation
```

---

### 4. Polymorphism
`VCOperation::execute()` is declared **pure virtual**. The `CLI` stores all
eight operation objects in a single `vector<VCOperation*>` and calls
`execute()` uniformly through the base pointer — the correct derived
class method is resolved at runtime.

```cpp
// In CLI.cpp — one line handles ALL 8 operations polymorphically:
operations[choice - 1]->execute(*repo);
```

`CommitMetadata::displayDetailed()` is also pure virtual, overridden by
`Commit::displayDetailed()`, enabling virtual dispatch when printing
commit details.

---

### 5. Abstraction
Two abstract classes are defined — they cannot be instantiated directly
because they contain at least one pure virtual function:

- **`CommitMetadata`** — declares `displayDetailed()` as pure virtual.
- **`VCOperation`** — declares `execute()`, `getOperationName()`, and
  `getDescription()` as pure virtual.

These abstract classes define *what* must be done without specifying *how*,
forcing derived classes to provide concrete implementations.

---

### 6. Dynamic Memory (`new` / `delete`)
`Repository` allocates every `Commit` object on the **heap**:
```cpp
commits.push_back(new Commit(message, workingDirectory));
```
All heap memory is released in the destructor:
```cpp
Repository::~Repository() {
    saveToFile();
    for (Commit* c : commits) delete c;
    commits.clear();
}
```
The `CLI` class similarly allocates each `VCOperation` on the heap and
deletes them on destruction. Both classes are marked **non-copyable** to
prevent double-free bugs.

---

### 7. Exception Handling
`try/catch` blocks guard every point where user input is parsed or file
operations are performed:

```cpp
try {
    int num = std::stoi(input);
    repo.showCommitDetail(num);
} catch (const std::invalid_argument&) {
    std::cout << "  [ERROR] Invalid input. Please enter a number." << std::endl;
} catch (const std::out_of_range&) {
    std::cout << "  [ERROR] Number out of range." << std::endl;
}
```

Repository construction also catches file-not-found gracefully, treating
it as a first-run scenario rather than a crash.

---

### 8. File Handling
`Repository::saveToFile()` writes the complete repository state (working
directory + all commits + file snapshots) to `<RepoName>_repo.dat` on every
clean exit. `Repository::loadFromFile()` reads and reconstructs all objects
on startup.

Content is encoded to handle embedded newlines (`<<NL>>` tokens), ensuring
the single-record-per-line file format is never corrupted by user content.

---

## 📁 Project File Structure

```
minigit/
│
├── main.cpp                  Entry point — creates CLI, calls initialize() + run()
│
├── FileEntry.h               Class declaration: filename + content
├── FileEntry.cpp             Implementation of FileEntry
│
├── CommitMetadata.h          Abstract base class: commit id, message, timestamp, number
├── CommitMetadata.cpp        Implementation: djb2 hash ID generation, display, counter
│
├── Commit.h                  Derived class: inherits CommitMetadata, adds file vector
├── Commit.cpp                Implementation: constructors + displayDetailed() override
│
├── VCOperation.h             Abstract base VCOperation + 8 derived operation classes
├── VCOperation.cpp           execute() implementations for all 8 operations
│
├── Repository.h              Core engine class declaration
├── Repository.cpp            Full implementation: CRUD, I/O, search, revert
│
├── CLI.h                     CLI class declaration
├── CLI.cpp                   Menu loop, polymorphic dispatch, input validation
│
├── Makefile                  Build automation (make / make clean)
└── README.md                 This file
```

**Total:** ~1,300 lines across 15 source files.

---

## 🔧 How to Compile

### Option A — Using `make` (Recommended)

```bash
# Navigate into the project directory
cd minigit

# Build the project
make
```

A successful build prints:
```
  Build successful! Run with: ./minigit
```

To clean all compiled files and the executable:
```bash
make clean
```

---

### Option B — Manual `g++` Command

```bash
g++ -std=c++17 -Wall -Wextra -g \
    main.cpp FileEntry.cpp CommitMetadata.cpp Commit.cpp \
    VCOperation.cpp Repository.cpp CLI.cpp \
    -o minigit
```

> **Compiler requirement:** g++ ≥ 7  **or**  clang++ ≥ 5 with C++17 support.  
> On Ubuntu/Debian: `sudo apt install g++`  
> On Windows: install [MinGW-w64](https://www.mingw-w64.org/) and use the same command.

---

## ▶️ How to Run

```bash
./minigit
```

On Windows (MinGW):
```bash
minigit.exe
```

---

## 📖 How to Use the System

### Step 1 — Enter Repository Name

```
  Enter repository name: MyProject
  [INFO] New repository 'MyProject' initialised.
```

- If `MyProject_repo.dat` already exists, all prior commits are loaded automatically.
- Repository names must **not** contain spaces. Use underscores or CamelCase.

---

### Step 2 — Navigate the Menu

```
  +---[ MENU ]----------------------------+
  |  1. Status
  |  2. Add / Update File
  |  3. Delete File
  |  4. Commit
  |  5. View Log
  |  6. Commit Detail
  |  7. Revert
  |  8. Search Commits
  |  9. Exit
  +---------------------------------------+

  Enter choice:
```

Type the number corresponding to the action you want and press **Enter**.

---

### Menu Options — Detailed Usage

#### Option 1 — Status
Displays the repository name, total commits, and all files currently in the working directory.

```
--- Working Directory Status ---
  Repository    : MyProject
  Commits so far: 2
  Working directory (2 file(s)):
    >> main.cpp
    >> README.md
```

#### Option 2 — Add / Update File
Add a new file or overwrite an existing one. Filename must not contain spaces.

```
--- Add / Update File ---
  Filename (e.g. main.cpp): main.cpp
  Content (single line): #include <iostream> int main() { return 0; }
  [OK] 'main.cpp' added/updated in working directory.
```

#### Option 3 — Delete File
Remove a file from the working directory.

```
--- Delete File ---
    >> temp.cpp
  Enter filename to delete: temp.cpp
  [OK] 'temp.cpp' deleted.
```

#### Option 4 — Commit
Save a snapshot of the entire working directory.

```
--- Commit ---
  Commit message: Add initial source files
  [OK] Commit #1 created successfully.
```

#### Option 5 — View Log
Display a summary of every commit.

```
--- Commit Log ---
  Total commits: 2
  ----------------------------------------
  Commit #1  [775dfae8...]  2025-04-22 18:26:58
  Message : Add initial source files
  Files (2): main.cpp  README.md
  ----------------------------------------
```

#### Option 6 — Commit Detail
Inspect the full content of every file inside a specific commit.

```
  ===========================================
  Commit #1  [775dfae8...]  2025-04-22 18:26:58
  Message : Add initial source files
  Files in this snapshot (2):
  -------------------------------------------
    File    : main.cpp
    Content : #include <iostream> int main() { return 0; }
  -------------------------------------------
```

#### Option 7 — Revert
Restore the working directory to the exact state of a past commit.

```
  Enter commit number to revert to: 1
  [OK] Working directory reverted to commit #1.
```

> ⚠️ Uncommitted changes in the working directory will be lost after a revert.

#### Option 8 — Search Commits
Find commits matching a keyword (searches both messages and filenames, case-insensitive).

```
  Enter search keyword: main
  Search results for "main":
  ----------------------------------------
  Commit #1  ...  (matched in filename)
  ----------------------------------------
```

#### Option 9 — Exit
Saves all data to disk and exits cleanly.

```
  Data saved. Goodbye!
```

---

## 🧪 Test Cases

Each test case specifies the exact inputs, expected output, and which
requirement is being validated.

---

### Test Case 1 — Initialize a New Repository

**Purpose:** Verify that a fresh repository is created when no prior data file exists.

**Inputs:**
```
Repository name : TestRepo
Menu choice     : 9
```

**Expected Output:**
```
  [INFO] New repository 'TestRepo' initialised.
  Data saved. Goodbye!
```

**Validates:** Constructor, file handling (first run), graceful exit.

---

### Test Case 2 — Add a File and Check Status

**Purpose:** Verify that a file is added to the working directory and shows in Status.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  Filename: main.cpp  →  Content: int main() { return 0; }
Choice : 1
Choice : 9
```

**Expected Output:**
```
  [OK] 'main.cpp' added/updated in working directory.

  Repository    : TestRepo
  Commits so far: 0
  Working directory (1 file(s)):
    >> main.cpp
```

**Validates:** `AddFileOperation`, `Repository::addFile()`, `StatusOperation`, encapsulation.

---

### Test Case 3 — Update an Existing File (No Duplicate)

**Purpose:** Verify that re-adding the same filename updates it rather than duplicating it.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  Filename: notes.txt  →  Content: Version 1
Choice : 2  →  Filename: notes.txt  →  Content: Version 2 updated
Choice : 1
Choice : 9
```

**Expected Output:**
```
  [OK] 'notes.txt' added/updated in working directory.
  [OK] 'notes.txt' added/updated in working directory.

  Working directory (1 file(s)):
    >> notes.txt
```

**Validates:** Duplicate filename detection in `Repository::addFile()`.

---

### Test Case 4 — Delete a File

**Purpose:** Verify that a file is removed and no longer appears in the working directory.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  Filename: temp.cpp  →  Content: temporary stuff
Choice : 3  →  Filename to delete: temp.cpp
Choice : 1
Choice : 9
```

**Expected Output:**
```
  [OK] 'temp.cpp' added/updated in working directory.
  [OK] 'temp.cpp' deleted.

  Working directory (0 file(s)):
    (empty)
```

**Validates:** `DeleteFileOperation`, `Repository::deleteFile()`.

---

### Test Case 5 — Create a Commit and View Log

**Purpose:** Verify that a commit is created with the correct metadata and appears in the log.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  Filename: app.cpp  →  Content: #include <iostream>
Choice : 4  →  Message: Initial commit
Choice : 5
Choice : 9
```

**Expected Output:**
```
  [OK] Commit #1 created successfully.

  Total commits: 1
  ----------------------------------------
  Commit #1  [xxxxxxxx...]  YYYY-MM-DD HH:MM:SS
  Message : Initial commit
  Files (1): app.cpp
  ----------------------------------------
```

**Validates:** `CommitOperation`, dynamic memory (`new Commit`), `LogOperation`.

---

### Test Case 6 — Inspect Commit Detail (Polymorphism)

**Purpose:** Verify that `displayDetailed()` (pure virtual override in `Commit`) shows full file content.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  Filename: hello.cpp  →  Content: cout << "Hello World";
Choice : 4  →  Message: Hello world commit
Choice : 6  →  Commit number: 1
Choice : 9
```

**Expected Output:**
```
  ===========================================
  Commit #1  [xxxxxxxx...]  YYYY-MM-DD HH:MM:SS
  Message : Hello world commit
  Files in this snapshot (1):
  -------------------------------------------
    File    : hello.cpp
    Content : cout << "Hello World";
  -------------------------------------------
  ===========================================
```

**Validates:** Polymorphism — `Commit::displayDetailed()` called via `CommitMetadata*`, abstraction.

---

### Test Case 7 — Revert to a Previous Commit

**Purpose:** Verify that reverting replaces the working directory with the selected commit's snapshot.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  v1.txt  →  This is version 1
Choice : 4  →  Message: Version 1
Choice : 2  →  v2.txt  →  This is version 2
Choice : 4  →  Message: Version 2
Choice : 7  →  Revert to commit: 1
Choice : 1
Choice : 9
```

**Expected Output (after revert + status):**
```
  [OK] Working directory reverted to commit #1.

  Working directory (1 file(s)):
    >> v1.txt
```

**Validates:** `RevertOperation`, `Repository::revert()`, snapshot isolation (v2.txt is gone).

---

### Test Case 8 — Search by Commit Message

**Purpose:** Verify that search matches the keyword against commit message text.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  a.txt  →  hello
Choice : 4  →  Message: Fix login bug
Choice : 2  →  b.txt  →  world
Choice : 4  →  Message: Add signup feature
Choice : 8  →  Keyword: login
Choice : 9
```

**Expected Output:**
```
  Search results for "login":
  ----------------------------------------
  Commit #1  [xxxxxxxx...]  ...
  Message : Fix login bug
  (matched in commit message)
  ----------------------------------------
```

**Validates:** `SearchOperation`, `Repository::searchCommits()`, case-insensitive matching.

---

### Test Case 9 — Search by Filename

**Purpose:** Verify that search matches the keyword against filenames inside commits.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  Filename: database.cpp  →  Content: DB init code
Choice : 4  →  Message: Setup project
Choice : 8  →  Keyword: database
Choice : 9
```

**Expected Output:**
```
  Search results for "database":
  ----------------------------------------
  Commit #1  ...
  (matched in filename)
  ----------------------------------------
```

**Validates:** Filename-level search inside commit snapshots.

---

### Test Case 10 — Data Persistence Across Sessions

**Purpose:** Verify that commits saved in Session 1 are correctly reloaded in Session 2.

**Session 1 Inputs:**
```
Repository name : PersistTest
Choice : 2  →  persist.txt  →  I will survive a restart
Choice : 4  →  Message: Persistence check
Choice : 9
```

**Session 2 Inputs (run the program again):**
```
Repository name : PersistTest
Choice : 5
Choice : 9
```

**Expected Output (Session 2):**
```
  [INFO] Repository 'PersistTest' loaded (1 commits).

  Total commits: 1
  ----------------------------------------
  Commit #1  [xxxxxxxx...]  YYYY-MM-DD HH:MM:SS
  Message : Persistence check
  Files (1): persist.txt
  ----------------------------------------
```

**Validates:** `saveToFile()` / `loadFromFile()`, file handling requirement.

---

### Test Case 11 — Input Validation: Empty Commit Message

**Purpose:** Verify the system rejects a blank commit message.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  x.txt  →  something
Choice : 4  →  Message: (press Enter without typing anything)
Choice : 9
```

**Expected Output:**
```
  [ERROR] Commit message cannot be empty.
```

**Validates:** Input validation in `CommitOperation::execute()`.

---

### Test Case 12 — Input Validation: Non-Numeric Menu Choice

**Purpose:** Verify the system handles non-numeric menu input gracefully without crashing.

**Inputs:**
```
Repository name : TestRepo
Menu choice     : abc
Menu choice     : 9
```

**Expected Output:**
```
  [ERROR] Invalid input — enter a number.
  Data saved. Goodbye!
```

**Validates:** Exception handling (`std::stoi` wrapped in `try/catch`) in `CLI::run()`.

---

### Test Case 13 — Commit on Empty Working Directory

**Purpose:** Verify the system refuses to commit when the working directory is empty.

**Inputs:**
```
Repository name : TestRepo
Choice : 4
Choice : 9
```

**Expected Output:**
```
  [ERROR] Nothing to commit. Add files first.
```

**Validates:** Guard logic in `CommitOperation::execute()`.

---

### Test Case 14 — Delete Non-Existent File

**Purpose:** Verify a descriptive error is shown when the user tries to delete a file that does not exist.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  real.txt  →  actual content
Choice : 3  →  Filename to delete: ghost.txt
Choice : 9
```

**Expected Output:**
```
  [ERROR] File 'ghost.txt' not found in working directory.
```

**Validates:** Return-value checking in `Repository::deleteFile()`.

---

### Test Case 15 — Revert to Non-Existent Commit Number

**Purpose:** Verify a descriptive error is shown when the commit number does not exist.

**Inputs:**
```
Repository name : TestRepo
Choice : 2  →  a.txt  →  data
Choice : 4  →  Message: Only commit
Choice : 7  →  Revert to commit: 99
Choice : 9
```

**Expected Output:**
```
  [ERROR] Commit #99 not found.
```

**Validates:** `Repository::revert()` correctly returns false for unknown commit numbers.

---

## ⚙️ Assumptions & Limitations

| Item | Detail |
|------|--------|
| **File content** | Stored as a single logical line. Embedded newlines within content are encoded (`<<NL>>`) and decoded transparently when displayed. |
| **Repository name** | Must not contain spaces. Use underscores or CamelCase (e.g., `MyProject`, `oop_lab`). |
| **No branching** | Single linear commit history only. Git-style branching and merging are out of scope. |
| **No diff engine** | The system stores and displays full file content per commit, not line-by-line diffs. |
| **Local storage only** | Data is persisted to `<RepoName>_repo.dat` in the current working directory. No network or remote support. |
| **Single user** | No concurrency handling — only one process should access the repository at a time. |
| **Revert is destructive** | Reverting replaces the working directory immediately. Uncommitted changes in the working directory will be lost. |
| **Binary files** | Not suited for binary content, as everything is stored in a plain-text `.dat` file. |
