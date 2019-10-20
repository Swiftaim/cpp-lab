#include "CodeBuilder.h"
#include "misc.h"
#include "fsm.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <thread>

using namespace std;

void test_filter()
{
    vector<shared_ptr<Product> > items{
        make_shared<Product>("T-shirt", Color::blue, Size::small),
        make_shared<Product>("T-shirt", Color::blue, Size::medium),
        make_shared<Product>("T-shirt", Color::blue, Size::large),
        make_shared<Product>("T-shirt", Color::red, Size::small),
        make_shared<Product>("T-shirt", Color::red, Size::medium),
        make_shared<Product>("T-shirt", Color::red, Size::large),
        make_shared<Product>("T-shirt", Color::red, Size::xl),
        make_shared<Product>("T-shirt", Color::green, Size::medium),
        make_shared<Product>("Trousers", Color::blue, Size::small),
        make_shared<Product>("Trousers", Color::blue, Size::medium),
        make_shared<Product>("Trousers", Color::blue, Size::large),
        make_shared<Product>("Sweater", Color::green, Size::large)
    };

    auto result = ProductFilter::by_color(items, Color::blue);
    transform(begin(result), end(result), ostream_iterator<Product>(cout), [](shared_ptr<Product>& ptr) {
        return *ptr;
    });

    BetterProductFilter bpf;
    ColorAndSizeSpec spec{Color::green, Size::medium};
    auto bpf_result = bpf.filter(items, spec);
    transform(begin(bpf_result), end(bpf_result), ostream_iterator<Product>(cout), [](shared_ptr<Product>& ptr) {
        return *ptr;
    });
}

void test_fsm(size_t ticks)
{
    const size_t delay = 100; // millisecond delay in tick function
    
    struct Object {
        int cooldown = 5;
        void reset() { cooldown = 5; }
    };

    using StateMachine = FSM::FSM<FSM::State, FSM::Message, Object>;

    StateMachine fsm;
    
    fsm.declareState(FSM::State::idle, 
        { 
            { FSM::Message::start,  FSM::State::walking },
            { FSM::Message::stop,   FSM::State::idle }
        }, 
        [](StateMachine& fsm) {
        },
        [=](Object& object, StateMachine& fsm) { 
            if (object.cooldown > 0) {
                std::cout << "[" << fsm.getCurrentState() << "] - ";
                std::cout << "cooldown = " << object.cooldown-- << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
            else {
                object.reset();
                fsm.sendMessage(FSM::Message::start, object);
            }
        }
    )
    .declareState(FSM::State::walking,
        {
            { FSM::Message::stop,       FSM::State::idle },
            { FSM::Message::arrived,    FSM::State::rotating }
        },
        [](StateMachine& fsm) {
        },
        [=](Object& object, StateMachine& fsm) {
            if (object.cooldown > 0) {
                std::cout << "[" << fsm.getCurrentState() << "] - ";
                std::cout << "cooldown = " << object.cooldown-- << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
            else {
                object.reset();
                fsm.sendMessage(FSM::Message::arrived, object);
            }
        }
    )
    .declareState(FSM::State::rotating,
        {
            { FSM::Message::settled,    FSM::State::resting },
            { FSM::Message::stop,       FSM::State::idle }
        },
        [](StateMachine& fsm) {
        },
        [=](Object& object, StateMachine& fsm) {
            if (object.cooldown > 0) {
                std::cout << "[" << fsm.getCurrentState() << "] - ";
                std::cout << "cooldown = " << object.cooldown-- << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
            else {
                object.reset();
                fsm.sendMessage(FSM::Message::settled, object);
            }
        }
    )
    .declareState(FSM::State::resting,
        {
            { FSM::Message::stop,       FSM::State::idle },
            { FSM::Message::woke_up,    FSM::State::idle }
        },
        [](StateMachine& fsm) {
        },
        [=](Object& object, StateMachine& fsm) {
            if (object.cooldown > 0) {
                std::cout << "[" << fsm.getCurrentState() << "] - ";
                std::cout << "cooldown = " << object.cooldown-- << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
            else {
                object.reset();
                fsm.sendMessage(FSM::Message::woke_up, object);
            }
        }
    )
    .setInitialState(FSM::State::idle);

    Object object;
    while (ticks-- > 0) {
        fsm.tick(object);
    }
}

void codelock()
{
    class CombinationLock
    {
        enum class States { locked, unlocking, open, error };
        enum class Messages { enter_digit, valid, invalid, lock };
        using StateMachine = FSM::FSM<States, Messages, int>;

        vector<int> _combination;
        StateMachine fsm;
    public:

        string status;

        CombinationLock(const vector<int> &combination) : _combination(combination), status("LOCKED") {
            std::cout << "CombinationLock.combination.size() = " << this->_combination.size() << std::endl;
            fsm.declareState(States::locked,
                {
                    { Messages::enter_digit,  States::unlocking }
                },
                [&](StateMachine& fsm) {
                    status = "LOCKED";
                },
                [&](int& digit, StateMachine& fsm) {
                    fsm.sendMessage(Messages::enter_digit, digit);
                }
            )
            .declareState(States::unlocking,
                {
                    { Messages::enter_digit,    States::unlocking },
                    { Messages::valid,          States::open },
                    { Messages::invalid,        States::error }
                },
                [&](StateMachine& fsm) {
                    status.clear();
                },
                [&](int& digit, StateMachine& fsm) {
                    static size_t index = 0;

                    if (_combination[index++] == digit) {
                        status += std::to_string(digit);
                    } else {
                        index = 0;
                        fsm.sendMessage(Messages::invalid, digit);
                    }
                    
                    if (index == _combination.size()) {
                        index = 0;
                        fsm.sendMessage(Messages::valid, digit);
                    }
                }
            )
            .declareState(States::open,
                {
                    { Messages::lock,  States::locked }
                },
                [&](StateMachine& fsm) {
                    status = "OPEN";
                },
                [&](int& digit, StateMachine& fsm) {
                    
                }
            )
            .declareState(States::error,
                {
                    { Messages::enter_digit,  States::unlocking }
                },
                [&](StateMachine& fsm) {
                    status = "ERROR";
                },
                [&](int& digit, StateMachine& fsm) {
                    
                }
            )
            .setInitialState(States::locked);
        }

        void enter_digit(int digit)
        {
            std::cout << "enter digit: " << digit << std::endl;
            fsm.tick(digit);
        }
    };

    vector<int> combination{1,2,3,4,5,6,7,8};
    CombinationLock lock(combination);
    for (auto i : combination) {
        lock.enter_digit(i); std::cout << lock.status << std::endl;
    }
}

int main(int argc, char* argv[])
{    
    // test_fsm(25);
    codelock();

    getchar();
    return 0;
}
