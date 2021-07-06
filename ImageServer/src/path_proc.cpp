#include "date_time.h"
#include "path_proc.h"

using namespace std;
using namespace boost::filesystem;

PathProc::PathProc(std::string &rootDir):
    m_pos(0)
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

PathProc::~PathProc()
{

}

string PathProc::getCurrentPath()
{
    return m_currentPath;
}

std::string PathProc::getPath(DateTime &dateTime)
{
    return DateTime::toString(dateTime).substr(0, DATE_TIME_DIR_LEN);
}

std::vector<std::string> PathProc::getFilesList()
{
    return m_currentPathFiles;
}

std::vector<std::string> PathProc::getDirList()
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

path PathProc::findPathRecursive(path& p, string fileName)
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

string PathProc::findPath(string& fileName)
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

std::vector<std::string> PathProc::getNextFilesList(size_t num)
{
    vector<string> fileList;
    if(num > m_currentPathFiles.size()) /* num > size , return all files list*/
    {
        m_pos = m_currentPathFiles.size() - 1; 
        return m_currentPathFiles;
    }
    else if(m_pos + num >= m_currentPathFiles.size())/* num + pos > size , return last num files list*/
    {
        fileList.resize(num);
        for (int i = m_currentPathFiles.size() - num; i < m_currentPathFiles.size(); i++)
        {
            fileList[i - m_currentPathFiles.size() + num] = m_currentPathFiles[i];
        }
        m_pos = m_currentPathFiles.size() - 1;
    }
    else
    {
        m_pos += num;
        fileList.resize(num);
        for (int i = 0; i < num; i++)
        {
            fileList[i] = m_currentPathFiles[i + m_pos];
        }
    }
    return fileList;
}