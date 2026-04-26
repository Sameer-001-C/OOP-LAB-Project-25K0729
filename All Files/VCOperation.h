#pragma once
#include <string>
#include <iostream>

class Repository;

class VCOperation {
public:
    virtual ~VCOperation() {}

    virtual void        execute(Repository& repo) = 0;
    virtual std::string getOperationName()  const = 0;
    virtual std::string getDescription()    const = 0;

    virtual void displayInfo() const;
};


class StatusOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Status"; }
    std::string getDescription()        const override { return "Show working directory files"; }
};

class AddFileOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Add / Update File"; }
    std::string getDescription()        const override { return "Add or update a file in working directory"; }
};

class DeleteFileOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Delete File"; }
    std::string getDescription()        const override { return "Remove a file from working directory"; }
};

class CommitOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Commit"; }
    std::string getDescription()        const override { return "Snapshot current working directory"; }
};

class LogOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "View Log"; }
    std::string getDescription()        const override { return "Show all commit history"; }
};

class ShowCommitDetailOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Commit Detail"; }
    std::string getDescription()        const override { return "Inspect files inside a commit"; }
};

class RevertOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Revert"; }
    std::string getDescription()        const override { return "Restore working dir to a commit"; }
};

class SearchOperation : public VCOperation {
public:
    void        execute(Repository& repo)     override;
    std::string getOperationName()      const override { return "Search Commits"; }
    std::string getDescription()        const override { return "Filter commits by keyword"; }
};
