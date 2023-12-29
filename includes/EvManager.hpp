#pragma once
#include "DefaultSetup.hpp"
#include	<sys/types.h>
#ifdef __linux__
# include <sys/select.h>
# include <set>
#else
# include	<sys/event.h>
#endif
#include	<sys/time.h>
#include    <utility>
#include    <iostream>

class EvManager
{
    private:
        EvManager();
        EvManager(const EvManager &rhs);
        EvManager &operator=(const EvManager &rhs);
        ~EvManager();
    

    public:
        enum Flag {
            write,
            read,
            eof,
            error,
            def
        };

        static bool start();

        static int getFlag(Flag flag);

        static bool addEvent(int fd, Flag flag);

        static bool delEvent(int fd, Flag flag);

        static std::pair<Flag, int> listen();

    private:
        static int _i;
        static int _numEvents;
        static const int CLIENT_LIMIT = 1000;
#ifdef __linux__
        static fd_set          _rfds;
        static fd_set          _wfds;
        static fd_set          _activeRfds;
        static fd_set          _activeWfds;
        static int _nfds;
        static std::set<int>         _fdRSet;
        static std::set<int>         _fdWSet;
        static std::set<int>         _fdActiveSet;
        static int _curFd;
#else
        static int _kq;
        static struct kevent _evList[CLIENT_LIMIT];
#endif

};