#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

//Hyperparams
double discount_factor = 0.8;

enum state {s1, s2, s3, s4, s5, s6};
enum action {left, right, up, down};

//Reward values
int rs2r = 50;
int rs6u = 100;

const std::vector<std::vector<action>> actions = {{right, down},{right, down, left},{},{up, right},{left, up, right},{left, up}};

std::vector<double> V(6, 0); //Create array V all values init to zero

int reward(state s, action a){
    if(s == s2 && a == right)
        return rs2r;
    if(s == s6 && a == up)
        return rs6u;
    return 0;
}

std::string action_name(action a){
    switch (a){
        case 0:
            return "left";
            break;
        case 1:
            return "right";
            break;
        case 2:
            return "up";
            break;
        case 3:
            return "down";
            break;
        default:
            return "";
        
    }
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
        if(highest_reward < (calculated_reward = reward(s, *act) + discount_factor*V[next_state(s, *act)])){
            highest_reward = calculated_reward;
            a = *act;
        }
    }
    return a;
}

void save_results(const std::stringstream &ss){
    std::ofstream outfile("results.txt");
    if (!outfile){
        std::cout << "Error occured while attempting to write to file!\n";
        exit(0);
    }

    outfile << ss.str();

    outfile.close();    
}

void reset(){
    for (auto ent : V){
        ent = 0;
    }
}

int run_vit(){
    reset();

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

    return iterations;
}

int main(int agrc, char *argv[]){
    std::stringstream output;

    output << "Value Iteration\n";

    int iterations = run_vit();
    
    output << "Highest Attainable Reward: " << V[s1] << std::endl;

    output << "\nQ1) Number of iterations taken to converge: " << iterations << std::endl; //Question 1 answer
    output << "\nQ1) Optimal values V*(s)\n";
    for(int state = s1; state <= s6; ++state){
        output << "State " << state+1 << " -> V*(" << state+1 << ") = " << V[state] << std::endl;
    }

    output << "\nQ2) Starting in state s1 the optimal policy is:\n    ";
    state s = s1;
    while (s != s3){
        output << "s" << s+1 << " : " << action_name(PI(s)) << " -> ";
        s = next_state(s, PI(s));
    }
    output << "s" << s+1 << " : done\n";

    output << "\nQ3) Yes, it is possible to adjust the reward function such that the values V*(s)\n"
                    "are different while PI* remains unchanged. For this simple case, this is achieved\n"
                    "by ensuring the reward for s2:right (rs2r) and the reward s6:up (rs6u) are set so the\n"
                    "following equation holds: 0.8*(rs2r) < (0.8^3)*(rs6u)\n\n"
                    "For example: if rs2r remains 50, then any value for rs6u greater than 78.125 will have the same\n"
                    "policy, but result in a unique reward function V*(s) for the states.\n\n";

    //Change reward function and rerun
    rs6u = 79;
    run_vit();

    output << "Setting rs6u to 79 results in policy (starting s1):\n    ";
    s = s1;
    while (s != s3){
        output << "s" << s+1 << " : " << action_name(PI(s)) << " -> ";
        s = next_state(s, PI(s));
    }
    output << "s" << s+1 << " : done\n"
    "\nThe reward function V* however has changed:\n";
    for(int state = s1; state <= s6; ++state){
        output << "State " << state+1 << " -> V*(" << state+1 << ") = " << V[state] << std::endl;
    }

    //display the results to terminal
    std::cout << output.str();

    //write to results file
    save_results(output);

}