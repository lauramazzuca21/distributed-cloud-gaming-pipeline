#pragma once

class IServer {
    //FIXME find a much better way to implement an interface for the connection between services
    public:
        virtual void start(...) = 0;
        virtual void stop(...) = 0;

        virtual void send(...) = 0;
        virtual void receive(...) = 0;
};