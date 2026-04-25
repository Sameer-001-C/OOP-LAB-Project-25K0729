#include "CommitMetadata.h"
#include <iostream>

int CommitMetadata::commitCounter = 0;

CommitMetadata::CommitMetadata()
    : commitId(""), message(""), timestamp(""), commitNumber(0) {}

CommitMetadata::CommitMetadata(const std::string& msg) {
    ++commitCounter;
    commitNumber = commitCounter;
    message      = msg;

    // Capture current date-time as timestamp
    time_t now = time(nullptr);
    char   buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    timestamp = std::string(buf);

    commitId = generateId(message, timestamp);
}

CommitMetadata::CommitMetadata(const std::string& id, const std::string& msg,
                               const std::string& ts, int num)
    : commitId(id), message(msg), timestamp(ts), commitNumber(num) {}

CommitMetadata::~CommitMetadata() {}

// ---------- Getters ----------
std::string CommitMetadata::getCommitId()     const { return commitId;     }
std::string CommitMetadata::getMessage()      const { return message;      }
std::string CommitMetadata::getTimestamp()    const { return timestamp;    }
int         CommitMetadata::getCommitNumber() const { return commitNumber; }

// ---------- Setters ----------
void CommitMetadata::setCommitId    (const std::string& id)  { commitId     = id;  }
void CommitMetadata::setMessage     (const std::string& msg) { message      = msg; }
void CommitMetadata::setTimestamp   (const std::string& ts)  { timestamp    = ts;  }
void CommitMetadata::setCommitNumber(int num)                 { commitNumber = num; }

// ---------- Display ----------
void CommitMetadata::display() const {
    std::cout << "  Commit #" << commitNumber
              << "  [" << commitId.substr(0, 8) << "...]"
              << "  " << timestamp << std::endl;
    std::cout << "  Message : " << message << std::endl;
}

// ---------- Static helpers ----------
void CommitMetadata::setCounter(int val) { commitCounter = val; }
int  CommitMetadata::getCounter()        { return commitCounter; }

std::string CommitMetadata::generateId(const std::string& message,
                                       const std::string& timestamp) {
    std::string combined = message + timestamp;
    unsigned long hash   = 5381;
    for (unsigned char c : combined) {
        hash = ((hash << 5) + hash) + c;   // djb2
    }
    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return oss.str();
}
