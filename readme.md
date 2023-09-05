编译过程:
   词法分析 ->  语法分析 -> 语义分析 -> 中间代码生成 -> 代码优化 -> 目标代码生成 -> 目标代码优化 -> 链接


```
x86 GNU汇编
   
通用寄存器:
      %rax  %eax  %ax   %al   返回值
      %rcx  %ecx  %cx   %cl   参数4  
      %rdx  %edx  %dx   %dl   参数3
      %rsi  %esi  %si   %sil  参数2
      %rdi  %esi  %di   %dil  参数1
      %rbp  %ebp  %bp   %bpl  基址针寄存器(被调用者保存,函数执行结束之后需要还原)
      %rsp  %esp  %sp   %spl  堆栈指针寄存器(栈顶,栈是向下生长的 随着栈的生长而逐渐变小)
      %r8   %r8d  %r8w  %r8b  参数5
      %r9   %r9d  %r9w  %r9b  参数6
      %r10  %r10d %r10w %r10b 其他寄存器
      %r11  %r11d %r11w %r11b 其他寄存器
      %r12  %r12d %r12w %r12b 其他寄存器
      %r13  %r13d %r13w %r13b 其他寄存器
      %r14  %r14d %r14w %r14b 其他寄存器
      %r15  %r15d %r15w %r15b 其他寄存器

标志寄存器EFLAFS (状态标志):
   CF(Carry Flag)	进位标志
      运算中，当数值的最高位产生了进位或者借位，CF位都会置1，否则为0。它可用于检测无符号整数运算结果是否溢出。也可用于多精度运算中。
   PF(Parity Flag) 奇偶标志
      用于标记结果低8位中1的个数，如果为偶数， PF位为1，否则为0 。注意，是最低的那8位，不管操作数是16位，还是32位。奇偶校验经常用于数据传输开始时和结束后的对比，判断传输过程中是否出现错误。
   AF(Auxiliary Carry Flag)	辅助进位标志
      辅助进位标志，用来记录运算结果低4位的进、借位情况，即若低半字节有进、借位，AF为1，否则为0。
   ZF(Zero Flag)	零值标志
      若计算结果为0，此标志位置1，否则为0。
   SF(Sign Flag)	符号标志
      若运算结果为负，则SF位为1，否则为0。
   OF(Overflow Flag)	溢出标志
      用来标识计算的结果是否超过了数据类型可以表示的范围，若OF为1，表示有溢出，为0则未溢出。专门用于检测有符号整数运算结果是否溢出。

操作数:
      0到3个操作数,多个操作数之间以逗号（“,”）分隔 源数据值可以以常数形式给出，或是从寄存器或内存中读取。结果可以存放在寄存器或内存中。
      1.立即数（immediate），用来表示常数值。立即数通过在整数前加一个“$”来表示。比如，$-577或$0x1F;
      2.寄存器（register），它表示某个寄存器的内容，通过在寄存器名声前加上“%”来表示。比如：%eax或%al；
      3.内存引用，它会根据计算出来的地址（通常称为有效地址）访问某个内存位置。内存引用的语法：segment:offset(base, index, scale)。
      D[segment]+offset+R[base]+R[index]*scale

指令后缀:
      b 1字节  w 2字节  l 4字节  q 8字节

指令:
      MOV S, D       S → D   从源位置复制到目的位置
      MOVZ           在将较小的源值复制到较大的目的时,把目的中剩余的字节填充位0
      MOVS           通过符号扩展来填充，把源操作数的最高位进行复制
      cbtw           把%al符号扩展到%ax
      cwtl           把%ax符号扩展到%eax
      cwtd           把%ax符号扩展到%dx:%ax
      cltq           把%eax符号扩展到%rax
      cltd           把%eax符号扩展到%edx:%eax
      cqto           把%rax符号扩展为八字
      cqtd           把%rax符号扩展为八字
      pushq S        将四字压入栈  
      等价 (
         subq $8, %rsp
         movq %rbp, (%rsp)
      )

      popq D         将四字弹出栈
      等价 (
         movq (%rsp), %rax
         addq $8, %rsp
      )
      leaq S, D               加载有效地址
      inc{bwlq} D    	      加 1
      dec{bwlq} D             减 1
      neg{bwlq} D             取反
      add{bwlq}   S,D   D +S -> D        加
      sub{bwlq}   S,D   D - S -> D        减
      imul{bwlq}  S,D   D * S -> D     乘
      idiv{bwlq}  S, D ->S     除
      not{bwlq} D	      ~D → D	逻辑非
      or{bwlq} S, D	   D | S → D	逻辑或
      and{bwlq} S, D	   D & S → D	逻辑与
      xor{bwlq} S, D	   D ^ S → D	逻辑异或
      sal{bwlq} k, D	   D << k → D	左移
      shl{bwlq} k, D	   D << k → D	左移（等同于asl）
      sar{bwlq} k, D	   D >>_A k → D	算术右移
      shr{bwlq} k, D	   D >>_L k → D	逻辑右移

      imulq S	S × R[%rax] → R[%rdx]: R[%rax]	64位有符号乘法
      // 除法单操作数 以 %rdx 对应的寄存器保存余数， %rax对应的寄存器保存商
      idivq S	R[%rdx]: R[%rax] mod S → R[%rdx]    R[%rdx]: R[%rax] ÷ S → R[%rax]	64位有符号除法


      call Label     过程调用
      ret	         从过程调用返回

      jmp             无条件跳转

段相关:
   .text	      代码段
   .rodata	   只读数据段
   .data	      数据段
   .bss	      未初始化数据段。bss段用于本地通用变量存储。
   .LC0        字符串常量标签，用于表示一个字符串常量的地址
```