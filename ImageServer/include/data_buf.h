
#ifndef _DATA_BUF_H
#define _DATA_BUF_H
#include <string>
class DataBuf
{
private:
    char* m_data;/* data */
    size_t m_size;
public:
    explicit DataBuf(size_t size);
    ~DataBuf();

    DataBuf(const DataBuf &other) = delete;

    DataBuf& operator=(const DataBuf &other) = delete;

    DataBuf(DataBuf &&other);

    DataBuf& operator=(DataBuf &&other);

    bool operator==(DataBuf &other);

    inline char* getData()
    {
        return m_data;
    };
    inline size_t getSize()
    {
        return m_size;
    };
};

#endif