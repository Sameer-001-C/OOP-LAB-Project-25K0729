#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

class CommitMetadata {
protected:
    std::string commitId;
    std::string message;
    std::string timestamp;
    int         commitNumber;

    static int commitCounter;

public:
    CommitMetadata();
    explicit CommitMetadata(const std::string& message);
    CommitMetadata(const std::string& id, const std::string& msg,
                   const std::string& ts, int num);
    virtual ~CommitMetadata();

    std::string getCommitId()     const;
    std::string getMessage()      const;
    std::string getTimestamp()    const;
    int         getCommitNumber() const;

    void setCommitId    (const std::string& id);
    void setMessage     (const std::string& msg);
    void setTimestamp   (const std::string& ts);
    void setCommitNumber(int num);

    virtual void display()         const;
    virtual void displayDetailed() const = 0;

    static void        setCounter(int val);
    static int         getCounter();
    static std::string generateId(const std::string& message,
                                  const std::string& timestamp);
};
