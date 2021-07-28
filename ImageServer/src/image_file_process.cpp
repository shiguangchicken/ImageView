#include <iostream>
#include <fstream>
#include <exception>
#include "date_time.h"
#include "path_proc.h"
#include "data_buf.h"
#include "globle_def.h"
#include "image_file_process.h"


ImageFileProcess::ImageFileProcess(std::string &rootPath)
{
    m_path = new PathProc(rootPath);
    m_thread = new boost::thread(boost::bind(&ImageFileProcess::threadEntry, this));
}

ImageFileProcess::~ImageFileProcess()
{
    delete m_path;
    delete m_thread;
}

void ImageFileProcess::run()
{
    m_thread->join();
}

ImageFile_t ImageFileProcess::getOneImage() /*获取 count数量 图片 */
{
    ImageFile_t pBuf;
    if(!m_readBuffers.empty())
    {
        pBuf = m_readBuffers.front();
        m_readBuffers.pop_front();
    }    
    return pBuf;
}

int ImageFileProcess::writeOneImage(ImageFile_t &pBuf)
{
    m_writeBuffers.push_back(pBuf);
    return OK;
}

int ImageFileProcess::loadSomeImage(int count)
{
    std::vector<std::string> strList = m_path->getNextFilesList(count);
    // delete  .jpg , left only date time string


    for (int i = 0; i < count; i++)
    {
        ImageFile_t imageFile;
        imageFile.buf = fileRead(strList[i]);
        imageFile.name = strList[i];
        m_readBuffers.push_back(imageFile);
    }
    return OK;
}

DataBuf* ImageFileProcess::fileRead(std::string fileName)
{
    if(0 < FILE_MAX_CHCHE_NUM - m_readBuffers.size())
    {
        return nullptr;
    }

    std::fstream fs;
    try
    {
        fs.open(fileName, std::fstream::in);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    fs.seekg(0, fs.end);
    int length = fs.tellg();
    fs.seekg(0, fs.beg);
    DataBuf* pBuf = new DataBuf(length);
    fs.read(pBuf->getData(), length);
    fs.close();    

    return pBuf;
}

void ImageFileProcess::fileWrite(DataBuf* pBuf, std::string fileName)
{
    std::fstream fs;
    try
    {
        fs.open(fileName, std::fstream::out);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
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
            // loadSomeImage(FILE_MAX_CHCHE_NUM);
        }
        if(!m_writeBuffers.empty())
        {
            ImageFile_t& img =  m_writeBuffers.front();
            fileWrite(img.buf, img.name);
        }
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
    }
}