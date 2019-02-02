/*
** $Id: lopcodes.h,v 1.125.1.1 2007/12/27 13:02:25 roberto Exp $
** Opcodes for Lua virtual machine
** See Copyright Notice in lua.h
*/

#ifndef lopcodes_h
#define lopcodes_h

#include "llimits.h"


/*===========================================================================
  We assume that instructions are unsigned numbers.
  All instructions have an opcode in the first 6 bits.
  Instructions can have the following fields:
	`A' : 8 bits 
  A寄存器 ：8bits
	`B' : 9 bits
  B寄存器：9bits
	`C' : 9 bits
  C寄存器：9bits
	`Bx' : 18 bits (`B' and `C' together)
  Bx寄存器18bits（B+C合体）
	`sBx' : signed Bx
  sBx：有符号的Bx

  A signed argument is represented in excess K; that is, the number
  value is the unsigned value minus K. K is exactly the maximum value
  for that argument (so that -max is represented by 0, and +max is
  represented by 2*max), which is half the maximum for the corresponding
  unsigned argument.
===========================================================================*/

//字节码的操作方法被分为了三种类型
//其中iAsBx为有符号的iABx，只不过最大整数为后者的一半，结构本身是一样的
enum OpMode {iABC, iABx, iAsBx};  /* basic instruction format */


/*
** size and position of opcode arguments.
*/
#define SIZE_C		9
#define SIZE_B		9
#define SIZE_Bx		(SIZE_C + SIZE_B)
#define SIZE_A		8

#define SIZE_OP		6

//opcode的位置
#define POS_OP		0
//寄存器A的位置
#define POS_A		(POS_OP + SIZE_OP)
//寄存器C的位置
#define POS_C		(POS_A + SIZE_A)
//寄存器B的位置
#define POS_B		(POS_C + SIZE_C)
//寄存器Bx的位置
#define POS_Bx		POS_C


/*
** limits for opcode arguments.
** we use (signed) int to manipulate most arguments,
** so they must fit in LUAI_BITSINT-1 bits (-1 for sign)
*/
#if SIZE_Bx < LUAI_BITSINT-1
#define MAXARG_Bx        ((1<<SIZE_Bx)-1)
#define MAXARG_sBx        (MAXARG_Bx>>1)         /* `sBx' is signed */
#else
#define MAXARG_Bx        MAX_INT
#define MAXARG_sBx        MAX_INT
#endif


#define MAXARG_A        ((1<<SIZE_A)-1)
#define MAXARG_B        ((1<<SIZE_B)-1)
#define MAXARG_C        ((1<<SIZE_C)-1)


/* creates a mask with `n' 1 bits at position `p' */
#define MASK1(n,p)	((~((~(Instruction)0)<<n))<<p)

/* creates a mask with `n' 0 bits at position `p' */
#define MASK0(n,p)	(~MASK1(n,p))

/**
 * the following macros help to manipulate instructions
 * i为Instruction
 */

//获取opcode
#define GET_OPCODE(i)	(cast(OpCode, ((i)>>POS_OP) & MASK1(SIZE_OP,0)))
//设置opcode
#define SET_OPCODE(i,o)	((i) = (((i)&MASK0(SIZE_OP,POS_OP)) | \
		((cast(Instruction, o)<<POS_OP)&MASK1(SIZE_OP,POS_OP))))

//获取寄存器A
#define GETARG_A(i)	(cast(int, ((i)>>POS_A) & MASK1(SIZE_A,0)))
//设置寄存器A
#define SETARG_A(i,u)	((i) = (((i)&MASK0(SIZE_A,POS_A)) | \
		((cast(Instruction, u)<<POS_A)&MASK1(SIZE_A,POS_A))))

//获取寄存器B
#define GETARG_B(i)	(cast(int, ((i)>>POS_B) & MASK1(SIZE_B,0)))
//设置寄存器B
#define SETARG_B(i,b)	((i) = (((i)&MASK0(SIZE_B,POS_B)) | \
		((cast(Instruction, b)<<POS_B)&MASK1(SIZE_B,POS_B))))

//获取寄存器C
#define GETARG_C(i)	(cast(int, ((i)>>POS_C) & MASK1(SIZE_C,0)))
//设置寄存器C
#define SETARG_C(i,b)	((i) = (((i)&MASK0(SIZE_C,POS_C)) | \
		((cast(Instruction, b)<<POS_C)&MASK1(SIZE_C,POS_C))))

