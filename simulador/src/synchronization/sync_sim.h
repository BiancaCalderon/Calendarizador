#pragma once
#include <vector>
#include "../models/process.h"
#include "../models/resource.h"
#include "../models/action.h"

class SyncSim {
public:
    void initialize(const std::vector<Process>&  p,
                    const std::vector<Resource>& r,
                    const std::vector<Action>&   a,
                    bool useMutex);

    void run();                 // recorre los ciclos y muestra estados
private:
    struct TimelineEvent {
        int cycle;
        std::string pid, op, res;
        bool accessed;          // true = ACCESSED, false = WAITING
    };

    int currentCycle_ = 0;
    bool useMutex_ = false;
    std::vector<Process>  processes_;
    std::vector<Resource> resources_;
    std::vector<Action>   actions_;
    std::vector<TimelineEvent> timeline_;

    Resource* findResource(const std::string& name);
    void printTimeline() const;         // salida simple en consola
};
