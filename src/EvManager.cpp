#include "EvManager.hpp"
#include <unistd.h>

int EvManager::_i = 0;
int EvManager::_numEvents = 0;
const int EvManager::CLIENT_LIMIT;
#ifdef __linux__
        fd_set          EvManager::_rfds;
        fd_set          EvManager::_wfds;
        fd_set          EvManager::_activeRfds;
        fd_set          EvManager::_activeWfds;
        int             EvManager::_nfds;
        std::set<int>         EvManager::_fdSet;
        std::set<int>::iterator EvManager::_itFds;
#else
        int EvManager::_kq = 0;
        struct kevent EvManager::_evList[1000];
#endif

bool EvManager::start() {
    FD_ZERO(&_rfds);
    FD_ZERO(&_wfds);
    _nfds = 0;
    _itFds = _fdSet.end();

    // if (_kq == 0) {
    //     _kq = kqueue();
    //     if (_kq == -1) {
    //         throw std::runtime_error(std::string("kqueue: ") + strerror(errno));
    //     }
    // }
    // return (true);
}

bool EvManager::addEvent(int fd, Flag flag) {
    if (flag == read) {
        _fdSet.insert(fd);
        FD_SET(fd, &_rfds);
        if (_nfds <= fd) {
            _nfds = fd + 1;
        }
    } else if (flag == write) {
        _fdSet.insert(fd);
        FD_SET(fd, &_wfds);
        if (_nfds <= fd) {
            _nfds = fd + 1;
        }
    }
    // if (_kq != 0) {
    //     int evFlag = getFlag(flag);
    //     struct kevent evSet;
    //     EV_SET(&evSet, fd, evFlag, EV_ADD, 0, 0, NULL);

    //     if (kevent(_kq, &evSet, 1, NULL, 0, NULL) == -1) {
    //         throw std::runtime_error(std::string("kevent: ") + strerror(errno));
    //     };
    //     return (true);
    // }
    // return (false);
}

bool EvManager::delEvent(int fd, Flag flag) {
    if (flag == read) {
        FD_CLR(fd, &_rfds);
        FD_CLR(fd, &_wfds);
        _fdSet.erase(fd);
        if (fd == _nfds - 1) {
            for (int maxFd = _nfds; maxFd >= 0; maxFd--) {
                if (FD_ISSET(maxFd, &_rfds)) {
                    _nfds = maxFd + 1;
                    break;
                }
            }
        }
    } else if (flag == write) {
        FD_CLR(fd, &_rfds);
        FD_CLR(fd, &_wfds);
        _fdSet.erase(fd);
        if (fd == _nfds - 1) {
            for (int maxFd = _nfds; maxFd >= 0; maxFd--) {
                if (FD_ISSET(maxFd, &_rfds)) {
                    _nfds = maxFd + 1;
                    break;
                }
            }
        }
    }
    // if (_kq != 0) {
    //     std::cout << "\ndelEvent" << std::endl;
    //     int evFlag = getFlag(flag);
    //     struct kevent evSet;
    //     EV_SET(&evSet, fd, evFlag, EV_DELETE, 0, 0, NULL);
    //     if (kevent(_kq, &evSet, 1, NULL, 0, NULL) == -1) {
    //         throw std::runtime_error(std::string("kevent: ") + strerror(errno));
    //     };
    //     close(fd);
    //     return (true);
    // }
    // return (false);
}

std::pair<EvManager::Flag, int> EvManager::listen() {
    std::cout << "listen" << std::endl;
    std::cout << "EvManager_itFds = " <<*_itFds << std::endl;
    if (_itFds == _fdSet.end()) {
        _activeRfds = _rfds;
        _activeWfds = _wfds;
        _numEvents = select(_nfds, &_activeRfds, &_activeWfds, NULL, NULL);
        std::cout << "_numEvents = " << _numEvents << std::endl;
        std::cout << "_nfds = " << _nfds << std::endl;
        std::cout << "_fdSetsize = " << _fdSet.size() << std::endl;
        _itFds = _fdSet.begin();
    }
    while (_itFds != _fdSet.end()) {
        std::cout << "_itFds = " <<*_itFds << std::endl;
        if (FD_ISSET(*_itFds, &_activeRfds)) {
            return (std::pair<EvManager::Flag, int>(EvManager::read, *(_itFds++)));
        } else if (FD_ISSET(*_itFds, &_activeWfds)) {
            return (std::pair<EvManager::Flag, int>(EvManager::write, *(_itFds++)));
        }
        _itFds++;
    }
    return (std::pair<EvManager::Flag, int>(EvManager::def, -1));
    // while (_numEvents == 0) {
    //     _numEvents = kevent(_kq, NULL, 0, _evList, CLIENT_LIMIT, NULL);
    // }
    // std::cout << "_numEvents = " << _numEvents << std::endl;
    // if (_numEvents == -1) {
    //     throw std::runtime_error(std::string("kevent: ") + strerror(errno));
    // }
    // if (_evList[_numEvents - 1].flags == EV_ERROR) {
    //     throw std::runtime_error(std::string("event: ") + strerror(errno));
    // }
    // EvManager::Flag flag = error;

    // if (_evList[_numEvents - 1].flags & EV_EOF) {
    //     flag = eof;
    // } else if (_evList[_numEvents - 1].filter == EVFILT_READ) {
    //     flag = read;
    // } else if (_evList[_numEvents - 1].filter == EVFILT_WRITE) {
    //     flag = write;
    // }
    // std::pair<EvManager::Flag, int> result = std::make_pair<EvManager::Flag, int>(flag, _evList[_numEvents - 1].ident);
    // --_numEvents;
    // return (result);
}

int EvManager::getFlag(Flag flag) {
    // switch (flag)
    // {
    // case read :
    //     return (EVFILT_READ);
    // case write :
    //     return (EVFILT_WRITE);
    // case eof :
    //     return (EV_EOF);
    // case error :
    //     return (EV_ERROR);
    // default :
    //     return (def);
    // }
    // return (def);
};