//获取寄存器Bx
#define GETARG_Bx(i)	(cast(int, ((i)>>POS_Bx) & MASK1(SIZE_Bx,0)))
//设置寄存器Bx
#define SETARG_Bx(i,b)	((i) = (((i)&MASK0(SIZE_Bx,POS_Bx)) | \
		((cast(Instruction, b)<<POS_Bx)&MASK1(SIZE_Bx,POS_Bx))))

//获取寄存器sBx
#define GETARG_sBx(i)	(GETARG_Bx(i)-MAXARG_sBx)
//设置寄存器Bx
#define SETARG_sBx(i,b)	SETARG_Bx((i),cast(unsigned int, (b)+MAXARG_sBx))

//创建ABC类型指令
#define CREATE_ABC(o,a,b,c)	((cast(Instruction, o)<<POS_OP) \
			| (cast(Instruction, a)<<POS_A) \
			| (cast(Instruction, b)<<POS_B) \
			| (cast(Instruction, c)<<POS_C))

//创建ABx类型指令
#define CREATE_ABx(o,a,bc)	((cast(Instruction, o)<<POS_OP) \
			| (cast(Instruction, a)<<POS_A) \
			| (cast(Instruction, bc)<<POS_Bx))


/*
** Macros to operate RK indices
*/

/* this bit 1 means constant (0 means register) */
//如果bit为1则是常数
#define BITRK		(1 << (SIZE_B - 1))

/* test whether value is a constant */
//TODO:判断该参数是否为常数 没看懂……
#define ISK(x)		((x) & BITRK)

/* gets the index of the constant */
//获取常量的索引
#define INDEXK(r)	((int)(r) & ~BITRK)

#define MAXINDEXRK	(BITRK - 1)

/* code a constant index as a RK value */
//TODO:RKASK
#define RKASK(x)	((x) | BITRK)


/*
** invalid register that fits in 8 bits
*/
#define NO_REG		MAXARG_A


/*
** R(x) - register
** Kst(x) - constant (in constant table)
** RK(x) == if ISK(x) then Kst(INDEXK(x)) else R(x)
*/


/*
** grep "ORDER OP" if you change these enums
*/

typedef enum {
/*----------------------------------------------------------------------
name		args	description
------------------------------------------------------------------------*/
//在寄存器间拷贝值
OP_MOVE,/*	A B	R(A) := R(B)					*/
//把一常量载入寄存器
OP_LOADK,/*	A Bx	R(A) := Kst(Bx)					*/
//将bool值载入寄存器
OP_LOADBOOL,/*	A B C	R(A) := (Bool)B; if (C) pc++			*/
//将空值载入一系列寄存器
OP_LOADNIL,/*	A B	R(A) := ... := R(B) := nil			*/
//将upvalue读入寄存器
OP_GETUPVAL,/*	A B	R(A) := UpValue[B]				*/

//将一个全局变量读入寄存器
OP_GETGLOBAL,/*	A Bx	R(A) := Gbl[Kst(Bx)]				*/
//将一个表元素读入寄存器
OP_GETTABLE,/*	A B C	R(A) := R(B)[RK(C)]				*/

//将一个寄存器值写入全局变量
OP_SETGLOBAL,/*	A Bx	Gbl[Kst(Bx)] := R(A)				*/
//将一个寄存器值写入上值
OP_SETUPVAL,/*	A B	UpValue[B] := R(A)				*/
//将一个寄存器值写入表元素
OP_SETTABLE,/*	A B C	R(A)[RK(B)] := RK(C)				*/

//创建新表
OP_NEWTABLE,/*	A B C	R(A) := {} (size = B,C)				*/

//为调用对象方法做准备
OP_SELF,/*	A B C	R(A+1) := R(B); R(A) := R(B)[RK(C)]		*/

//加法操作
OP_ADD,/*	A B C	R(A) := RK(B) + RK(C)				*/
//减法操作
OP_SUB,/*	A B C	R(A) := RK(B) - RK(C)				*/
//乘法操作
OP_MUL,/*	A B C	R(A) := RK(B) * RK(C)				*/
//除法操作
OP_DIV,/*	A B C	R(A) := RK(B) / RK(C)				*/
//取模操作
OP_MOD,/*	A B C	R(A) := RK(B) % RK(C)				*/
//取幂操作
OP_POW,/*	A B C	R(A) := RK(B) ^ RK(C)				*/
//一元负操作
OP_UNM,/*	A B	R(A) := -R(B)					*/
//逻辑非操作
OP_NOT,/*	A B	R(A) := not R(B)				*/
//取长度操作
OP_LEN,/*	A B	R(A) := length of R(B)				*/

//连接一系列寄存器操作
OP_CONCAT,/*	A B C	R(A) := R(B).. ... ..R(C)			*/

//无条件跳转
OP_JMP,/*	sBx	pc+=sBx					*/

//相等测试
OP_EQ,/*	A B C	if ((RK(B) == RK(C)) ~= A) then pc++		*/
//小于测试
OP_LT,/*	A B C	if ((RK(B) <  RK(C)) ~= A) then pc++  		*/
//小于等于测试
OP_LE,/*	A B C	if ((RK(B) <= RK(C)) ~= A) then pc++  		*/

//bool测试，带条件跳转
OP_TEST,/*	A C	if not (R(A) <=> C) then pc++			*/ 
//bool测试，带条件跳转以及赋值
OP_TESTSET,/*	A B C	if (R(B) <=> C) then R(A) := R(B) else pc++	*/ 

//调用闭包
OP_CALL,/*	A B C	R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1)) */
//执行尾调用
OP_TAILCALL,/*	A B C	return R(A)(R(A+1), ... ,R(A+B-1))		*/
//返回操作
OP_RETURN,/*	A B	return R(A), ... ,R(A+B-2)	(see note)	*/

//迭代器数字for循环
OP_FORLOOP,/*	A sBx	R(A)+=R(A+2);
			if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }*/
//初始化数字for循环 
OP_FORPREP,/*	A sBx	R(A)-=R(A+2); pc+=sBx				*/

//迭代一般形式的for循环
OP_TFORLOOP,/*	A C	R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2)); 
                        if R(A+3) ~= nil then R(A+2)=R(A+3) else pc++	*/ 
//设置表的一系列数组元素
OP_SETLIST,/*	A B C	R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B	*/

//关闭用作upvalue的一系列局部变量
OP_CLOSE,/*	A 	close all variables in the stack up to (>=) R(A)*/
//创建一函数原型的闭包
OP_CLOSURE,/*	A Bx	R(A) := closure(KPROTO[Bx], R(A), ... ,R(A+n))	*/
//可变参数赋给寄存器
OP_VARARG/*	A B	R(A), R(A+1), ..., R(A+B-1) = vararg		*/
} OpCode;


