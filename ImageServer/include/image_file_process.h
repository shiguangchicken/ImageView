#pragma once

#include <vector>
#include <queue>
#include <string>
#include <stdint.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>

#define DATE_TIME_DIR_LEN 10
#define DATE_TIME_MEMBER_NUM 7
#define FILE_MAX_CHCHE_NUM 10
#define READ_MAX_SIZE 0xffff
#define IMAGE_CACHE_SIZE (20 * 1024 * 1024) //20M

class DateTime
{
public:
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
    uint32_t millisecond;

    DateTime();

    DateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour,
                 uint32_t minute, uint32_t second, uint32_t millisecond);
    ~DateTime();

    static std::string toString(DateTime &dateTime); /* y-m-d-h-m-s-ms */
    
    static DateTime fromString(std::string &str);
    
    std::string toString();
    
};

class Path
{
private:
    /* data */
    std::string m_currentPath; /*  */ 
    unsigned short m_pos; //记录当前读取文件的位置
    std::vector<std::string> m_currentPathFiles;
    std::vector<std::string> m_currentPathDirs;
public:
    explicit Path(std::string &rootDir);

    ~Path();
    
    std::string getCurrentPath();

    std::string getPath(DateTime &dateTime);

    /* 列出当前文件夹下的所有文件 */
    std::vector<std::string> getFilesList();

    std::vector<std::string> getDirList();

    /* 查找文件所在绝对路径 */
    std::string findPath(string& fileName);

    std::vector<std::string> getNextFilesList(size_t num); //获取后面num个文件的名字

private:
    boost::filesystem::path findPathRecursive(boost::filesystem::path& p, std::string fileName);

};

class DataBuf
{
private:
    char* m_data;/* data */
    size_t m_size;
public:
    explicit DataBuf(size_t size);

    DataBuf(const DataBuf &other) = delete;

    DataBuf& operator=(const DataBuf &other) = delete;

    DataBuf(DataBuf &&other);

    DataBuf& operator=(DataBuf &&other);

    inline char* getData()
    {
        return m_data;
    }
    inline size_t getSize()
    {
        return m_size;
    }
};

class ImageFileProcess
{
private:
    /* data */
    Path* m_path; /* the root path of image files */
    
    uint8_t m_cacheSize;
    
    std::vector<std::fstream> m_fileStreams;
    
    // std::vector<uint8_t*> m_fileBuffers; /* 缓存的图片 */

    std::queue<DataBuf*> m_readBuffers; //读取的缓存图片

    std::queue<DataBuf*> m_writeBuffers; //待写入的缓存图片

    boost::thread::id m_threadId;

public:
    ImageFileProcess(std::string &rootPath);

    ~ImageFileProcess();

    DataBuf* getOneImage(); /*获取一张图片 */

    int writeOneImage(DataBuf* pBuf);
   
private:
    int loadSomeImage(int count);
    
    DataBuf* fileRead(string fileName); //读取文件

    void fileWrite(DataBuf* pBuf, string fileName); //写文件 

    void threadEntry();

};
