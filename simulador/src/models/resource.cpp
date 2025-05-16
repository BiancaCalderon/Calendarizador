#include "resource.h"
#include <algorithm>

Resource::Resource(const std::string& name, int count)
    : name(name), initialCount(count), availableCount(count) {}

std::string Resource::getName() const {
    return name;
}

int Resource::getInitialCount() const {
    return initialCount;
}

int Resource::getAvailableCount() const {
    return availableCount;
}

const std::vector<std::string>& Resource::getAccessQueue() const {
    return accessQueue;
}

bool Resource::acquire(const std::string& processId) {
    if (availableCount > 0) {
        availableCount--;
        return true;
    }
    return false;
}

void Resource::release(const std::string& processId) {
    if (availableCount < initialCount) {
        availableCount++;
    }
    // Eliminar el proceso de la cola si está presente
    removeFromQueue(processId);
}

void Resource::addToQueue(const std::string& processId) {
    if (!isInQueue(processId)) {
        accessQueue.push_back(processId);
    }
}

void Resource::removeFromQueue(const std::string& processId) {
    auto it = std::find(accessQueue.begin(), accessQueue.end(), processId);
    if (it != accessQueue.end()) {
        accessQueue.erase(it);
    }
}

bool Resource::isInQueue(const std::string& processId) const {
    return std::find(accessQueue.begin(), accessQueue.end(), processId) != accessQueue.end();
}

void Resource::reset() {
    availableCount = initialCount;
    accessQueue.clear();
}