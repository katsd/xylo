//
// Created by Katsu Matsuda on 2020/04/30.
//

#ifndef _CXYLO_H_
#define _CXYLO_H_

#ifdef __cplusplus
extern "C" {
#endif

void* CreateXylo(const char* source);

void RunXylo(void* xylo, const char* func_name);

#ifdef __cplusplus
}
#endif

#endif //_CXYLO_H_
