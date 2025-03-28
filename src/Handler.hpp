#ifndef HANDLER_H_
#define HANDLER_H_

#include <iostream>
#include <memory>

// foward declaration of sample class
class Sample;

class Handler {
    public:
        Handler(Sample * s) : sample (s) {}
        virtual ~Handler() = default;
        virtual void handle(std::vector<double>& results) = 0;
        void setNext(std::shared_ptr<Handler> nextHandler) {
            m_pNext = nextHandler;
        };

    protected:
        std::shared_ptr<Handler> m_pNext;
        Sample * sample;

};

#endif // HANDLER_H_
