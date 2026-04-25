#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

// ============================================================
//  Abstract Base Class: CommitMetadata
//  Stores identifying metadata for a commit.
//  Commit derives from this class (inheritance + abstraction).
// ============================================================
class CommitMetadata {
protected:
    std::string commitId;
    std::string message;
    std::string timestamp;
    int         commitNumber;

    static int commitCounter;   // shared across all instances

public:
    CommitMetadata();
    // Constructor used when creating a brand-new commit (auto-generates id/timestamp)
    explicit CommitMetadata(const std::string& message);
    // Constructor used when restoring a commit from file (all fields provided)
    CommitMetadata(const std::string& id, const std::string& msg,
                   const std::string& ts, int num);
    virtual ~CommitMetadata();

    // Getters
    std::string getCommitId()     const;
    std::string getMessage()      const;
    std::string getTimestamp()    const;
    int         getCommitNumber() const;

    // Setters (used during file restore)
    void setCommitId    (const std::string& id);
    void setMessage     (const std::string& msg);
    void setTimestamp   (const std::string& ts);
    void setCommitNumber(int num);

    // Non-pure virtual: provides a default one-line display
    virtual void display() const;

    // Pure virtual: derived class MUST provide detailed display (abstraction)
    virtual void displayDetailed() const = 0;

    // Static helpers for counter management (used by file I/O)
    static void setCounter(int val);
    static int  getCounter();

    // Generates a simple hex hash from message + timestamp
    static std::string generateId(const std::string& message,
                                  const std::string& timestamp);
};