#define NUM_OPCODES	(cast(int, OP_VARARG) + 1)



/*===========================================================================
  Notes:
  (*) In OP_CALL, if (B == 0) then B = top. C is the number of returns - 1,
      and can be 0: OP_CALL then sets `top' to last_result+1, so
      next open instruction (OP_CALL, OP_RETURN, OP_SETLIST) may use `top'.

  (*) In OP_VARARG, if (B == 0) then use actual number of varargs and
      set top (like in OP_CALL with C == 0).

  (*) In OP_RETURN, if (B == 0) then return up to `top'

  (*) In OP_SETLIST, if (B == 0) then B = `top';
      if (C == 0) then next `instruction' is real C

  (*) For comparisons, A specifies what condition the test should accept
      (true or false).

  (*) All `skips' (pc++) assume that next instruction is a jump
===========================================================================*/


/*
** masks for instruction properties. The format is:
** bits 0-1: op mode
** bits 2-3: C arg mode
** bits 4-5: B arg mode
** bit 6: instruction set register A
** bit 7: operator is a test
*/  

//寄存器类型
enum OpArgMask {
  //参数不被使用
  OpArgN,  /* argument is not used */
  //参数被使用
  OpArgU,  /* argument is used */
  //TODO:argument is a register or a jump offset
  OpArgR,  /* argument is a register or a jump offset */
  //TODO:argument is a constant or register/constant
  OpArgK   /* argument is a constant or register/constant */
};

LUAI_DATA const lu_byte luaP_opmodes[NUM_OPCODES];

//获取opcode的类型
#define getOpMode(m)	(cast(enum OpMode, luaP_opmodes[m] & 3))
//获取寄存器B的参数类型
#define getBMode(m)	(cast(enum OpArgMask, (luaP_opmodes[m] >> 4) & 3))
//获取寄存器C的参数类型
#define getCMode(m)	(cast(enum OpArgMask, (luaP_opmodes[m] >> 2) & 3))
//获取寄存器A的参数类型
#define testAMode(m)	(luaP_opmodes[m] & (1 << 6))
//获取opcode是否为bool测试
#define testTMode(m)	(luaP_opmodes[m] & (1 << 7))


LUAI_DATA const char *const luaP_opnames[NUM_OPCODES+1];  /* opcode names */


/* number of list items to accumulate before a SETLIST instruction */
#define LFIELDS_PER_FLUSH	50


#endif
