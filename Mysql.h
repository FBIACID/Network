#pragma once
#include "stdax.h"
#ifndef MYSQL_DB
#define MYSQL_DB

static const char* _Host_name_ = "localhost";
static const char* _User_name_ = "y_L";
static const char* _Password_ = "yuanliu182040";

void Init_Mysql(MYSQL &db);
string Select(string& sql,MYSQL &db);
void Insert(string& sql,MYSQL &db);
void Delete(string& sql,MYSQL &db);
void Close(MYSQL &DB);
#endif // !MYSQL_DB


