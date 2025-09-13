#ifndef  __MySQLClient_HH__
#define  __MySQLClient_HH__

#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::vector;

namespace Titan{
class MySQLClient{


public:
    MySQLClient();
    ~MySQLClient();

    //建立连接
    bool connect(const string & host,
             unsigned short port,
             const string & user,
             const string & passwd,
             const string & db);

    bool writeOperationQuery(const string & sql);//写入

    vector<vector<string>> readOperationQuery(const string & sql);//读出

    static void dump(const vector<vector<string>> &res);//展示结果集


private:
    void init();
    
    
private:
    MYSQL _conn;


};
}   

#endif