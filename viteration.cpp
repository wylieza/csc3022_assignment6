#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>

//Hyperparams
double discount_factor = 0.8;

enum state {s1, s2, s3, s4, s5, s6};
enum action {left, right, up, down};

const std::vector<std::vector<action>> actions = {{right, down},{right, down, left},{},{up, right},{left, up, right},{left, up}};

std::vector<double> V(6, 0); //Create array V all values init to zero

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

double new_V(state s){
    double highest_reward = 0;
    double calculated_reward;
    for (action a : actions[s]){
        if(highest_reward < (calculated_reward = reward(s, a) + discount_factor*V[next_state(s, a)]))
            highest_reward = calculated_reward;
    }
    return highest_reward;
}

action PI(state s){
    action a = actions[s][0];
    double highest_reward = reward(s, a) + discount_factor*V[next_state(s, a)];
    double calculated_reward;
    for (auto act = ++actions[s].begin(); act < actions[s].end(); ++act){
        if(highest_reward < (calculated_reward = reward(s, *act) + discount_factor*V[next_state(s, *act)]))
            highest_reward = calculated_reward;
            a = *act;
    }
    return a;
}

void save_V(const std::string &fname){
    std::ofstream outfile(fname);
    if (!outfile){
        std::cout << "Error occured while attempting to write to file!\n";
        exit(0);
    }

    for(int state = s1; state <= s6; ++state){
        outfile << "State " << state+1 << " -> V*(" << state+1 << ") = " << V[state] << std::endl;
    }

    outfile.close();
    
}

int main(int agrc, char *argv[]){
    std::cout << "Value Iteration\n";

    double delta;
    double epsilon = 1e-2;
    double v;
    int iterations = 0;

    do {
        iterations++;
        //std::cout << "Iteration: " << iterations << std::endl;
        delta = 0;
        for (int si = s1; si < s6+1; ++si){
            v = V[si];
            V[si] = new_V((state) si);
            delta = std::max(delta, std::abs(v - V[si]));
        }
        //std::cout << "Delta: " << delta << " Epsilon: " << epsilon << "\n";
    } while (delta > epsilon);

    std::cout << "Policy learned:\n";
    state s = s1;
    while (s != s3){
        std::cout << "s" << s+1 << " : a" << PI(s) << " -> ";
        s = next_state(s, PI(s));
    }
    std::cout << "s" << s+1 << " : done\n";
    std::cout << "Highest Attainable Reward: " << V[s1] << std::endl;

    std::cout << "Number of iterations taken to converge: " << iterations << std::endl; //Question 1 answer
    save_V("optimal_values.txt"); //Question 1 text file
}