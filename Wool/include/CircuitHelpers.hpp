//
// Created by mario on 18.03.20.
//

#ifndef GRANITE_CIRCUITHELPERS_HPP
#define GRANITE_CIRCUITHELPERS_HPP

#include "simple-circuits.hpp"
#include "circuit-util.hpp"
#include "circuit.hpp"

//
// Created by mario on 18.03.20.
//

#include "circuit.hpp"
#include "circuit-util.hpp"
#include "simple-circuits.hpp"

Circuit duplicatorCircuit(){
    Circuit duplicator;
    Wire in1 = duplicator.add_input("in1");
    Wire out1 = duplicator.add_assignment("out1", Gate::Alias, in1);
    Wire out2 = duplicator.add_assignment("out2", Gate::Alias, in1);
    duplicator.set_output(out1);
    duplicator.set_output(out2);
    return duplicator;
}

Circuit equalCircuit(){
    Circuit dup = duplicatorCircuit();
    Circuit first = seq(par(dup,dup), par(single_binary_gate_circuit(Gate::Add), single_binary_gate_circuit(Gate::Multiply)));
    Circuit second = seq(first, par(single_unary_gate_circuit(Gate::Negate), single_unary_gate_circuit(Gate::Alias)));
    return seq(second, single_binary_gate_circuit(Gate::Multiply));
}

Circuit unequalCircuit(){
    return seq(equalCircuit(), single_unary_gate_circuit(Gate::Negate));
}

Circuit swapCircuit(){
    Circuit swap;
    Wire in1 = swap.add_input("in1");
    Wire in2 = swap.add_input("in2");
    Wire out1 = swap.add_assignment("out1", Gate::Alias, in2);
    Wire out2 = swap.add_assignment("out2", Gate::Alias, in1);
    swap.set_output(out1);
    swap.set_output(out2);
    return swap;
}

Circuit smallerEqualCircuit(){
    return seq(single_binary_gate_circuit(Gate::Compare),single_unary_gate_circuit(Gate::Negate));
}

Circuit smallerCircuit(){
    return seq(swapCircuit(), single_binary_gate_circuit(Gate::Compare));
}

Circuit greaterEqualCircuit(){
    return seq(smallerCircuit(), single_unary_gate_circuit(Gate::Negate));
}

#endif //GRANITE_CIRCUITHELPERS_HPP
