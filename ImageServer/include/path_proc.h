#ifndef _PATH_PROC_H_
#define _PATH_PROC_H_
#include <string>
#include <vector>
#include <boost/filesystem.hpp>


#define DATE_TIME_DIR_LEN 10

class DateTime;

class PathProc
{
private:
    /* data */
    std::string m_currentPath; /*  */ 
    unsigned short m_pos; //记录当前读取文件的位置
    std::vector<std::string> m_currentPathFiles;
    std::vector<std::string> m_currentPathDirs;
public:
    explicit PathProc(std::string &rootDir);

    ~PathProc();
    
    std::string getCurrentPath();

    std::string getPath(DateTime &dateTime);

    /* 列出当前文件夹下的所有文件 */
    std::vector<std::string> getFilesList();

    std::vector<std::string> getDirList();

    /* 查找文件所在绝对路径 */
    std::string findPath(std::string& fileName);

    std::vector<std::string> getNextFilesList(size_t num); //获取后面num个文件的名字

private:
    boost::filesystem::path findPathRecursive(boost::filesystem::path& p, std::string fileName);

};

#endif