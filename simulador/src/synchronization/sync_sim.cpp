#include "sync_sim.h"
#include <iostream>
#include <algorithm>

void SyncSim::initialize(const std::vector<Process>& p,
                         const std::vector<Resource>& r,
                         const std::vector<Action>&  a)
{
    processes_  = p;
    resources_  = r;
    actions_    = a;

    // Ordena las acciones por ciclo para procesarlas secuencialmente
    std::sort(actions_.begin(), actions_.end(),
          [](const Action& x, const Action& y){
              return x.getCycle() < y.getCycle();
          });
}

Resource* SyncSim::findResource(const std::string& name)
{
    for (auto& res : resources_)
        if (res.getName() == name) return &res;
    return nullptr;
}

void SyncSim::run()
{
    size_t idx = 0;
    while (idx < actions_.size()) {
        int nextCycle = actions_[idx].getCycle();
        currentCycle_ = nextCycle;

        while (idx < actions_.size() && actions_[idx].getCycle() == currentCycle_) {
            const Action& act = actions_[idx];

            Resource* res = findResource(act.getResourceName());

            bool accessed = false;
            if (res && res->acquire(act.getProcessId())) {
                accessed = true;
            }

            timeline_.push_back({ currentCycle_,
                                  act.getProcessId(),
                                  Action::actionTypeToString(act.getType()),
                                  act.getResourceName(),
                                  accessed });
            ++idx;
        }

        for (auto& r : resources_)
            r.release();

        ++currentCycle_;
    }

    printTimeline();
}

void SyncSim::printTimeline() const
{
    std::cout << "\n--- Sync Timeline ---\n";
    for (const auto& ev : timeline_) {
        std::cout << "Cycle " << ev.cycle << ": "
                  << ev.pid << ' ' << ev.op << ' ' << ev.res << " → "
                  << (ev.accessed ? "ACCESSED" : "WAITING") << '\n';
    }
}
