#include "MySQLClient.h"
#include <iostream>
#include <mysql/mysql.h>
using std::cout;
using std::endl;
namespace wxt{
MySQLClient::MySQLClient()
{
    init();
}
MySQLClient::~MySQLClient(){
    //释放连接
    if(pconn){
        mysql_close(pconn);
    }
}

void MySQLClient::init(){
    //初始化MYSQL 结构体
    pconn=mysql_init(&_conn);
    if(!pconn){
        printf("MySQLClient init error.\n");
    }
}

bool MySQLClient::connect(const string &host,const string &user,const string &password,const string &db,unsigned short port){
    MYSQL *pconn=mysql_real_connect(&_conn,host.data(),user.data(),password.data(),db.data(),port,nullptr,0);
    if(!pconn){
        printf("(%d,%s)\n",mysql_errno(&_conn),mysql_error(&_conn));
        return false;
    }else{
        return true;
    }

}

//写操作封装(INSERT/UPDATE/DELETE)
bool MySQLClient::writeOperationQuery(const string &sql){
    int ret=mysql_real_query(pconn,sql.data(),sql.size());
    if(ret!=0){
        //查询出错
        printf("(%d,%s)\n",mysql_errno(pconn),mysql_error(pconn));
        return false;
    }else{
        //查询成功
        printf("Query Ok, %ld row(s) affected.\n",mysql_affected_rows(pconn));
        return true;
    }
}

//读操作封装(SELECT)
vector<vector<string>> MySQLClient::readOperationQuery(const string &sql){
    int ret=mysql_real_query(pconn,sql.data(),sql.size());
    if(ret!=0){
        //select查询失败的情况
        printf("(%d,%s)\n",mysql_errno(pconn),mysql_error(pconn));
        return vector<vector<string>>();
    }

    vector<vector<string>> queryResult;//存储查询的结果
    MYSQL_RES *result=mysql_store_result(pconn);//获取并缓存查询结果集
    if(!result){
        //错误处理
        printf("(%d,%s)\n",mysql_errno(pconn),mysql_error(pconn));
        return vector<vector<string>>();
    }

    int rows=mysql_num_rows(result);
    if(rows==0){
        //无结果时
        return vector<vector<string>>();
    }

    //有结果集的情况
    queryResult.reserve(rows+1);
    int cols=mysql_num_fields(result);
    printf("rows:%d,clos:%d\n",rows,cols);

    MYSQL_FIELD *filed=nullptr;
    vector<string> fileds;
    
    //读取列名并加入结果集
    while((filed=mysql_fetch_field(result))!=nullptr){
        fileds.push_back(filed->name);//存储列名
    }
    queryResult.push_back(std::move(fileds));//转移到容器中
    
    //读取每一行的数据
    MYSQL_ROW row;
    while((row=mysql_fetch_row(result))!=nullptr){
        vector<string> temp;
        temp.reserve(cols);
        for(int i=0;i<cols;++i){
            if(row[i]!=NULL){
                temp.push_back(string(row[i]));
            }else{
                temp.push_back(string());
            }
        }
        //获取一行数据，加入结果集中
        queryResult.push_back(temp);
    }

    //释放结果集
    mysql_free_result(result);
    return queryResult;
}
void MySQLClient::display(const vector<vector<string>> &res){
    cout << "result set has " << res.size() << " row(s)" << endl;
    for(auto & vec : res) {
        for(auto & filed : vec) {
            cout << filed << "\t";
        }
        cout << endl;
    }
    cout << endl;

}
}
