#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

namespace FSM {

using namespace std;

enum class State {
    idle,
    walking,
    rotating,
    resting
};

ostream& operator<<(ostream& os, const State state) {
    switch (state) {
        case State::idle:
            os << "idle";
            break;
        case State::walking: 
            os << "walking";
            break;
        case State::rotating: 
            os << "rotating";
            break;
        case State::resting: 
            os << "resting";
            break;
        default: 
            break;
    }
    return os;
}

enum class Message {
    start,
    arrived,
    settled,
    woke_up,
    stop
};

ostream& operator<<(ostream& os, const Message Message) {
    switch (Message) {
        case Message::start: 
            os << "start";
            break;
        case Message::arrived: 
            os << "arrived";
            break;
        case Message::settled: 
            os << "settled";
            break;
        case Message::woke_up: 
            os << "woke_up";
            break;
        case Message::stop: 
            os << "stop";
            break;
        default:
            break;
    }
    return os;
}

template <typename STATE_ENUM, typename MESSAGE_ENUM, typename DATA>
class FSM {
public:
    FSM() = default;
    ~FSM() = default;

    FSM<STATE_ENUM, MESSAGE_ENUM, DATA>& declareState(STATE_ENUM state, 
        vector<pair<MESSAGE_ENUM, STATE_ENUM> > transitions, 
        function<void(FSM<STATE_ENUM, MESSAGE_ENUM, DATA>&)> onEnter,
        function<void(DATA&, FSM<STATE_ENUM, MESSAGE_ENUM, DATA>&)> tickFunc);

    void setInitialState(STATE_ENUM initialState);
    const STATE_ENUM getCurrentState() const { return mCurrentState; }

    void tick(DATA& data);

    bool sendMessage(MESSAGE_ENUM Message, DATA& data);

private:
    template <typename S, typename T> using TransitionTable = map<S, vector<pair<T, S> > >;
    template <typename S, typename O> using TickFunctionTable = map<S, function<void(O&, FSM&)> >;
    template <typename S> using OnEnterFunctionTable = map<S, function<void(FSM&)> >;

    TransitionTable<STATE_ENUM, MESSAGE_ENUM> mTransitionTable;
    OnEnterFunctionTable<STATE_ENUM> mOnEnterTable;
    TickFunctionTable<STATE_ENUM, DATA> mTickFunctionTable;
    STATE_ENUM mCurrentState;
};

template <typename STATE_ENUM, typename MESSAGE_ENUM, typename DATA>
void FSM<STATE_ENUM, MESSAGE_ENUM, DATA>::setInitialState(STATE_ENUM initialState)
{
    mCurrentState = initialState;
    mOnEnterTable[mCurrentState](*this);
}

template <typename STATE_ENUM, typename MESSAGE_ENUM, class DATA>
void FSM<STATE_ENUM, MESSAGE_ENUM, DATA>::tick(DATA& data)
{
    mTickFunctionTable[mCurrentState](data, *this);
}

template <typename STATE_ENUM, typename MESSAGE_ENUM, typename DATA>
bool FSM<STATE_ENUM, MESSAGE_ENUM, DATA>::sendMessage(MESSAGE_ENUM Message, DATA& data)
{
    const auto& row = mTransitionTable[mCurrentState];
    auto transition = find_if(row.begin(), row.end(), [=](const pair<MESSAGE_ENUM, STATE_ENUM>& entry) {
        return entry.first == Message ? true : false;
    });
    if (transition != row.end()) {
        mCurrentState = transition->second;
        mOnEnterTable[mCurrentState](*this);
        tick(data);
        return true;
    }
    return false;
}

template <typename STATE_ENUM, typename MESSAGE_ENUM, typename DATA>
FSM<STATE_ENUM, MESSAGE_ENUM, DATA>& FSM<STATE_ENUM, MESSAGE_ENUM, DATA>::declareState(STATE_ENUM state, 
    vector<pair<MESSAGE_ENUM, STATE_ENUM> > transitions,
    function<void(FSM<STATE_ENUM, MESSAGE_ENUM, DATA>&)> onEnter,
    function<void(DATA&, FSM<STATE_ENUM, MESSAGE_ENUM, DATA>&)> tickFunc)
{
    mTransitionTable[state] = transitions;
    mOnEnterTable[state] = onEnter;
    mTickFunctionTable[state] = tickFunc;
    
    return *this;
}

}