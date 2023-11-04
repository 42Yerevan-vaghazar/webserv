#include "EvManager.hpp"
#include <unistd.h>

int EvManager::_i = 0;
int EvManager::_numEvents = 0;
const int EvManager::CLIENT_LIMIT;
int EvManager::_kq = 0;
struct kevent EvManager::_evList[1000];

bool EvManager::start() {
    if (_kq == 0) {
        _kq = kqueue();
        if (_kq == -1) {
            throw std::runtime_error(std::string("kqueue: ") + strerror(errno));
        }
    }
    return (true);
}

bool EvManager::addEvent(int fd, Flag flag) {
    if (_kq != 0) {
        int evFlag = getFlag(flag);
        struct kevent evSet;
        EV_SET(&evSet, fd, evFlag, EV_ADD, 0, 0, NULL);

        if (kevent(_kq, &evSet, 1, NULL, 0, NULL) == -1) {
            throw std::runtime_error(std::string("kevent: ") + strerror(errno));
        };
        return (true);
    }
    return (false);
}

bool EvManager::delEvent(int fd, Flag flag) {
    if (_kq != 0) {
        std::cout << "\ndelEvent" << std::endl;
        int evFlag = getFlag(flag);
        struct kevent evSet;
        EV_SET(&evSet, fd, evFlag, EV_DELETE, 0, 0, NULL);
        int res = kevent(_kq, &evSet, 1, NULL, 0, NULL);
        // if (res == -1) {
        //     throw std::runtime_error(std::string("kevent: ") + strerror(errno));
        // };
        // close(fd);
        return (true);
    }
    return (false);
}

std::pair<EvManager::Flag, int> EvManager::listen() {
    if (_numEvents == 0) {
        memset(_evList, 0, CLIENT_LIMIT);
        while (_numEvents == 0) {
            _numEvents = kevent(_kq, NULL, 0, _evList, CLIENT_LIMIT, NULL);
        }
    }
    std::cout << "\n\n_numEvents = " << _numEvents << std::endl;
    if (_numEvents == -1 || _evList[_numEvents - 1].flags == EV_ERROR) {
        throw std::runtime_error(std::string("kevent: ") + strerror(errno));
    }
    EvManager::Flag flag = error;

    if (_evList[_numEvents - 1].flags & EV_EOF) {
        flag = eof;
    } else if (_evList[_numEvents - 1].filter == EVFILT_READ) {
        flag = read;
    } else if (_evList[_numEvents - 1].filter == EVFILT_WRITE) {
        flag = write;
    }
    std::pair<EvManager::Flag, int> result = std::make_pair<EvManager::Flag, int>(flag, _evList[_numEvents - 1].ident);
    --_numEvents;
    return (result);
}

int EvManager::getFlag(Flag flag) {
    switch (flag)
    {
    case read :
        return (EVFILT_READ);
    case write :
        return (EVFILT_WRITE);
    case eof :
        return (EV_EOF);
    case error :
        return (EV_ERROR);
    default :
        return (def);
    }
    return (def);
};