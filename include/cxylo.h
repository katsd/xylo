//
// Created by Katsu Matsuda on 2020/04/30.
//

#ifndef _CXYLO_H_
#define _CXYLO_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	INT,
	FLOAT,
	STRING,
} CObjType;

typedef union
{
	long ival;
	double dval;
	const char* str;
} CObjValue;

typedef struct
{
	CObjType type;

	CObjValue value;
} CObj;

void* CreateXylo(const char* source);

void RunXylo(void* xylo);

void RunXyloFunc(void* xylo, const char* func_name);

void AddXyloFunc(void* xylo, char* func_name, unsigned long arg_num, CObj (* func)(CObj args[], unsigned long arg_num));

#ifdef __cplusplus
}
#endif

#endif //_CXYLO_H_