#pragma once

#include <vector>
#include <queue>
#include <string>
#include <stdint.h>
#include <fstream>

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

    std::vector<std::string> getLeftFilesList();

private:
    path findPathRecursive(path& p, string fileName);

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

    std::queue<DataBuf> m_fileBuffers;

public:
    ImageFileProcess(std::string &rootPath);

    ~ImageFileProcess();

    int getOneImages(DataBuf& buf); /*获取 count数量 图片 */

private:
    int loadSomeImage(int count);
    
    int fileRead(int count);

};
