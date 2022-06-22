#pragma once

class IServer {
    //FIXME change to use callbacks
    public:
        virtual void start(...) = 0;
        virtual void stop(...) = 0;

        virtual void send(...) = 0;
        virtual void receive(...) = 0;
};