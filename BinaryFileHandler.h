#pragma once
#include <vector>
#include <fstream>

struct Header
{
	char signature[40];
	uint16_t version;
	uint32_t records;

};
struct Data
{
	uint32_t id;    
	char name[50];            
};

class BinaryFileHandler
{
private:
	std::ofstream wOpenFile(const std::string& fileName);
	void wWriteHeader(std::ofstream& file, const Header& header);
	void wWriteRecords(std::ofstream& file, const std::vector<Data>& records);

	std::ifstream rOpenFile(const std::string& fileName);
	Header  rReadHeader(std::ifstream& file);
	void rReadRecords(std::ifstream& file, std::vector<Data>& records);
public:
	void Write(const std::string& fileName, const std::vector<Data>& records);
	void Read(const std::string& fileName, std::vector<Data>& records);
};
