#include <vector>
#include <iostream>

enum state {s1, s2, s3, s4, s5, s6};
enum action {left, right, up, down};

const std::vector<std::vector<action>> actions = {{right, down},{right, down, left},{},{up, right},{left, up, right},{left, up}};

int reward(state s, action a){
    if(s == s2 && a == right)
        return 50;
    if(s == s6 && a == up)
        return 100;
    return 0;
}

state next_state(state s, action a){
    switch (s){
        case s1:
            if (a == right) return s2;
            if (a == down) return s4;
            break;
        case s2:
            if (a == left) return s1;
            if (a == right) return s3;
            if (a == down) return s5;
            break;
        case s4:
            if (a == up) return s1;
            if (a == right) return s5;
            break;
        case s5:
            if (a == right) return s6;
            if (a == up) return s2;
            if (a == left) return s4;
            break;
        case s6:
            if (a == left) return s5;
            if (a == up) return s3;
            break;
        default:
            return s;
    };
}

int main(int agrc, char *argv[]){
    std::cout << "Value Iteration\n";
}