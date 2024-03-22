#include "fileLib.h"
#include "debug.h"

size_t GetFileSize(FILE* file) 
{
    ASSERT(file != nullptr);

    fseek(file, 0, SEEK_END);

    size_t size = ftell(file);

    fseek(file, 0, SEEK_SET);

    return size;
}

void ReadBufToText(FileData* fileData)  
{
    ASSERT(fileData != nullptr);

    fileData->text[0] = fileData->buf;
    for (size_t pass = 0, line = 1; pass < fileData->bufSize; pass++)
    {
        if (fileData->buf[pass] == '\0')
        {
            fileData->text[line] = fileData->buf + pass + 1;
            line++;
        }
    }

}

size_t CountLines(FileData* fileData)
{
    ASSERT (fileData != nullptr);

    size_t counter = 0;

    for (size_t i = 0; i < fileData->bufSize; i++)
    {
        if (fileData->buf[i] == '\n')
            counter++;
    }
    
    return counter;
}

void SwapChars(FileData* fileData)
{
    ASSERT(fileData != nullptr);
    for (size_t i = 0; i < fileData->bufSize; i++)
    {
        if (fileData->buf[i] == '\n') 
        {
            fileData->buf[i] = '\0';
        }
    }
}


void FileRead(FILE* file, FileData* fileData)
{
    ASSERT(file != nullptr);
    ASSERT(fileData != nullptr);

    fread(fileData->buf, sizeof(char), fileData->bufSize, file);
    *(fileData->buf + fileData->bufSize - 1) = '\0';
}

void FileDataDtor(FileData* fileData)
{
    ASSERT(fileData != nullptr);

    fileData->bufSize = 0;
    fileData->textSize = 0;
    
    free(fileData->buf);
    free(fileData->text);
}

void ParseFile(FILE* file, FileData* fileData) 
{
    ASSERT(file != nullptr);
    ASSERT(fileData != nullptr);

    fileData->bufSize = GetFileSize(file) + 1;

    fileData->buf = (char*)calloc(fileData->bufSize, sizeof(char)); 
    ASSERT(fileData->buf != nullptr);

    FileRead(file, fileData); 

    fileData->textSize = CountLines(fileData);

    fileData->text = (char**)calloc(fileData->textSize, sizeof(char*));
    ASSERT(fileData->text);

    SwapChars(fileData);
    
    ReadBufToText(fileData); 
}

void WriteToFile(FileData* data, FILE* fileout)
{
    ASSERT(data != nullptr);
    ASSERT(fileout != nullptr);

    for (size_t i = 0; i < data->textSize; i++)
    {
        fputs(data->text[i], fileout);
        fputc('\n', fileout);
    }
}
