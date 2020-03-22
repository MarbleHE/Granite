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


Circuit unequalCircuit(){
    Circuit dup1 = duplicatorCircuit();
    Circuit dup2 = duplicatorCircuit();
    Circuit allin = par(dup1,dup2);
    Circuit t1 = par(par(single_unary_gate_circuit(Gate::Alias), smallerCircuit()),single_unary_gate_circuit(Gate::Alias));
    Circuit t2 = par(swapCircuit(),single_unary_gate_circuit(Gate::Alias));
    Circuit t1seqt2 = seq(t1,t2);
    Circuit t3 = par(single_unary_gate_circuit(Gate::Alias), greaterCircuit());
    Circuit t1tot3 = seq(t1seqt2, t3);
    return seq(allin, seq(t1tot3, single_binary_gate_circuit(Gate::Add)));
}

Circuit booleanUnequalCircuit(){
    return single_binary_gate_circuit(Gate::Add);
}

Circuit booleanEqualCircuit(){
    return seq(booleanUnequalCircuit(), single_unary_gate_circuit(Gate::Negate));
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


Circuit smallerCircuit(){
    return seq(swapCircuit(), single_binary_gate_circuit(Gate::Compare));
}

Circuit greaterCircuit(){
    return single_binary_gate_circuit(Gate::Compare);
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

Circuit booleanOrCircuit(){
    Circuit eq;
    Wire in1 = eq.add_input("in1");
    Wire in2 = eq.add_input("in2");
    Wire t1 = eq.add_assignment("t1", Gate::Multiply, in1, in2);
    Wire b1 = eq.add_assignment("t2", Gate::Add, in1, in2);
    Wire out = eq.add_assignment("out", Gate::Add, t1, b1);
    eq.set_output(out);
    return eq;
}

Circuit greaterZeroCircuit(){
    Circuit dup = duplicatorCircuit();
    Circuit first = seq(dup, par(single_unary_gate_circuit(Gate::Alias), single_unary_gate_circuit(Gate::Negate)));
    return seq(first, single_binary_gate_circuit(Gate::Compare));
};

Circuit smallerZeroCircuit(){
    Circuit dup = duplicatorCircuit();
    Circuit first = seq(dup, par(single_unary_gate_circuit(Gate::Negate), single_unary_gate_circuit(Gate::Alias)));
    return seq(first, single_binary_gate_circuit(Gate::Compare));
}

Circuit arithmeticOr(){
    return seq(single_binary_gate_circuit(Gate::Add),greaterZeroCircuit());
}

Circuit reBoolCircuit(){
    Circuit dup = duplicatorCircuit();
    Circuit t = seq(dup, par(greaterZeroCircuit(), smallerZeroCircuit()));
    return seq(seq(t,single_unary_gate_circuit(Gate::Add)), greaterZeroCircuit());
}