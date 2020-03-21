#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm.h"

int regs[NB_REGISTRES];
struct instruction instructions[MAX_INSTRUCTIONS];

int cmpt_instructions = 0;

int breakpoints[100];
int cmpt_breakpoints = 0;

char mode = '\n';

int print_buffer[100];
int cmpt_print_buffer = 0;

char *str_instructions[7] = {"COP", "AFC", "PRI", "ADD", "SOU", "MUL", "DIV"};

void execute()
{
  int i = 0, pass;
  char c;
  printf("\e[1;1H\e[2J");
  print_registers();
  print_instructions(0);
  while (i < cmpt_instructions)
  {
    pass = print_input_mode(i);
    printf("\e[1;1H\e[2J");
    if (pass)
    {
      execute_instruction(i);
      i++;
    }
    print_registers();
    print_instructions(i);
    print_printbuffer();
  }
  printf("\e[1;1H\e[2J");
  print_registers();
  print_instructions(cmpt_instructions);
  print_printbuffer();
}

void add_instruction(int type, int val1, int val2, int val3)
{
  instructions[cmpt_instructions].type = type;
  instructions[cmpt_instructions].val1 = val1;
  instructions[cmpt_instructions].val2 = val2;
  instructions[cmpt_instructions].val3 = val3;
  cmpt_instructions++;
}

void execute_instruction(int index)
{
  struct instruction inst = instructions[index];

  switch (inst.type)
  {
  case COP:
    regs[inst.val1] = regs[inst.val2];
    break;
  case AFC:
    regs[inst.val1] = inst.val2;
    break;
  case PRI:
    print_buffer[cmpt_print_buffer] = regs[inst.val1];
    cmpt_print_buffer++;
    break;
  case ADD:
    regs[inst.val1] = regs[inst.val2] + regs[inst.val3];
    break;
  case SOU:
    regs[inst.val1] = regs[inst.val2] - regs[inst.val3];
    break;
  case MUL:
    regs[inst.val1] = regs[inst.val2] * regs[inst.val3];
    break;
  case DIV:
    regs[inst.val1] = regs[inst.val2] / regs[inst.val3];
    break;
  }
}

void print_registers()
{
  int i;
  for (i = 0; i < NB_REGISTRES; i++)
  {
    printf("R%-2d 0x%08d  ", i, regs[i]);
    if ((i + 1) % 8 == 0)
      printf("\n");
  }
  printf("\n");
}

void print_instructions(int line)
{
  int i;
  struct instruction inst;
  for (i = line - 2; i <= line + 2; i++)
  {
    if (i < 0)
      printf("  START\n");
    else if (i < cmpt_instructions)
    {
      inst = instructions[i];
      if (i == line)
        printf("> ");
      else if (check_breakpoints(i))
        printf("o ");
      else
        printf("  ");
      printf("%s %d %d %d\n", str_instructions[inst.type - 1], inst.val1, inst.val2, inst.val3);
    }
    else
      printf("  END\n");
  }
  printf("\n");
}

void print_printbuffer()
{
  if (cmpt_print_buffer == 0)
    return;
  int i;
  printf("PRINT :\n");
  for (i = 0; i < cmpt_print_buffer; i++)
  {
    printf("%d\n", print_buffer[i]);
  }
  printf("\n");
}

int print_input_mode(int line)
{
  if (mode != 'r' || check_breakpoints(line))
  {
    printf("[Enter] Step by step | [r] Run | [b n] Breakpoint line n (same to remove)\n");
    printf("Mode : ");
    scanf("%c", &mode);
    if (mode == 'b')
    {
      add_breakpoint();
    }
    if (mode != '\n')
      getchar();
  }
  printf("\n");
  return (mode != 'b');
}

void add_breakpoint()
{
  int i;
  int line;
  scanf("%d", &line);
  if (line > 0 && line <= cmpt_instructions)
  {
    for (i = 0; i < cmpt_breakpoints; i++)
    {
      if (breakpoints[i] == line)
      {
        breakpoints[i] = -1;
        return;
      }
    }
    breakpoints[cmpt_breakpoints] = line;
    cmpt_breakpoints++;
  }
}

int check_breakpoints(int line)
{
  int i;
  for (i = 0; i < cmpt_breakpoints; i++)
  {
    if (breakpoints[i] == line + 1)
      return 1;
  }
  return 0;
}