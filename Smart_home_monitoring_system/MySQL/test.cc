#include "MySQLClient.h"
#include <cstdlib>
#include <mysql/mysql.h>

using namespace wxt;
int main()
{
    MySQLClient myclient;
    bool ret = myclient.connect("192.168.101.128", "root", "123", "wxt",3306);
    if(!ret) {
        printf("2222");
        return EXIT_FAILURE;
    }
    //string sql1 = "INSERT INTO person VALUES(14, 'beiliya', 40, 60, 60, 60)";
    //myclient.writeOperationQuery(sql1);
    string sql2 = "select * from Student";
    vector<vector<string>> res = myclient.readOperationQuery(sql2);
    MySQLClient::display(res);
    return 0;
}
