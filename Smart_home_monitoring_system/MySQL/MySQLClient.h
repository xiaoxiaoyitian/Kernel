#ifndef _MYSQLCLIENT_H
#define _MYSWLCLIENT_H
#include <vector>
#include <string>
#include <mysql/mysql.h>

using std::vector;
using std::string;
namespace wxt{

class MySQLClient{
public:
    MySQLClient();
    ~MySQLClient();
    void init();
    bool connect(const string &host,
                 const string &user,
                 const string &password,
                 const string &db,
                 unsigned short port);
    //写操作封装(INSERT/UPDATE/DELETE)
    bool writeOperationQuery(const string &sql);
    //读操作封装(SELECT)
    vector<vector<string>> readOperationQuery(const string &sql);
    //打印查询结果
    static void display(const vector<vector<string>> &res);
private:
    MYSQL _conn;
    MYSQL* pconn;
};
}
#endif

