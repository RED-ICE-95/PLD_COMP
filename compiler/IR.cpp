void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params) {
    instr.push_back(new IRInstr (this, op, t, params));
}