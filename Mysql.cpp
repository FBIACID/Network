#include "Mysql.h"


void Init_Mysql(MYSQL &db)
{
	MYSQL mydata;
	assert(!mysql_server_init(0, NULL, NULL));
	if (!mysql_init(&mydata)) {
		std::cerr << "Mysql start faied quit" << endl;
		return;
	}
	if (mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk"))
	{
		cerr << "Mysql option error." << endl;
		return;
	}
	if (mysql_real_connect(&mydata, _Host_name_, _User_name_, _Password_, "data", 3306, NULL, 0))
	{
		db = mydata; return;
	}
	else {
		cerr << "error quit" << endl;
		return;
	}
}

string Select(string& sql,MYSQL &db)
{
	MYSQL_RES* result = NULL;
	string pid;
	if (!mysql_query(&db, sql.c_str()))
	{
		result = mysql_store_result(&db);
		/*// 取得并打印行数
		int rowcount = mysql_num_rows(result);
		cout << "row count: " << rowcount << endl;
		
		// 取得并打印各字段的名称
		unsigned int fieldcount = mysql_num_fields(result);
		MYSQL_FIELD* field = NULL;
		for (unsigned int i = 0; i < fieldcount; i++) {
			field = mysql_fetch_field_direct(result, i);
			cout << field->name << "\t\t";
		}
		cout << endl;

		// 打印各行
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		while (NULL != row) {
			for (int i = 0; i < fieldcount; i++) {
				cout << row[i] << "\t\t";
			}
			cout << endl;
			row = mysql_fetch_row(result);
		}*/
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		if (row != NULL) {
			pid = row[0];
		}
		else
		{
			pid = "!error++";
		}
	}
	mysql_free_result(result);
	return pid;

}
void Insert(string& sql,MYSQL &db)
{
	if (!mysql_query(&db, sql.c_str())) {
		return;
	}
	else
	{
		cerr << "Have an error Insert to Mysql" << endl;
	}
}
void Delete(string& sql, MYSQL& db) {
	if (!mysql_query(&db, sql.c_str()))
	{
		return;
	}
	else
	{
		cerr << "Have an error in delete from Mysql" << endl;
	}
}
void Close(MYSQL &DB)
{
	mysql_close(&DB);
	mysql_server_end();
}