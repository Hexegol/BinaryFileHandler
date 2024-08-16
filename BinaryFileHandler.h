#pragma once
#include <vector>

struct Header
{
	char signature[4];
	uint16_t version;
	uint32_t records;

};
struct Data
{
	uint32_t id;    
	std::string name;            
};

class BinaryFileHandler
{
private:
	std::ofstream wOpenFile(const std::string& fileName);
	void WriteHeader(const std::string& fileName, Header& header);
	void wWriteRecords(const std::string& fileName, const std::vector<Data>& records);

	std::ifstream rOpenFile(const std::string& fileName);
	Header  rReadHeader(std::ifstream& file);
	std::vector<Data> rReadRecords(std::ifstream& file, uint32_t recordCount);
public:
	void Write(const std::string& fileName, const std::vector<Data>& records);
	void Read(const std::string& fileName, std::vector<Data>& records);
};

