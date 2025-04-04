#include "Job.h"
#include <SD.h>

// Should be implemented by other loggers.

class CSVLogger : public Job
{
public:
    CSVLogger(const char *filename, int chipSelectPin, const char* header, time_t logIntervalMs)
        : Job(logIntervalMs), _filename(filename), _chipSelect(chipSelectPin), _lastSync(0), _header(header) {}

    virtual Outcome initialize() override
    {    
        pinMode(_chipSelect, OUTPUT);
        if (!SD.begin(_chipSelect))
            return Outcome::FailurePermanent;

        bool fileExisted = SD.exists(_filename);

        _file = SD.open(_filename, FILE_WRITE);
        if (!_file)
            return Outcome::FailurePermanent;

        if (!fileExisted && _header != NULL) {
            _file.println(_header);
        }
        
        return Outcome::Success;
    }

    virtual Outcome execute() override
    {
        if (!_file)
            return Outcome::FailureRetry;

        unsigned int chunkSize = _file.availableForWrite();

        if (_buffer.length() == 0 || chunkSize == 0)
            return Outcome::Waiting;

        if (_buffer.length() >= chunkSize)
        {
            _file.write(_buffer.c_str(), chunkSize);
            _buffer.remove(0, chunkSize);
        }

        last_execution_time = millis();
        return Outcome::Success;
    }

    virtual Outcome close() override
    {
        if (_file)
        {
            _file.flush();
            _file.close();
            return Outcome::Success;
        }
        return Outcome::FailureRetry;
    }

protected:
    const char *_filename;
    int _chipSelect;
    File _file;
    String _buffer;
    unsigned long _lastSync;
    const char* _header;
};
