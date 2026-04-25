#pragma once
#include <string>
#include <iostream>

// Forward declaration — avoids circular include with Repository.h
class Repository;

// ============================================================
//  Abstract Base Class: VCOperation
//  Represents a single version-control command the user can run.
//  Each derived class overrides execute() — this is the primary
//  demonstration of Abstraction + Polymorphism in the system.
// ============================================================
class VCOperation {
public:
    virtual ~VCOperation() {}

    // Pure virtual functions — derived classes MUST implement these
    virtual void        execute(Repository& repo) = 0;
    virtual std::string getOperationName()  const = 0;
    virtual std::string getDescription()    const = 0;

    // Non-pure virtual — provides a default implementation
    virtual void displayInfo() const;
};


// ============================================================
//  Derived Operation 1: Status
// ============================================================
class StatusOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Status";           }
    std::string getDescription()        const override { return "Show working directory files"; }
};

// ============================================================
//  Derived Operation 2: Add / Update File
// ============================================================
class AddFileOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Add / Update File"; }
    std::string getDescription()        const override { return "Add or update a file in working directory"; }
};

// ============================================================
//  Derived Operation 3: Delete File
// ============================================================
class DeleteFileOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Delete File";       }
    std::string getDescription()        const override { return "Remove a file from working directory"; }
};

// ============================================================
//  Derived Operation 4: Commit
// ============================================================
class CommitOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Commit";            }
    std::string getDescription()        const override { return "Snapshot current working directory"; }
};

// ============================================================
//  Derived Operation 5: Log
// ============================================================
class LogOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "View Log";          }
    std::string getDescription()        const override { return "Show all commit history"; }
};

// ============================================================
//  Derived Operation 6: Show Commit Detail
// ============================================================
class ShowCommitDetailOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Commit Detail";     }
    std::string getDescription()        const override { return "Inspect files inside a commit"; }
};

// ============================================================
//  Derived Operation 7: Revert
// ============================================================
class RevertOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Revert";            }
    std::string getDescription()        const override { return "Restore working dir to a commit"; }
};

// ============================================================
//  Derived Operation 8: Search
// ============================================================
class SearchOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Search Commits";    }
    std::string getDescription()        const override { return "Filter commits by keyword"; }
};
