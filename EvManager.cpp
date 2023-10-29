#include "EvManager.hpp"
#include <unistd.h>

int EvManager::_i = 0;
int EvManager::_numEvents = 0;
const int EvManager::CLIENT_LIMIT;
int EvManager::_kq = 0;
struct kevent EvManager::_evList[1000];
const int EvManager::read;
const int EvManager::write;
const int EvManager::error;

bool EvManager::start() {
    if (_kq == 0) {
        _kq = kqueue();
        if (_kq == -1) {
            throw std::runtime_error(std::string("kqueue: ") + strerror(errno));
        }
    }
    return (true);
}

bool EvManager::addEvent(int fd, int flag) {
    struct kevent evSet;
    EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);

    if (kevent(_kq, &evSet, 1, NULL, 0, NULL) == -1) {
        throw std::runtime_error(std::string("kevent: ") + strerror(errno));
    };
    return (true);
}

bool EvManager::delEvent(int fd, int flag) {
    std::cout << "\ndelEvent" << std::endl;
    struct kevent evSet;
    EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    if (kevent(_kq, &evSet, 1, NULL, 0, NULL) == -1) {
        throw std::runtime_error(std::string("kevent: ") + strerror(errno));
    };
    close(fd);
    return (true);
}

std::pair<int, int> EvManager::listen() {
    while (_numEvents == 0) {
        _numEvents = kevent(_kq, NULL, 0, _evList, CLIENT_LIMIT, NULL);
    }
    std::cout << "_numEvents = " << _numEvents << std::endl;
    if (_numEvents == -1) {
        throw std::runtime_error(std::string("kevent: ") + strerror(errno));
    }
    if (_evList[_numEvents - 1].flags == EV_ERROR) {
        throw std::runtime_error(std::string("event: ") + strerror(errno));
    }
    std::pair<int, int> result;
    if (_evList[_numEvents - 1].flags & EV_EOF) {
        result = std::make_pair<int, int>(EV_EOF, _evList[_numEvents - 1].ident);
    } else if (_evList[_numEvents - 1].filter == EVFILT_READ) {
        result = std::make_pair<int, int>(_evList[_numEvents - 1].filter, _evList[_numEvents - 1].ident);
    }
    --_numEvents;
    return (result);
}