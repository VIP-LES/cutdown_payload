#include "Job.h"
#include <SD.h>

// Should be implemented by other loggers.

class CSVLogger : public Job
{
public:
    CSVLogger(const char *filename, int chipSelectPin, const char* header, time_t logIntervalMs)
        : Job(logIntervalMs), _filename(filename), _chipSelect(chipSelectPin), _header(header) {}

    bool initialized = false;

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
            _file.flush();
        }
        
        initialized = true;
        return Outcome::Success;
    }

    virtual Outcome execute() override
    {
        if (!_file)
            return Outcome::FailureRetry;

        unsigned int chunkSize = _file.availableForWrite();
        JOB_DEBUG("Buffer Len: " + _buffer.length());
        JOB_DEBUG("Chunk Size: " + chunkSize);

        if (_buffer.length() == 0 || chunkSize == 0)
            return Outcome::Waiting;

        if (_buffer.length() >= chunkSize)
        {
            _file.write(_buffer.c_str(), chunkSize);
            _file.flush();
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
    size_t _maxBufferSize = 512;

    void bufferLine(const String &line) {
        if (!initialized) {
            JOB_DEBUG("Not initialized, so no buffering...");
            return;
        }
        if (_buffer.length() + line.length() + 2 < _maxBufferSize)
        {
            _buffer += line;
            _buffer += "\n";
        }
        else
        {
            JOB_DEBUG("Buffer full, line skipped! This needs to be adjusted.");
        }
    }
};
