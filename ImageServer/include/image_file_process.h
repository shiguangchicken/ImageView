#ifndef _IMAGE_FILE_PROCESS_H
#define _IMAGE_FILE_PROCESS_H

#include <list>
#include <string>
#include <boost/thread/thread.hpp>

#define FILE_MAX_CHCHE_NUM 10
#define READ_MAX_SIZE 0xffff
#define IMAGE_CACHE_SIZE (20 * 1024 * 1024) //20M


class DataBuf;
class PathProc;

typedef struct
{
    // DateTime dateTime;//str equeal the name of the file 
    DataBuf* buf; //the file data
    std::string name; // name of the file
}FileHead_t, ImageFile_t;

class ImageFileProcess
{
private:
    /* data */
    PathProc* m_path; /* the root path of image files */
    
    uint8_t m_cacheSize;
       
    // std::vector<uint8_t*> m_fileBuffers; /* 缓存的图片 */

    std::list<ImageFile_t> m_readBuffers; //读取的缓存图片

    std::list<ImageFile_t> m_writeBuffers; //待写入的缓存图片

    boost::thread* m_thread;

public:

    void run();

    ImageFileProcess(std::string &rootPath);

    ~ImageFileProcess();

    ImageFile_t& getOneImage(); /*获取一张图片 */

    int writeOneImage(ImageFile_t &pBuf);
   
private:
    int loadSomeImage(int count);
    
    DataBuf* fileRead(std::string fileName); //读取文件

    void fileWrite(DataBuf* pBuf, std::string fileName); //写文件 

    void threadEntry();
};


#endif