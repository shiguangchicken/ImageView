#include <sstream>
#include <boost/filesystem.hpp>
#include "../include/globle_def.h"
#include "../include/image_file_process.h"

using namespace std;
using namespace boost::filesystem;

DateTime::DateTime()
{

}

DateTime::DateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour,
                 uint32_t minute, uint32_t second, uint32_t millisecond)
{
    this->year          = year;
    this->month         = month;
    this->day           = day;
    this->hour          = hour;
    this->minute        = minute;
    this->second        = second;
    this->millisecond   = millisecond;
}

string DateTime::toString(DateTime &dateTime)
{
    stringstream ss;
    ss << dateTime.year << "-" << dateTime.month << "-" << dateTime.day \
        << "-" << dateTime.hour << "-" << dateTime.minute << "-" \
        << dateTime.second << "-" << dateTime.millisecond; 
    
    return ss.str();    
}

DateTime DateTime::fromString(std::string &str) /* yyyy-mm-dd-hh-mm-ss-mm */
{
    DateTime dt;
    uint32_t* p[DATE_TIME_MEMBER_NUM] = { &dt.year, &dt.month, &dt.day,\
                &dt.hour, &dt.minute, &dt.second, &dt.millisecond };
    vector<uint32_t *> variable(p, p + DATE_TIME_MEMBER_NUM) ;
    
    string tempStr;
    size_t pos = 0;
    size_t index = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
        if(str[i] == '-')
        {
            *p[index] = stoi(tempStr);
            pos = i + 1;
        }
        else
        {
            tempStr[i - pos] = str[i];
        }
    }     
    return dt;
}

std::string DateTime::toString()
{
    return toString(*this);
}

Path::Path(std::string &rootDir)
{
    m_currentPath = rootDir;
}

string Path::getCurrentPath()
{
    return m_currentPath;
}

std::string Path::getPath(DateTime &dateTime)
{
    return DateTime::toString(dateTime).substr(0, DATE_TIME_DIR_LEN);
}

std::vector<std::string> Path::getFilesList()
{
    path p(m_currentPath);
    vector<string> fileList;
    if(is_directory(p))
    {
        for (directory_entry &i : directory_iterator(p))
        {
            if(is_regular_file(i.path()))
            {
                fileList.push_back(i.path().string());
            }
        }        
    }
    return fileList;
}

std::vector<std::string> Path::getDirList()
{
    path p(m_currentPath);
    vector<string> dirList;
    if(is_directory(p))
    {
        for (directory_entry &i : directory_iterator(p))
        {
            if(is_directory(i.path()))
            {
                dirList.push_back(i.path().native());
            }
        }        
    }
    return dirList;
}

path Path::findPathRecursive(path& p, string fileName)
{
    path nonePath;
    if(exists(p))
    {
        if(is_directory(p))
        {
            for (directory_entry &i : directory_iterator(p))
            {
                if(i.path().filename() == fileName)
                {
                    return i.path();
                }
                path p1 = i.path();
                return findPathRecursive(p1, fileName);
            }        
        }
        return nonePath;
    }
    return nonePath;
}

string Path::findPath(string& fileName)
{
    //todo
    path p(m_currentPath);
    path res = findPathRecursive(p, fileName);
    if(res.empty())
    {
        return "";
    }
    return res.native();
}


DataBuf::DataBuf(size_t size)
{
    if(size < 1024*1024*20)
    {
        m_data = new char[size];
        m_size = size;
    }
    else
    {
        m_data = nullptr;
        m_size = 0;
    }
}

DataBuf::DataBuf(DataBuf &&other)
{
    m_data = other.m_data;
    m_size = other.m_size;

    other.m_data = nullptr;
    other.m_size = 0;
}

DataBuf& DataBuf::operator=(DataBuf &&other)
{
    if(*this == other)
    {
        return *this; 
    }
    m_data = other.m_data;
    m_size = other.m_size;

    other.m_data = nullptr;
    other.m_size = 0;
}


ImageFileProcess::ImageFileProcess(string &rootPath)
{
    m_path = new Path(rootPath);
}

ImageFileProcess::~ImageFileProcess()
{

}

int ImageFileProcess::getOneImages(DataBuf& buf) /*获取 count数量 图片 */
{
    if(!m_fileBuffers.empty())
    {
        buf = m_fileBuffers.front();
        m_fileBuffers.pop();
        return OK;
    }
    
    return ERR;
}

int ImageFileProcess::loadSomeImage(int count)
{

    fileRead(count);
    //m_fileStreams read and update
    return OK;
}

int ImageFileProcess::fileRead(int count)
{
    if(count > FILE_MAX_CHCHE_NUM - m_fileBuffers.size())
    {
        return ERR;
    }

    if(m_path->getLeftFilesList() < count)
    {
        count = m_path->getLeftFilesList();
    }

    for (int i = 0; i < count; i++)
    {
        m_fileStreams[i].open("a.img");
        char* buf = new char(IMAGE_CACHE_SIZE);
        m_fileBuffers.push_back((uint8*)buf);
        while (m_fileStreams[i].eof())
        {
            m_fileStreams[i].read(buf, READ_MAX_SIZE);
        }
    }
    
}