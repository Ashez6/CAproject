#include <stdio.h>

// Enumerated types for state codes and return codes
enum state_codes {
    IF,
    ID,
    EX,
    MEM,
    WB,
};

enum ret_codes {
    OK,
    OK2,
    END
};

// Struct for representing transitions
struct transition {
    enum state_codes src_state;
    enum ret_codes ret_code;
    enum state_codes dst_state;
};

// Array of state function pointers
typedef void (*state_fn)(void);

// State functions
void fetch(void) { }
void decode(void) { /* ... */ }
void execute(void) { /* ... */ }
void memory(void) { /* ... */ }
void writeBack(void) { /* ... */ }


// Array of state function pointers
state_fn state[] = {
    fetch,
    decode,
    execute,
    memory, 
    writeBack
};

// Transition rules
struct transition state_transitions[] = {
     {IF, OK, ID},
     {ID, OK, ID},
     {ID, OK2, EX},
     {EX, OK, EX},
     {EX, OK2, MEM},
     {MEM, OK, WB}
};

// Function to look up the next state based on the current state and return code
enum state_codes lookup_transitions(enum state_codes cur_state, enum ret_codes rc) {
    for (int i = 0; i < sizeof(state_transitions) / sizeof(state_transitions[0]); ++i) {
        if (state_transitions[i].src_state == cur_state && state_transitions[i].ret_code == rc) {
            return state_transitions[i].dst_state;
        }
    }
    return END; // Default to END state
}

int main(void) {
    enum state_codes cur_state = fetch;
    enum ret_codes rc;
    state_fn state_fun;

    while (1) {
        state_fun = state[cur_state];
        state_fun();

        if (cur_state == IF) {
            fetch(); 
            break;
        }
         if (cur_state == ID) {
            decode(); 
            break;
        }
         if (cur_state == EX) {
            execute(); 
            break;
        }
         if (cur_state == MEM) {
            memory(); 
            break;
        }
         if (cur_state == WB) {
            writeBack(); 
            break;
        }

        // Simulate getting the return code (OK, FAIL, REPEAT)
        // For simplicity, assume it's OK for now
        rc = OK;

        cur_state = lookup_transitions(cur_state, rc);
    }

    return 0;
}
