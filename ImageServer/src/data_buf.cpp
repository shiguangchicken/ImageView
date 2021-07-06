#include "data_buf.h"

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
DataBuf::~DataBuf()
{
    delete[] m_data;
    m_size = 0;
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
    return *this;
}
bool DataBuf::operator==(DataBuf &other)
{
    return m_data == other.m_data;
}