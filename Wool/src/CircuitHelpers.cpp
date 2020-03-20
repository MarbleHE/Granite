//
// Created by mario on 20.03.20.
//

#include "CircuitHelpers.hpp"
#include "simple-circuits.hpp"
#include "circuit-util.hpp"
#include "circuit.hpp"

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

Circuit rotateCircuit(){
    Circuit rot;
    Wire in1 = rot.add_input("in1");
    Wire inAmount = rot.add_const_input("inAmount");
    Wire out1 = rot.add_assignment("out1", Gate::Rotate, in1, inAmount);
    rot.set_output(out1);
    return rot;
}

Circuit constAliasCircuit(){
    Circuit c;
    Wire cin1 = c.add_const_input("cin1");
    Wire out1 = c.add_assignment("out1", Gate::Alias, cin1);
    c.set_output(out1);
    return c;
}