class LdSt{
public:
	std::string Input;
	BrigInstLdSt Output;
	BrigOperandAddress RefAddr;
	BrigOperandReg RefReg[2];
	BrigOperandCompound RefComp;
	void validate(BrigInstLdSt get){
		EXPECT_EQ(Output.size, get.size);
		EXPECT_EQ(Output.kind, get.kind);
		EXPECT_EQ(Output.opcode, get.opcode);
		EXPECT_EQ(Output.type, get.type);
		EXPECT_EQ(Output.packing, get.packing);
		EXPECT_EQ(Output.o_operands[0], get.o_operands[0]);
		EXPECT_EQ(Output.o_operands[1], get.o_operands[1]);
		EXPECT_EQ(Output.o_operands[2], get.o_operands[2]);
		EXPECT_EQ(Output.o_operands[3], get.o_operands[3]);
		EXPECT_EQ(Output.o_operands[4], get.o_operands[4]);
		EXPECT_EQ(Output.storageClass, get.storageClass);
		EXPECT_EQ(Output.memorySemantic, get.memorySemantic);
		EXPECT_EQ(Output.equivClass, get.equivClass);
	}
	void init(std::string input, BrigInstLdSt ref, BrigOperandReg RefReg0, BrigOperandAddress RefAddr,
			 BrigOperandReg* RefReg1=NULL,  BrigOperandCompound* RefComp=NULL){
		(this->Input).assign(input);
		this->Output = ref;
		this->RefReg[0] = RefReg0;
		this->RefAddr = (RefAddr);
		if(RefReg1) this->RefReg[1] = (*RefReg1);
		if(RefComp) this->RefComp = (*RefComp);
	}
	void validate(BrigOperandAddress get){
		EXPECT_EQ(RefAddr.size, get.size);
		EXPECT_EQ(RefAddr.kind, get.kind);
		EXPECT_EQ(RefAddr.type, get.type);
		EXPECT_EQ(RefAddr.reserved, get.reserved);
		//EXPECT_EQ(RefAddr.directive, get.directive);	
		
	}
	void validate(BrigOperandCompound get){
		EXPECT_EQ(RefComp.size, get.size);
		EXPECT_EQ(RefComp.kind, get.kind);
		EXPECT_EQ(RefComp.type, get.type);
		EXPECT_EQ(RefComp.reserved, get.reserved);
		EXPECT_EQ(RefComp.name, get.name);	
		EXPECT_EQ(RefComp.reg, get.reg);	
		EXPECT_EQ(RefComp.offset, get.offset);	
	}
	void validate(BrigOperandReg get0, int i){
		EXPECT_EQ(RefReg[i].size, get0.size);
		EXPECT_EQ(RefReg[i].kind, get0.kind);
		EXPECT_EQ(RefReg[i].type, get0.type);
		EXPECT_EQ(RefReg[i].reserved, get0.reserved);
		EXPECT_EQ(RefReg[i].name, get0.name);
		}	
	
};

LdSt TestCase_LdSt[20]; 
void Init_LdStTestCases(){

	std::string in; 
	in.assign( "ld_arg_f32 $s0, [%input];\n");
	BrigInstLdSt out = {
    44,                // size
    BrigEInstLdSt,     // kind
    BrigLd,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {8, 32, 44, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
	};
	
	BrigOperandReg reg1 = {
	sizeof(BrigOperandReg),
	BrigEOperandReg,
	Brigb32,
	0,
	15  //Check!
	};
	
	BrigOperandAddress addr = {
	sizeof(BrigOperandAddress),
	BrigEOperandAddress,
	Brigb64,
	0,
	0  //Offset to .directives, However, no directive in .directives. Ambiguous testing.
	};
	
	TestCase_LdSt[0].init(in, out, reg1, addr);	
		
	in.assign("st_arg_f32 $s0, [%output][$s2-4];\n");
	BrigInstLdSt out1 = {
    44,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {8, 44, 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  
    //Ref to $s0
    BrigOperandReg reg2 = {
	sizeof(BrigOperandReg),
	BrigEOperandReg,
	Brigb32,
	0,
	16
	};
  	
	//Ref to $s2
	BrigOperandReg reg3 = {
	sizeof(BrigOperandReg),
	BrigEOperandReg,
	Brigb32,
	0,
	20 
	};
	
	//Ref to Compound Addr
	BrigOperandCompound comp = {
	sizeof(BrigOperandCompound),
	BrigEOperandCompound,
	Brigb64,
	0,
	20,//byte offset to operand addr
	32, //to operand reg
	-4	
	};
	
	TestCase_LdSt[1].init(in, out1, reg2, addr, &reg3, &comp);	
}
