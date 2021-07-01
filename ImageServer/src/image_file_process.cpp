#include <sstream>
#include "../include/globle_def.h"
#include "../include/image_file_process.h"
#include <exception>

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
    path p(m_currentPath);
    if(exists(p))
    {
        if(is_directory(p))
        {
            for (directory_entry &i : directory_iterator(p))
            {
                if(is_regular_file(i.path()))
                {
                    m_currentPathFiles.push_back(i.path().native());
                }
            }        
        }
    }
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
    return m_currentPathFiles;
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

std::vector<std::string> Path::getNextFilesList(size_t num)
{
    vector<string> fileList;
    if(num > m_currentPathFiles.size())
    {
        return fileList;
    }
    else if(m_pos + num >= m_currentPathFiles.size())
    {
        for (int i = m_currentPathFiles.size() - 1 - num; i < m_currentPathFiles.size(); i++)
        {
            fileList[i - m_currentPathFiles.size() + 1 + num] = m_currentPathFiles[i];
        }
        m_pos = m_currentPathFiles.size() - 1;
    }
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
    boost::thread th(boost::bind(&ImageFileProcess::threadEntry, this));
    m_threadId = -1;
}

ImageFileProcess::~ImageFileProcess()
{

}

DataBuf* ImageFileProcess::getOneImage() /*获取 count数量 图片 */
{
    DataBuf* pBuf = nullptr;
    if(!m_readBuffers.empty())
    {
        pBuf = m_readBuffers.front();
        m_readBuffers.pop();
    }    
    return pBuf;
}

int ImageFileProcess::writeOneImage(DataBuf* pBuf)
{
    m_writeBuffers.push(pBuf);
    return OK;
}

int ImageFileProcess::loadSomeImage(int count)
{
    vector<string> strList = m_path->getNextFilesList(count);
    //todo 获取要打开的文件名称

    for (int i = 0; i < count; i++)
    {
        DataBuf * p = fileRead(strList[i]);
        m_readBuffers.push(p);
    }
    
    //m_fileStreams read and update
    return OK;
}

DataBuf* ImageFileProcess::fileRead(string fileName)
{
    if(count > FILE_MAX_CHCHE_NUM - m_fileBuffers.size())
    {
        return nullptr;
    }

    fstream fs;
    try
    {
        fs.open(fileName, fstream::in);
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
    }
    fs.seekg(0, fs.end);
    int length = fs.tellg();
    fs.seekg(0, fs.beg);
    DataBuf pBuf = new DataBuf(length);
    fs.read(pBuf->getData(), length);
    fs.close();    

    return pBuf;
}

void ImageFileProcess::fileWrite(DataBuf* pBuf, string fileName)
{
    fstream fs;
    try
    {
        fs.open(fileName, fstream::out);
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
    }
    fs.write(pBuf->getData(), pBuf->getSize());
    fs.close();    

    return ;
}

void ImageFileProcess::threadEntry()
{
    while(true)
    {
        if(m_readBuffers.size() < FILE_MAX_CHCHE_NUM/2)
        {
            loadSomeImage(FILE_MAX_CHCHE_NUM);
        }
        if(!m_writeBuffers.empty())
        {
            DataBuf* pBuf =  m_writeBuffers.front();
            fileWrite(pBuf, "a.img");
        }
        boost::this_thread::sleep(boost::chrono::milliseconds(1));
    }
}