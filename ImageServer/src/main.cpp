#include <iostream>
#include <string>
#include "async_tcp_server.h"
#include "image_file_process.h"
#include "data_buf.h"
#include "path_proc.h"
#include "date_time.h"

using namespace std;

void test_file_process()
{
    std::string rootpath("test");
    ImageFileProcess imp(rootpath);
    imp.run();
    ImageFile_t img[20];
    for (size_t i = 0; i < 20; i++)
    {
        DataBuf *pDb = new DataBuf(100);
        char * pbuf = pDb->getData();
        string str;
        str = string("img ") + to_string(i) + string(" :\n") + string("hello image file process");
        memcpy(pbuf, str.c_str(), str.length());
        img[i].buf = pDb;
        img[i].name = string("img") + to_string(i) + ".jpg";
    }
    
    
    
}

void test_data_buf()
{
    DataBuf db(20);
    char * pbuf = db.getData();
    string str = "hello data buf";
    memcpy(pbuf, str.c_str(), str.length());

    cout << db.getData() << endl;
    cout << db.getSize() << endl;
    cout << "move:\n";

    DataBuf db1 = std::move(db);
    cout << db1.getData() << endl;
    cout << db1.getSize() << endl;
}

void test_date_time()
{
    DateTime dt(2021, 7, 3, 18, 38, 40, 0);
    cout <<  dt.toString() << endl;

    string str("2020-12-1-13-2-36-15");
    DateTime dt1 = dt.fromString(str);
    cout <<  dt1.toString() << endl;
}

void test_path_proc()
{
    std::string rootpath("test");
    PathProc proc(rootpath);
    vector<string> v = proc.getFilesList();
    for (auto &&i : v)
    {
        cout << i << "   ";
    }
    v = proc.getDirList();
    cout << "\ndirs: \n";
    for (auto &&i : v)
    {
        cout << i << "   ";
    }
    cout << "\nnext 5: \n";
    v = proc.getNextFilesList(3);
    for (auto &&i : v)
    {
        cout << i << "   ";
    }
    cout << "\nnext 8: \n";
    v = proc.getNextFilesList(4);
    for (auto &&i : v)
    {
        cout << i << "   ";
    }
    cout << endl;
}

using namespace boost::asio;
int main()
{
    // try{
    //     io_context io;
    //     AsyncTcpServer *tcp_server = new AsyncTcpServer(&io);
    //     tcp_server->start_accept();
    //     io.run();
    // }catch(std::exception& e){
    //     std::cerr << e.what() << std::endl;
    // }

    return 0;
}